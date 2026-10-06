#pragma once

#include <cover_curve/types.hpp>
#include <vector>

namespace cover_curve::algorithms::fast_grid_dp {

Result solveFastGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N
);

Result solveFastGridDPOnGrid(
    const Function& f,
    const std::vector<double>& points,
    int n
);

}
