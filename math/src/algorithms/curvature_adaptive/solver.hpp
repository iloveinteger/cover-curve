#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::algorithms::curvature_adaptive {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance = 1e-6,
    int initialN = 32,
    int maxN = 1024,
    int initialHeightLevels = 32,
    int curvatureSamples = 128
);

}
