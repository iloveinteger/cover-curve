#pragma once

#include <cover_curve/types.hpp>
#include <vector>

namespace cover_curve {

struct DirectHeightOptions {
    // Maximum cutting-plane rounds are maxSweeps * number of heights.
    int maxSweeps = 100;
    double tolerance = 1e-8;
};

struct DirectHeightContact {
    int segment = -1;
    double x = 0.0;
    double multiplier = 0.0;
};

struct DirectHeightDetailedResult {
    Result result;
    std::vector<double> heights;
    std::vector<DirectHeightContact> contacts;
};

// Solve the fixed-breakpoint continuous-height problem without breakpoint
// DP. The implementation uses a cutting-plane LP: solve a finite LP,
// separate the resulting piecewise-linear majorant against f on every
// segment, add violated contact constraints, and repeat.
Result directHeightSolve(
    const Function& f,
    const std::vector<double>& points,
    const DirectHeightOptions& options = {}
);

DirectHeightDetailedResult directHeightSolveDetailed(
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
