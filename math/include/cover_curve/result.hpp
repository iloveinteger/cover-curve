#pragma once

#include <vector>

namespace cover_curve {

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
