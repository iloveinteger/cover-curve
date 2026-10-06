#include <cover_curve/solvers/direct_height.hpp>

#include "../../numerical/integration.hpp"
#include "../../numerical/support_max.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cover_curve {
namespace {

class Simplex {
public:
    Simplex(
        const std::vector<std::vector<double>>& A,
        const std::vector<double>& b,
        const std::vector<double>& c
    )
        : m_(static_cast<int>(b.size())),
          n_(static_cast<int>(c.size())),
          B_(m_),
          N_(n_ + 1),
          D_(m_ + 2, std::vector<double>(n_ + 2, 0.0)) {
        for (int i = 0; i < m_; ++i)
            for (int j = 0; j < n_; ++j)
                D_[i][j] = A[i][j];

        for (int i = 0; i < m_; ++i) {
            B_[i] = n_ + i;
            D_[i][n_] = -1.0;
            D_[i][n_ + 1] = b[i];
        }

        for (int j = 0; j < n_; ++j)
            D_[m_][j] = -c[j];

        N_[n_] = -1;
        D_[m_ + 1][n_] = 1.0;
    }

    bool solve(std::vector<double>& x) {
        if (m_ == 0) {
            x.assign(n_, 0.0);
            return true;
        }

        int r = 0;
        for (int i = 1; i < m_; ++i)
            if (D_[i][n_ + 1] < D_[r][n_ + 1])
                r = i;

        if (D_[r][n_ + 1] < -kEps) {
            pivot(r, n_);
            if (!simplex(2) || D_[m_ + 1][n_ + 1] < -kEps)
                return false;

            for (int i = 0; i < m_; ++i) {
                if (B_[i] != -1)
                    continue;

                int s = 0;
                for (int j = 1; j <= n_; ++j) {
                    if (s == 0 ||
                        D_[i][j] < D_[i][s] ||
                        (D_[i][j] == D_[i][s] && N_[j] < N_[s])) {
                        s = j;
                    }
                }
                pivot(i, s);
            }
        }

        if (!simplex(1))
            return false;

        x.assign(n_, 0.0);
        for (int i = 0; i < m_; ++i)
            if (B_[i] >= 0 && B_[i] < n_)
                x[B_[i]] = std::max(0.0, D_[i][n_ + 1]);

        return true;
    }

private:
    static constexpr double kEps = 1e-10;

    void pivot(int r, int s) {
        const double inv = 1.0 / D_[r][s];

        for (int i = 0; i < m_ + 2; ++i) {
            if (i == r)
                continue;
            const double factor = D_[i][s] * inv;
            for (int j = 0; j < n_ + 2; ++j)
                if (j != s)
                    D_[i][j] -= D_[r][j] * factor;
            D_[i][s] = D_[r][s] * factor;
        }

        for (int j = 0; j < n_ + 2; ++j)
            if (j != s)
                D_[r][j] *= inv;

        for (int i = 0; i < m_ + 2; ++i)
            if (i != r)
                D_[i][s] *= -inv;

        D_[r][s] = inv;
        std::swap(B_[r], N_[s]);
    }

    bool simplex(int phase) {
        const int objective = m_ + phase - 1;

        for (;;) {
            int s = -1;
            for (int j = 0; j <= n_; ++j) {
                if (N_[j] == -phase)
                    continue;
                if (s == -1 ||
                    D_[objective][j] < D_[objective][s] - kEps ||
                    (std::abs(D_[objective][j] - D_[objective][s]) <= kEps &&
                     N_[j] < N_[s])) {
                    s = j;
                }
            }

            if (s == -1 || D_[objective][s] >= -kEps)
                return true;

            int r = -1;
            for (int i = 0; i < m_; ++i) {
                if (D_[i][s] <= kEps)
                    continue;

                const double ratio =
                    D_[i][n_ + 1] / D_[i][s];

                if (r == -1 ||
                    ratio < D_[r][n_ + 1] / D_[r][s] - kEps ||
                    (std::abs(
                         ratio - D_[r][n_ + 1] / D_[r][s]
                     ) <= kEps &&
                     B_[i] < B_[r])) {
                    r = i;
                }
            }

            if (r == -1)
                return false;

            pivot(r, s);
        }
    }

    int m_;
    int n_;
    std::vector<int> B_;
    std::vector<int> N_;
    std::vector<std::vector<double>> D_;
};

struct Constraint {
    std::vector<double> a;
    double rhs;
};

Result buildResult(
    const Function& f,
    const std::vector<double>& points,
    const std::vector<double>& heights
) {
    Result result;
    result.breakpoints = points;
    result.value = 0.0;
    result.segments.reserve(points.size() - 1);

    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        const double x0 = points[i];
        const double x1 = points[i + 1];
        const double y0 = heights[i];
        const double y1 = heights[i + 1];
        const double dx = x1 - x0;
        const double slope = (y1 - y0) / dx;
        const double intercept = y0 - slope * x0;
        const double cost =
            dx * (y0 + y1) / 2.0 -
            numerical::adaptiveIntegral(f, x0, x1);

        result.value += cost;
        result.segments.push_back({
            x0, x1, slope, intercept, cost,
            std::numeric_limits<double>::quiet_NaN()
        });
    }

    result.value = std::max(0.0, result.value);
    return result;
}

double lineValue(
    const std::vector<double>& points,
    const std::vector<double>& heights,
    std::size_t segment,
    double x
) {
    const double x0 = points[segment];
    const double x1 = points[segment + 1];
    const double t = (x - x0) / (x1 - x0);
    return (1.0 - t) * heights[segment] + t * heights[segment + 1];
}

}

Result directHeightSolve(
    const Function& f,
    const std::vector<double>& points,
    const DirectHeightOptions& options
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (points.size() < 2)
        throw std::invalid_argument(
            "At least two breakpoints are required."
        );

    if (options.maxSweeps <= 0 ||
        !std::isfinite(options.tolerance) ||
        options.tolerance <= 0.0) {
        throw std::invalid_argument("Invalid direct-height options.");
    }

    for (std::size_t i = 1; i < points.size(); ++i) {
        if (!std::isfinite(points[i - 1]) ||
            !std::isfinite(points[i]) ||
            points[i] <= points[i - 1]) {
            throw std::invalid_argument(
                "Breakpoints must be finite and strictly increasing."
            );
        }
    }

    const int dimension = static_cast<int>(points.size());
    std::vector<double> lower(dimension);

    for (int i = 0; i < dimension; ++i) {
        lower[i] = f(points[i]);
        if (!std::isfinite(lower[i]))
            throw std::runtime_error(
                "Function returned a non-finite value."
            );
    }

    // Write y = lower + z, z >= 0.  For a sample/contact x in segment i,
    //
    //   (1-t)y_i + t y_{i+1} >= f(x)
    //
    // becomes one linear inequality in z.  The objective is linear in z.
    // We solve the resulting finite LP and add violated continuous
    // constraints until every segment's separation oracle is satisfied.
    std::vector<Constraint> constraints;

    const auto addConstraint = [&](std::size_t segment, double x) {
        const double x0 = points[segment];
        const double x1 = points[segment + 1];
        const double t = (x - x0) / (x1 - x0);

        Constraint c;
        c.a.assign(dimension, 0.0);
        c.a[segment] = -(1.0 - t);
        c.a[segment + 1] = -t;
        c.rhs =
            -(f(x) -
              ((1.0 - t) * lower[segment] +
               t * lower[segment + 1]));

        constraints.push_back(std::move(c));
    };

    // Endpoints are already enforced by z >= 0.  Midpoints give the first
    // LP a useful nontrivial relaxation instead of starting completely
    // unconstrained above the endpoint values.
    for (std::size_t i = 0; i + 1 < points.size(); ++i)
        addConstraint(i, (points[i] + points[i + 1]) / 2.0);

    std::vector<double> widths(points.size() - 1);
    std::vector<double> objective(dimension, 0.0);
    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        widths[i] = points[i + 1] - points[i];
        objective[i] += widths[i] / 2.0;
        objective[i + 1] += widths[i] / 2.0;
    }

    double integralF = 0.0;
    for (std::size_t i = 0; i + 1 < points.size(); ++i)
        integralF += numerical::adaptiveIntegral(
            f, points[i], points[i + 1]
        );

    std::vector<double> heights(dimension);
    const int maxIterations = std::max(
        4,
        options.maxSweeps * static_cast<int>(points.size())
    );

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        std::vector<std::vector<double>> A;
        std::vector<double> b;
        A.reserve(constraints.size());
        b.reserve(constraints.size());

        for (const auto& constraint : constraints) {
            A.push_back(constraint.a);
            b.push_back(constraint.rhs);
        }

        // Minimize objective*z == maximize -objective*z.
        std::vector<double> maximizeObjective(dimension);
        for (int j = 0; j < dimension; ++j)
            maximizeObjective[j] = -objective[j];

        Simplex lp(A, b, maximizeObjective);
        std::vector<double> z;
        if (!lp.solve(z))
            throw std::runtime_error(
                "Direct height LP became infeasible."
            );

        for (int j = 0; j < dimension; ++j)
            heights[j] = lower[j] + std::max(0.0, z[j]);

        bool added = false;
        double worstViolation = 0.0;

        for (std::size_t i = 0; i + 1 < points.size(); ++i) {
            const double x0 = points[i];
            const double x1 = points[i + 1];

            const auto violation = [&](double x) {
                return f(x) - lineValue(
                    points, heights, i, x
                );
            };

            const auto support =
                numerical::adaptiveSupportMaximum(
                    violation,
                    x0,
                    x1,
                    0.0,
                    12,
                    4,
                    4
                );

            if (!std::isfinite(support.value))
                throw std::runtime_error(
                    "Failed to separate a segment constraint."
                );

            worstViolation =
                std::max(worstViolation, support.value);

            const double scale =
                std::max({
                    1.0,
                    std::abs(f(x0)),
                    std::abs(f(x1)),
                    std::abs(heights[i]),
                    std::abs(heights[i + 1])
                });

            if (support.value > options.tolerance * scale) {
                addConstraint(i, support.x);
                added = true;
            }
        }

        if (!added)
            break;
    }

    // One final separation pass makes the stopping criterion explicit.
    for (std::size_t i = 0; i + 1 < points.size(); ++i) {
        const auto violation = [&](double x) {
            return f(x) - lineValue(
                points, heights, i, x
            );
        };
        const auto support =
            numerical::adaptiveSupportMaximum(
                violation,
                points[i],
                points[i + 1],
                0.0,
                16,
                5,
                5
            );

        const double scale =
            std::max({
                1.0,
                std::abs(f(points[i])),
                std::abs(f(points[i + 1])),
                std::abs(heights[i]),
                std::abs(heights[i + 1])
            });

        if (!std::isfinite(support.value) ||
            support.value > options.tolerance * scale) {
            throw std::runtime_error(
                "Direct height solver did not reach continuous feasibility "
                "(segment " + std::to_string(i) +
                ", violation=" + std::to_string(support.value) +
                ", x=" + std::to_string(support.x) +
                ", tolerance=" +
                std::to_string(options.tolerance * scale) + ")."
            );
        }
    }

    Result result = buildResult(f, points, heights);
    if (result.value < -options.tolerance)
        throw std::runtime_error(
            "Direct height solver produced a negative majorant cost."
        );

    return result;
}

Result directHeightSolve(
    const Function& f,
    double a,
    double b,
    int n,
    const DirectHeightOptions& options
) {
    if (!std::isfinite(a) ||
        !std::isfinite(b) ||
        a >= b) {
        throw std::invalid_argument("Require finite a < b.");
    }

    if (n < 1)
        throw std::invalid_argument("n must be positive.");

    std::vector<double> points(n + 1);
    for (int i = 0; i <= n; ++i)
        points[i] = a + (b - a) * i / n;

    return directHeightSolve(f, points, options);
}

}
