#pragma once

#include <vector>
#include <utility>

#include <cover_curve/function.hpp>

namespace cover_curve {

using DataPoint = std::pair<double, double>;
using DataPoints = std::vector<DataPoint>;

Function linearInterpolation(const DataPoints& points);

Function naturalCubicSpline(const DataPoints& points);

}
