#pragma once

#include <cover_curve/types.hpp>

#include <vector>

namespace cover_curve::algorithms::adaptive_grid_dp {

Result solveGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N
);

Result solveGridDPOnGrid(
    const Function& f,
    const std::vector<double>& points,
    int n
);

}
