#pragma once

#include <cover_curve/types.hpp>

#include <vector>

namespace cover_curve::algorithms::curvature_adaptive {

std::vector<double> makeCurvatureGrid(
    const Function& f,
    double a,
    double b,
    int N,
    int samples
);

}
