#include <cover_curve/solvers/envelope_sqp.hpp>

#include "../fast_grid_dp/solver.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cover_curve {
namespace {

struct Evaluation {
    Result result;
    std::vector<double> gradient;
};

std::vector<double> heightsFromResult(const Result& result) {
    std::vector<double> heights(result.breakpoints.size(), 0.0);
    if (result.segments.empty())
        return heights;

    for (std::size_t i = 0; i < result.segments.size(); ++i) {
        const auto& s = result.segments[i];
        heights[i] = s.slope * s.x0 + s.intercept;
    }

    const auto& last = result.segments.back();
    heights.back() = last.slope * last.x1 + last.intercept;
    return heights;
}

std::vector<double> curvatureSeed(
    const Function& f,
    double a,
    double b,
    int n,
    int samples
) {
    samples = std::max(samples, 17);
    if ((samples & 1) == 0)
        ++samples;

    std::vector<double> xs(samples), density(samples, 0.0), cdf(samples, 0.0);
    const double h = (b - a) / (samples - 1);
    for (int i = 0; i < samples; ++i)
        xs[i] = a + i * h;

    // Three-point curvature estimate.  The asymptotic theory gives knot
    // density proportional to c^(1/3)|f''|^(1/3), where c=1/12 on convex
    // pieces and c=1/24 on concave pieces.  A small floor prevents a zero
    // curvature/inflection point from creating an unusably large gap.
    const double floor = 1e-8;
    for (int i = 1; i + 1 < samples; ++i) {
        const double d2 =
            (f(xs[i + 1]) - 2.0 * f(xs[i]) + f(xs[i - 1])) / (h * h);
        const double k = std::abs(d2);
        const double c = d2 >= 0.0 ? 1.0 / 12.0 : 1.0 / 24.0;
        density[i] = std::max(floor, std::cbrt(c * k));
    }
    density.front() = density[1];
    density.back() = density[samples - 2];

    for (int i = 1; i < samples; ++i)
        cdf[i] = cdf[i - 1] +
            0.5 * h * (density[i - 1] + density[i]);

    const double total = cdf.back();
    if (!(total > 0.0) || !std::isfinite(total)) {
        std::vector<double> uniform(n + 1);
        for (int i = 0; i <= n; ++i)
            uniform[i] = a + (b - a) * i / n;
        return uniform;
    }

    std::vector<double> result(n + 1);
    result.front() = a;
    result.back() = b;
    for (int j = 1; j < n; ++j) {
        const double target = total * j / n;
        const auto it = std::lower_bound(cdf.begin(), cdf.end(), target);
        const int k = static_cast<int>(std::distance(cdf.begin(), it));
        if (k <= 0) {
            result[j] = xs[0];
            continue;
        }
        const double c0 = cdf[k - 1];
        const double c1 = cdf[k];
        const double u = (target - c0) / std::max(c1 - c0, 1e-30);
        result[j] = xs[k - 1] + u * (xs[k] - xs[k - 1]);
    }
    return result;
}

std::vector<double> numericalDerivative(
    const Function& f,
    const std::vector<double>& x,
    double a,
    double b,
    double relativeStep
) {
    std::vector<double> d(x.size(), 0.0);
    const double scale = std::max(1.0, b - a);

    for (std::size_t i = 1; i + 1 < x.size(); ++i) {
        const double h = relativeStep *
            std::max({1.0, std::abs(x[i]), scale});
        const double left = std::max(a, x[i] - h);
        const double right = std::min(b, x[i] + h);

        if (!(left < right))
            continue;

        d[i] = (f(right) - f(left)) / (right - left);
    }

    return d;
}

std::vector<double> envelopeGradient(
    const Function& f,
    const std::vector<double>& points,
    const Result& result,
    const std::vector<DirectHeightContact>& contacts,
    double a,
    double b,
    double finiteDifferenceStep
) {
    const std::size_t n = points.size() - 1;
    const std::vector<double> y = heightsFromResult(result);
    std::vector<double> gradient(points.size(), 0.0);

    // Derivative of the trapezoidal objective with respect to an interior
    // breakpoint x_j.
    for (std::size_t j = 1; j < n; ++j)
        gradient[j] +=
            0.5 * (y[j - 1] - y[j + 1]);

    // Contact contribution from the semi-infinite constraints
    // f(z) - L_i(z) <= 0.
    std::vector<double> dualWeight(points.size(), 0.0);
    std::vector<double> objectiveCoeff(points.size(), 0.0);

    for (std::size_t i = 0; i < n; ++i) {
        const double h = points[i + 1] - points[i];
        objectiveCoeff[i] += h / 2.0;
        objectiveCoeff[i + 1] += h / 2.0;
    }

    for (const auto& contact : contacts) {
        const int i = contact.segment;
        if (i < 0 || static_cast<std::size_t>(i) >= n)
            continue;

        const double x0 = points[i];
        const double x1 = points[i + 1];
        const double h = x1 - x0;
        const double t =
            (contact.x - x0) / h;
        const double lambda = contact.multiplier;
        const double dy = y[i + 1] - y[i];

        if (i > 0)
            gradient[i] +=
                lambda * dy * (x1 - contact.x) / (h * h);

        if (static_cast<std::size_t>(i + 1) < n)
            gradient[i + 1] +=
                lambda * dy * (contact.x - x0) / (h * h);

        dualWeight[i] += lambda * (1.0 - t);
        dualWeight[i + 1] += lambda * t;
    }

    // z = y - f(x) was used by the fixed-height LP.  Its nonnegativity
    // multipliers are
    //
    //   mu_j = c_j - sum_k lambda_k w_{kj}.
    //
    // These are the endpoint atoms of the continuous majorant constraint.
    // Their contribution is mu_j f'(x_j).  The solver accepts arbitrary
    // continuous Functions, so only this endpoint derivative is numerical;
    // the expensive derivative of the complete objective is still obtained
    // analytically from the LP dual.
    const auto df =
        numericalDerivative(
            f, points, a, b, finiteDifferenceStep
        );

    for (std::size_t j = 1; j < n; ++j) {
        const double mu =
            objectiveCoeff[j] - dualWeight[j];

        if (mu > 1e-9)
            gradient[j] += mu * df[j];
    }

    return gradient;
}

Evaluation evaluate(
    const Function& f,
    const std::vector<double>& points,
    const EnvelopeSQPOptions& options,
    double a,
    double b
) {
    const auto detailed =
        directHeightSolveDetailed(
            f, points, options.innerOptions
        );

    Evaluation e;
    e.result = detailed.result;
    e.gradient = envelopeGradient(
        f,
        points,
        e.result,
        detailed.contacts,
        a,
        b,
        options.finiteDifferenceStep
    );
    return e;
}

std::vector<double> lbfgsDirection(
    const std::vector<double>& gradient,
    const std::vector<std::vector<double>>& sHistory,
    const std::vector<std::vector<double>>& yHistory
) {
    std::vector<double> q = gradient;
    const std::size_t m = sHistory.size();
    std::vector<double> alpha(m, 0.0);

    for (std::size_t k = m; k-- > 0;) {
        const double ys =
            std::inner_product(
                yHistory[k].begin(),
                yHistory[k].end(),
                sHistory[k].begin(),
                0.0
            );
        if (ys <= 1e-14)
            continue;

        alpha[k] =
            std::inner_product(
                sHistory[k].begin(),
                sHistory[k].end(),
                q.begin(),
                0.0
            ) / ys;

        for (std::size_t i = 0; i < q.size(); ++i)
            q[i] -= alpha[k] * yHistory[k][i];
    }

    double gamma = 1.0;
    if (!yHistory.empty()) {
        const auto& lastS = sHistory.back();
        const auto& lastY = yHistory.back();
        const double yy =
            std::inner_product(
                lastY.begin(), lastY.end(),
                lastY.begin(), 0.0
            );
        const double sy =
            std::inner_product(
                lastS.begin(), lastS.end(),
                lastY.begin(), 0.0
            );
        if (yy > 1e-14)
            gamma = sy / yy;
    }

    for (double& v : q)
        v *= gamma;

    for (std::size_t k = 0; k < m; ++k) {
        const double ys =
            std::inner_product(
                yHistory[k].begin(),
                yHistory[k].end(),
                sHistory[k].begin(),
                0.0
            );
        if (ys <= 1e-14)
            continue;

        const double beta =
            std::inner_product(
                yHistory[k].begin(),
                yHistory[k].end(),
                q.begin(),
                0.0
            ) / ys;

        for (std::size_t i = 0; i < q.size(); ++i)
            q[i] += sHistory[k][i] * (alpha[k] - beta);
    }

    for (double& v : q)
        v = -v;

    return q;
}

double maxFeasibleStep(
    const std::vector<double>& x,
    const std::vector<double>& direction,
    double a,
    double b
) {
    double alpha = 1.0;
    const double eps =
        1e-10 * std::max(1.0, b - a);

    for (std::size_t i = 1; i + 1 < x.size(); ++i) {
        if (direction[i] > 0.0) {
            alpha = std::min(
                alpha,
                (x[i + 1] - eps - x[i]) /
                    direction[i]
            );
        } else if (direction[i] < 0.0) {
            alpha = std::min(
                alpha,
                (x[i - 1] + eps - x[i]) /
                    direction[i]
            );
        }
    }

    return std::max(0.0, alpha);
}

bool validInterior(
    const std::vector<double>& x,
    double a,
    double b
) {
    if (x.size() < 2 || x.front() != a || x.back() != b)
        return false;

    for (std::size_t i = 1; i < x.size(); ++i)
        if (!(x[i] > x[i - 1]) || x[i] > b)
            return false;

    return true;
}

}

EnvelopeSQPDetailedResult envelopeSQPSolveDetailed(
    const Function& f,
    double a,
    double b,
    int n,
    const EnvelopeSQPOptions& options
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");
    if (!std::isfinite(a) || !std::isfinite(b) || a >= b)
        throw std::invalid_argument("Require finite a < b.");
    if (n < 1)
        throw std::invalid_argument("n must be positive.");
    if (options.maxIterations <= 0 ||
        options.memory <= 0 ||
        options.lineSearchSteps <= 0 ||
        options.seeds <= 0 ||
        options.gradientTolerance <= 0.0 ||
        options.stepTolerance <= 0.0 ||
        options.finiteDifferenceStep <= 0.0 ||
        options.curvatureSamples < 17)
        throw std::invalid_argument("Invalid envelope SQP options.");

    std::vector<std::vector<double>> seeds;
    seeds.reserve(options.seeds);

    std::vector<double> uniform(n + 1);
    for (int i = 0; i <= n; ++i)
        uniform[i] = a + (b - a) * i / n;

    if (!options.initialBreakpoints.empty()) {
        if (options.initialBreakpoints.size() !=
            static_cast<std::size_t>(n + 1) ||
            !validInterior(
                options.initialBreakpoints, a, b
            )) {
            throw std::invalid_argument(
                "initialBreakpoints must contain n+1 ordered points."
            );
        }
        seeds.push_back(options.initialBreakpoints);
    } else {
        seeds.push_back(uniform);
    }

    // The second independent seed is theory-driven: distribute breakpoints
    // according to the asymptotic L1 majorant density. This replaces the old
    // arbitrary power-law seeds and usually gives a much better starting point
    // at essentially negligible cost compared with one inner LP solve.
    if (options.useCurvatureSeed &&
        static_cast<int>(seeds.size()) < options.seeds) {
        seeds.push_back(curvatureSeed(
            f, a, b, n, options.curvatureSamples
        ));
    }

    // If more seeds are explicitly requested, retain cheap deterministic
    // alternatives rather than invoking the expensive global grid solver.
    for (int s = 1;
         static_cast<int>(seeds.size()) < options.seeds;
         ++s) {
        const double power = s % 2 == 1 ? 2.0 : 0.5;
        std::vector<double> candidate(n + 1);
        for (int i = 0; i <= n; ++i) {
            const double u = static_cast<double>(i) / n;
            candidate[i] = a + (b - a) * std::pow(u, power);
        }
        seeds.push_back(std::move(candidate));
    }

    if (options.includeFastGridSeed &&
        static_cast<int>(seeds.size()) < options.seeds) {
        const Result coarse =
            algorithms::fast_grid_dp::solve(
                f, a, b, n, 1e-5, std::max(n, 8), 32
            );
        if (validInterior(coarse.breakpoints, a, b))
            seeds.push_back(coarse.breakpoints);
    }

    Result globalBest{};
    std::vector<double> globalGradient;
    int globalIterations = 0;
    int globalStart = 0;
    bool haveBest = false;

    for (std::size_t seedIndex = 0;
         seedIndex < seeds.size();
         ++seedIndex) {
        const auto& seed = seeds[seedIndex];
        std::vector<double> x = seed;
        Evaluation current =
            evaluate(f, x, options, a, b);
        int iterationsDone = 0;

        std::vector<std::vector<double>> sHistory;
        std::vector<std::vector<double>> yHistory;

        for (int iteration = 0;
             iteration < options.maxIterations;
             ++iteration) {
            double gradNorm = 0.0;
            for (std::size_t i = 1; i < x.size() - 1; ++i)
                gradNorm = std::max(
                    gradNorm, std::abs(current.gradient[i])
                );

            if (gradNorm <= options.gradientTolerance)
                break;

            std::vector<double> direction =
                lbfgsDirection(
                    current.gradient,
                    sHistory,
                    yHistory
                );

            double directionalDerivative = 0.0;
            for (std::size_t i = 1; i < x.size() - 1; ++i)
                directionalDerivative +=
                    current.gradient[i] * direction[i];

            if (!(directionalDerivative < 0.0)) {
                direction.assign(x.size(), 0.0);
                for (std::size_t i = 1; i < x.size() - 1; ++i)
                    direction[i] = -current.gradient[i];

                directionalDerivative = 0.0;
                for (std::size_t i = 1; i < x.size() - 1; ++i)
                    directionalDerivative +=
                        current.gradient[i] * direction[i];
            }

            double alpha =
                maxFeasibleStep(x, direction, a, b);
            if (!(alpha > 0.0))
                break;

            const double oldValue = current.result.value;
            bool accepted = false;
            Evaluation next;
            std::vector<double> nextX;

            for (int ls = 0;
                 ls < options.lineSearchSteps;
                 ++ls) {
                nextX = x;
                for (std::size_t i = 1;
                     i < x.size() - 1; ++i)
                    nextX[i] += alpha * direction[i];

                if (!validInterior(nextX, a, b)) {
                    alpha *= 0.5;
                    continue;
                }

                next =
                    evaluate(f, nextX, options, a, b);

                if (next.result.value <=
                    oldValue +
                    options.sufficientDecrease *
                    alpha * directionalDerivative) {
                    accepted = true;
                    break;
                }

                alpha *= 0.5;
            }

            if (!accepted)
                break;

            std::vector<double> s(x.size(), 0.0);
            std::vector<double> yy(x.size(), 0.0);
            for (std::size_t i = 1; i < x.size() - 1; ++i) {
                s[i] = nextX[i] - x[i];
                yy[i] =
                    next.gradient[i] -
                    current.gradient[i];
            }

            const double sy =
                std::inner_product(
                    s.begin(), s.end(), yy.begin(), 0.0
                );

            if (sy > 1e-12) {
                if (static_cast<int>(sHistory.size()) >=
                    options.memory) {
                    sHistory.erase(sHistory.begin());
                    yHistory.erase(yHistory.begin());
                }
                sHistory.push_back(std::move(s));
                yHistory.push_back(std::move(yy));
            }

            x = std::move(nextX);
            current = std::move(next);
            iterationsDone = iteration + 1;

            if (std::abs(oldValue - current.result.value) <=
                options.stepTolerance *
                std::max(1.0, std::abs(oldValue)))
                break;
        }

        if (!haveBest ||
            current.result.value < globalBest.value) {
            globalBest = std::move(current.result);
            globalGradient = std::move(current.gradient);
            globalIterations = iterationsDone;
            globalStart = static_cast<int>(seedIndex);
            haveBest = true;
        }
    }

    return {
        std::move(globalBest),
        std::move(globalGradient),
        globalIterations,
        globalStart
    };
}

Result envelopeSQPSolve(
    const Function& f,
    double a,
    double b,
    int n,
    const EnvelopeSQPOptions& options
) {
    return envelopeSQPSolveDetailed(f, a, b, n, options).result;
}

}
