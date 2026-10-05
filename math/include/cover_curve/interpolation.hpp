#pragma once

#include <cover_curve/types.hpp>

#include <utility>
#include <vector>

namespace cover_curve {

using DataPoint = std::pair<double, double>;
using DataPoints = std::vector<DataPoint>;

Function linearInterpolation(
    const DataPoints& points
);

Function naturalCubicSpline(
    const DataPoints& points
);

}
