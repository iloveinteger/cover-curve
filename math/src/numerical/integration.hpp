#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::numerical {

double adaptiveIntegral(
    const Function& f,
    double a,
    double b,
    double tolerance = 1e-8,
    int maxDepth = 14
);

}
