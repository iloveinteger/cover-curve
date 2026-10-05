#pragma once

#include <cover_curve/interpolation.hpp>

namespace cover_curve::interpolation {

Function naturalCubicSpline(
    const DataPoints& points
);

}
