#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve::algorithms::adaptive_grid_dp {

Segment oneSegmentCost(
    const Function& f,
    double u,
    double v
);

}
