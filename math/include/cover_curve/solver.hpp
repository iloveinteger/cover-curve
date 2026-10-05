#pragma once

#include <cover_curve/function.hpp>
#include <cover_curve/result.hpp>

namespace cover_curve {

Result solve(
    const Function& f,
    double a,
    double b,
    int n
);

}
