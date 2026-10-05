#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::numerical {

struct SupportMaximum {
    double x;
    double value;
};

SupportMaximum adaptiveSupportMaximum(
    const Function& f,
    double u,
    double v,
    double beta,
    int initialSamples = 32,
    int maxDepth = 8,
    int refinementCount = 32
);

}
