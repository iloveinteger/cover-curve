#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::algorithms::fast_grid_dp {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance = 1e-6,
    int initialN = 8,
    int maxN = 32
);

}
