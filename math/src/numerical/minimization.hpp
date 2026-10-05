#pragma once

#include <functional>

namespace cover_curve::numerical {

struct Interval {
    double lo;
    double hi;
};

Interval bracketMinimum(
    const std::function<double(double)>& objective,
    double initial = 0.0,
    double initialStep = 1.0,
    double growth = 2.0,
    int maxIterations = 32
);

double goldenSectionMinimum(
    const std::function<double(double)>& objective,
    double lo,
    double hi,
    double tolerance = 1e-8,
    int maxIterations = 50
);

}
