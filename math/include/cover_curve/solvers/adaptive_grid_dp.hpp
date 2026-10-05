#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve {

Result adaptiveGridDP(
    const Function& f,
    double a,
    double b,
    int n
);

}
