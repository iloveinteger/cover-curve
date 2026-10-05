#pragma once

#include <functional>
#include <vector>

namespace cover_curve {

using Function = std::function<double(double)>;

struct Segment {
    double x0;
    double x1;
    double slope;
    double intercept;
    double cost;
    double contact;
};

struct Result {
    double value;
    std::vector<double> breakpoints;
    std::vector<Segment> segments;
};

}
