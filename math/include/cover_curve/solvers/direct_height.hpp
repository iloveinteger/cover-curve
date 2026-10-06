#pragma once

#include <cover_curve/types.hpp>
#include <vector>

namespace cover_curve {

struct DirectHeightOptions {
    // Each cutting-plane round solves one LP and then separates every
    // segment. maxSweeps bounds the outer-round budget up to the
    // implementation's internal safety multiplier.
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

// Solve the fixed-breakpoint continuous-height problem numerically.
//
// The method starts from finitely many constraints, solves the resulting LP,
// searches each segment for a violated continuous constraint, adds violating
// contacts, and repeats. Feasibility is guaranteed only to the configured
// separation tolerance unless the separation search is independently
// certified.
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
