#include <cover_curve/solvers/direct_height.hpp>

#include "../numerical/integration.hpp"
#include "../numerical/support_max.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cover_curve {
namespace {

struct Oracle {
    const Function& f;
    const std::vector<double>& x;
    double fmax;

    double requiredRight(int i, double p) const {
        const double u = x[i];
        const double v = x[i + 1];
        const double width = v - u;
        const double eps = std::max(1e-12 * width, 1e-14);
        double q = std::max(f(u), f(v));

        if (u + eps < v) {
            const auto ratio = [&](double z) {
                return (f(z) - p) / (z - u);
            };
            const auto support =
                numerical::adaptiveSupportMaximum(
                    ratio, u + eps, v, 0.0, 8, 3, 3
                );
            if (std::isfinite(support.value))
                q = std::max(q, p + width * support.value);
        }
        return q;
    }

    double requiredLeft(int i, double q) const {
        const double u = x[i];
        const double v = x[i + 1];
        double lo = f(u);
        double hi = fmax;

        if (requiredRight(i, lo) <= q)
            return lo;

        if (requiredRight(i, hi) > q)
            return std::numeric_limits<double>::infinity();

        for (int it = 0; it < 45; ++it) {
            const double mid = (lo + hi) / 2.0;
            if (requiredRight(i, mid) <= q)
                hi = mid;
            else
                lo = mid;
        }
        return hi;
    }
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
    return result;
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

    if (options.maxSweeps < 0 ||
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

    const auto maximum =
        numerical::adaptiveSupportMaximum(
            f, points.front(), points.back(), 0.0, 16, 4, 4
        );
    if (!std::isfinite(maximum.value))
        throw std::runtime_error(
            "Failed to determine a finite feasible height."
        );

    Oracle oracle{f, points, maximum.value};

    std::vector<double> heights(points.size(), maximum.value);
    for (std::size_t i = 0; i < points.size(); ++i)
        heights[i] = std::max(heights[i], f(points[i]));

    for (int sweep = 0; sweep < options.maxSweeps; ++sweep) {
        const double oldValue =
            buildResult(f, points, heights).value;

        for (std::size_t i = 1; i + 1 < points.size(); ++i) {
            const double fromLeft =
                oracle.requiredRight(
                    static_cast<int>(i - 1), heights[i - 1]
                );
            const double fromRight =
                oracle.requiredLeft(
                    static_cast<int>(i), heights[i + 1]
                );

            const double next = std::max({
                f(points[i]), fromLeft, fromRight
            });

            // The old vector is feasible, so the coordinate-wise minimum
            // cannot be above the current height except for numerical noise.
            heights[i] = std::min(heights[i], next);
        }

        heights.front() =
            std::min(
                heights.front(),
                oracle.requiredLeft(0, heights[1])
            );
        heights.back() =
            std::min(
                heights.back(),
                oracle.requiredRight(
                    static_cast<int>(points.size() - 2),
                    heights[points.size() - 2]
                )
            );

        for (std::size_t i = 0; i < heights.size(); ++i)
            heights[i] = std::max(heights[i], f(points[i]));

        const double newValue =
            buildResult(f, points, heights).value;
        const double scale =
            std::max({1.0, std::abs(oldValue), std::abs(newValue)});

        if ((oldValue - newValue) / scale < options.tolerance)
            break;
    }

    return buildResult(f, points, heights);
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
