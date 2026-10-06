#include "solver.hpp"

#include "curvature_grid.hpp"
#include "grid_dp.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace cover_curve::algorithms::curvature_adaptive {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance,
    int initialN,
    int maxN,
    int curvatureSamples
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (!std::isfinite(a) || !std::isfinite(b) || a >= b)
        throw std::invalid_argument("Require finite a < b.");

    if (n < 1)
        throw std::invalid_argument("n must be positive.");

    if (!std::isfinite(tolerance) || tolerance <= 0.0)
        throw std::invalid_argument("tolerance must be positive.");

    if (initialN < n)
        initialN = n;

    if (maxN < initialN)
        throw std::invalid_argument(
            "maxN must satisfy maxN >= initialN."
        );

    if (curvatureSamples < 3)
        throw std::invalid_argument(
            "curvatureSamples must be at least 3."
        );

    int N = initialN;
    Result previous{};
    bool hasPrevious = false;

    while (true) {
        const auto grid =
            makeCurvatureGrid(
                f, a, b, N, curvatureSamples
            );

        Result current = solveGrid(f, grid, n);

        if (hasPrevious) {
            const double relativeChange =
                std::abs(current.value - previous.value) /
                std::max({
                    1.0,
                    std::abs(current.value),
                    std::abs(previous.value)
                });

            if (relativeChange < tolerance)
                return current;
        }

        if (N >= maxN)
            return current;

        previous = std::move(current);
        hasPrevious = true;
        N = std::min(2 * N, maxN);
    }
}

}

namespace cover_curve {

Result curvatureAdaptive(
    const Function& f,
    double a,
    double b,
    int n
) {
    return algorithms::curvature_adaptive::solve(
        f, a, b, n
    );
}

}
