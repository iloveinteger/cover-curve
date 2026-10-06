#pragma once

#include <cover_curve/types.hpp>

#include <vector>

namespace cover_curve::algorithms::curvature_adaptive {

Result solveGrid(
    const Function& f,
    const std::vector<double>& points,
    int n
);

}
