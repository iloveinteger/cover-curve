#pragma once

#include <cover_curve/function.hpp>

namespace cover_curve {

struct SegmentCost {
    double cost;
    double slope;
    double intercept;
    double contact;
};

SegmentCost oneSegmentCost(
    const Function& f,
    double u,
    double v
);

}
