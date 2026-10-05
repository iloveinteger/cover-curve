#pragma once

#include <cover_curve/cover_curve.hpp>

namespace cover_curve::numerical {

double adaptiveIntegral(
    const Function& f,
    double a,
    double b,
    double tolerance = 1e-8,
    int maxDepth = 14
);

}
