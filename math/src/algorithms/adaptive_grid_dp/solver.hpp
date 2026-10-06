#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::algorithms::adaptive_grid_dp {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance = 1e-6,
    int initialN = 16,
    int maxN = 64
);

}
