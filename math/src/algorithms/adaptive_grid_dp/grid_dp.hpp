#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::algorithms::adaptive_grid_dp {

Result solveGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N,
    int heightLevels = 64
);

}
