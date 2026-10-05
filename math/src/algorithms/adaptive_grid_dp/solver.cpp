#include "solver.hpp"

#include "grid_dp.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace cover_curve::algorithms::adaptive_grid_dp {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance,
    int initialN,
    int maxN,
    int initialHeightLevels
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

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

    if (initialHeightLevels < 2)
        throw std::invalid_argument(
            "initialHeightLevels must be at least 2."
        );

    int N = initialN;
    int heightLevels = initialHeightLevels;

    Result previous{};
    bool hasPrevious = false;

    while (true) {
        Result current =
            solveGridDP(
                f,
                a,
                b,
                n,
                N,
                heightLevels
            );

        if (hasPrevious) {
            const double relativeChange =
                std::abs(
                    current.value - previous.value
                )
                /
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

        // Doubling the number of height samples halves the height-grid
        // spacing up to the change in the finite height bound.
        heightLevels =
            std::min(
                2 * heightLevels - 1,
                4097
            );
    }
}

}

namespace cover_curve {

Result adaptiveGridDP(
    const Function& f,
    double a,
    double b,
    int n
) {
    return algorithms::adaptive_grid_dp::solve(
        f,
        a,
        b,
        n
    );
}

}
