#pragma once

#include <cover_curve/types.hpp>
#include <vector>

namespace cover_curve {

struct DirectHeightOptions {
    int maxSweeps = 20;
    double tolerance = 1e-8;
};

// Direct fixed-breakpoint height relaxation. This solver deliberately does
// not use breakpoint/grid DP. It minimizes each vertex height subject to the
// two adjacent majorant constraints, starting from a globally feasible height.
Result directHeightSolve(
    const Function& f,
    const std::vector<double>& points,
    const DirectHeightOptions& options = {}
);

Result directHeightSolve(
    const Function& f,
    double a,
    double b,
    int n,
    const DirectHeightOptions& options = {}
);

}
