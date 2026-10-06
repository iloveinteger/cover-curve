#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve {

struct BreakpointSearchOptions {
    int maxDepth = 8;
    int maxEvaluations = 128;
};

Result breakpointSearch(
    const Function& f,
    double a,
    double b,
    int n,
    const BreakpointSearchOptions& options = {}
);

}
