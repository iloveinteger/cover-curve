#include "solver.hpp"

#include "grid_dp.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>
#include <utility>

namespace cover_curve::algorithms::fast_grid_dp {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance,
    int initialN,
    int maxN
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (!std::isfinite(tolerance) || tolerance <= 0.0)
        throw std::invalid_argument("tolerance must be positive.");

    if (initialN < n)
        initialN = n;

    if (maxN < initialN)
        throw std::invalid_argument(
            "maxN must satisfy maxN >= initialN."
        );

    int N = initialN;
    Result previous{};
    bool hasPrevious = false;

    while (true) {
        Result current = solveFastGridDP(f, a, b, n, N);

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

Result fastGridDP(
    const Function& f,
    double a,
    double b,
    int n
) {
    return algorithms::fast_grid_dp::solve(
        f, a, b, n
    );
}

}
