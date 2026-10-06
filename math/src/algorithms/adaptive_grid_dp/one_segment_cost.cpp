#include "one_segment_cost.hpp"

#include "../../numerical/integration.hpp"
#include "../../numerical/minimization.hpp"
#include "../../numerical/support_max.hpp"

#include <algorithm>
#include <cmath>
#include <functional>

namespace cover_curve::algorithms::adaptive_grid_dp {

Segment oneSegmentCost(
    const Function& f,
    double u,
    double v
) {
    if (v <= u) {
        const double y = f(u);

        return {
            u,
            v,
            0.0,
            y,
            0.0,
            u
        };
    }

    const double integral =
        numerical::adaptiveIntegral(
            f,
            u,
            v
        );

    const auto objective =
        [&](double beta) {
            const auto support =
                numerical::adaptiveSupportMaximum(
                    f,
                    u,
                    v,
                    beta
                );

            return
                (v - u) * support.value
                +
                beta * (v * v - u * u) / 2.0
                -
                integral;
        };

    const double initialSlope =
        (f(v) - f(u)) / (v - u);

    const auto bracket =
        numerical::bracketMinimum(
            objective,
            initialSlope
        );

    const double slope =
        numerical::goldenSectionMinimum(
            objective,
            bracket.lo,
            bracket.hi
        );

    const auto support =
        numerical::adaptiveSupportMaximum(
            f,
            u,
            v,
            slope
        );

    // Reuse the final support evaluation.
    const double cost = std::max(
        0.0,
        (v - u) * support.value
        + slope * (v * v - u * u) / 2.0
        - integral
    );

    return {
        u,
        v,
        slope,
        support.value,
        cost,
        support.x
    };
}

}
