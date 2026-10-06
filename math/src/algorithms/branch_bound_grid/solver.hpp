#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::algorithms::branch_bound_grid {

Result solve(
    const Function& f,
    double a,
    double b,
    int n,
    double tolerance = 1e-4,
    int initialN = 8,
    int maxN = 24,
    int sampleCount = 9
);

}
