#pragma once

#include <cover_curve/types.hpp>

#include <vector>

namespace cover_curve {

Result fastGridDP(
    const Function& f,
    double a,
    double b,
    int n
);

Result fastGridDPOnGrid(
    const Function& f,
    const std::vector<double>& points,
    int n
);

}
