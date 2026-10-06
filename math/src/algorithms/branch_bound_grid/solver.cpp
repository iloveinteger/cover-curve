#include "solver.hpp"

#include "../fast_grid_dp/grid_dp.hpp"
#include "../../numerical/integration.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cover_curve::algorithms::branch_bound_grid {

namespace {

constexpr double kComparisonSlack = 1e-10;

struct GridProblem {
    const Function& f;
    std::vector<double> x;
    int n;
    int sampleCount;

    // Independent sampled-segment excess costs. Each is a lower bound for
    // the corresponding true one-segment excess cost.
    std::vector<std::vector<double>> lower;

    // relaxed[k][i] is a lower bound for k remaining segments from i to
    // the right endpoint after dropping continuity and allowing the sampled
    // finite constraints only.
    std::vector<std::vector<double>> relaxed;

    double best = std::numeric_limits<double>::infinity();
    Result bestResult{};
};

double sampledLineIntegralLowerBound(
    const Function& f,
    double u,
    double v,
    int samples
) {
    struct Point {
        double x;
        double y;
    };

    std::vector<Point> points;
    points.reserve(samples + 1);

    for (int k = 0; k <= samples; ++k) {
        const double t = static_cast<double>(k) / samples;
        const double x = u + (v - u) * t;
        const double y = f(x);

        if (!std::isfinite(y))
            throw std::runtime_error(
                "Function evaluation returned a non-finite value."
            );

        points.push_back({x, y});
    }

    const double dx = v - u;
    const double moment = (v * v - u * u) / 2.0;
    double best = std::numeric_limits<double>::infinity();

    const auto evaluate = [&](double slope) {
        double intercept = -std::numeric_limits<double>::infinity();

        for (const auto& point : points)
            intercept = std::max(
                intercept,
                point.y - slope * point.x
            );

        best = std::min(
            best,
            moment * slope + dx * intercept
        );
    };

    // The finite sampled LP is a two-variable LP. Eliminating the
    // intercept leaves a convex piecewise-linear function of the slope.
    // Its minimum is attained at a breakpoint or on a flat piece.
    evaluate(0.0);

    for (std::size_t i = 0; i < points.size(); ++i) {
        for (std::size_t j = i + 1; j < points.size(); ++j) {
            const double dxPoints =
                points[j].x - points[i].x;

            if (dxPoints <= 0.0)
                continue;

            evaluate(
                (points[j].y - points[i].y) / dxPoints
            );
        }
    }

    return best;
}

void buildRelaxation(GridProblem& problem) {
    const int m =
        static_cast<int>(problem.x.size()) - 1;

    problem.lower.assign(
        m + 1,
        std::vector<double>(
            m + 1,
            std::numeric_limits<double>::infinity()
        )
    );

    for (int i = 0; i < m; ++i) {
        for (int j = i + 1; j <= m; ++j) {
            const double lineIntegral =
                sampledLineIntegralLowerBound(
                    problem.f,
                    problem.x[i],
                    problem.x[j],
                    problem.sampleCount
                );

            const double fIntegral =
                numerical::adaptiveIntegral(
                    problem.f,
                    problem.x[i],
                    problem.x[j]
                );

            // The finite sampled problem relaxes the true semi-infinite
            // majorant constraint, hence its excess is a valid lower bound.
            problem.lower[i][j] =
                std::max(0.0, lineIntegral - fIntegral);
        }
    }

    problem.relaxed.assign(
        problem.n + 1,
        std::vector<double>(
            m + 1,
            std::numeric_limits<double>::infinity()
        )
    );

    problem.relaxed[0][m] = 0.0;

    // This is only a relaxation used for pruning. It is intentionally
    // independent of the continuous-height Bellman optimization.
    for (int k = 1; k <= problem.n; ++k) {
        for (int i = 0; i < m; ++i) {
            const int lastStart = m - (k - 1);

            for (int j = i + 1; j <= lastStart; ++j) {
                if (!std::isfinite(problem.relaxed[k - 1][j]))
                    continue;

                problem.relaxed[k][i] = std::min(
                    problem.relaxed[k][i],
                    problem.lower[i][j] +
                    problem.relaxed[k - 1][j]
                );
            }
        }
    }
}

void search(
    GridProblem& problem,
    int current,
    int remaining,
    double partialLower,
    std::vector<int>& path
) {
    if (remaining == 0) {
        const int m =
            static_cast<int>(problem.x.size()) - 1;

        if (current != m ||
            partialLower >= problem.best - kComparisonSlack) {
            return;
        }

        std::vector<double> points;
        points.reserve(path.size());

        for (const int index : path)
            points.push_back(problem.x[index]);

        // Fixed breakpoints leave only the continuous shared-height
        // optimization. This gives a feasible upper candidate.
        const Result candidate =
            fast_grid_dp::solveGridDPOnGrid(
                problem.f,
                points,
                problem.n
            );

        if (std::isfinite(candidate.value) &&
            candidate.value < problem.best) {
            problem.best = candidate.value;
            problem.bestResult = candidate;
        }
        return;
    }

    const double tail =
        problem.relaxed[remaining][current];

    if (!std::isfinite(tail) ||
        partialLower + tail >= problem.best - kComparisonSlack) {
        return;
    }

    const int m =
        static_cast<int>(problem.x.size()) - 1;

    // Keep enough cells for the remaining segments.
    const int lastNext = m - (remaining - 1);

    for (int next = current + 1;
         next <= lastNext;
         ++next) {
        const double newLower =
            partialLower + problem.lower[current][next];

        if (newLower >= problem.best - kComparisonSlack)
            continue;

        path.push_back(next);
        search(
            problem,
            next,
            remaining - 1,
            newLower,
            path
        );
        path.pop_back();
    }
}

Result solveOnGrid(
    const Function& f,
    double a,
    double b,
    int n,
    int N,
    int sampleCount
) {
    std::vector<double> grid(N + 1);

    for (int i = 0; i <= N; ++i) {
        grid[i] =
            a + (b - a) * static_cast<double>(i) / N;
    }

    GridProblem problem{
        f,
        std::move(grid),
        n,
        sampleCount
    };

    buildRelaxation(problem);

    // Any feasible numerical solution is an upper bound for pruning.
    // The full-grid fast solver supplies the initial incumbent.
    problem.bestResult =
        fast_grid_dp::solveGridDPOnGrid(
            f,
            problem.x,
            n
        );
    problem.best = problem.bestResult.value;

    std::vector<int> path;
    path.reserve(n + 1);
    path.push_back(0);

    search(
        problem,
        0,
        n,
        0.0,
        path
    );

    if (!std::isfinite(problem.best))
        throw std::runtime_error(
            "Branch-and-bound failed to find a feasible solution."
        );

    return problem.bestResult;
}

} // namespace

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance,
    int initialN,
    int maxN,
    int sampleCount
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (!std::isfinite(a) ||
        !std::isfinite(b) ||
        a >= b) {
        throw std::invalid_argument("Require finite a < b.");
    }

    if (n < 1)
        throw std::invalid_argument("n must be positive.");

    if (!std::isfinite(tolerance) || tolerance <= 0.0)
        throw std::invalid_argument(
            "tolerance must be positive."
        );

    if (initialN < n)
        initialN = n;

    if (maxN < initialN)
        throw std::invalid_argument(
            "maxN must satisfy maxN >= initialN."
        );

    if (sampleCount < 3)
        throw std::invalid_argument(
            "sampleCount must be at least 3."
        );

    int N = initialN;
    Result previous{};
    bool havePrevious = false;

    while (true) {
        Result current =
            solveOnGrid(
                f, a, b, n, N, sampleCount
            );

        if (havePrevious) {
            const double scale =
                std::max({
                    1.0,
                    std::abs(previous.value),
                    std::abs(current.value)
                });

            if (std::abs(current.value - previous.value)
                <= tolerance * scale) {
                return current;
            }
        }

        if (N >= maxN)
            return current;

        previous = std::move(current);
        havePrevious = true;
        N = std::min(2 * N, maxN);
    }
}

} // namespace cover_curve::algorithms::branch_bound_grid

namespace cover_curve {

Result branchBoundGrid(
    const Function& f,
    double a,
    double b,
    int n
) {
    return algorithms::branch_bound_grid::solve(
        f, a, b, n
    );
}

} // namespace cover_curve
