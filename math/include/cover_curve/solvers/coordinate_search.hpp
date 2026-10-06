#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve {

struct CoordinateSearchOptions {
    int maxSweeps = 4;
    int samples = 7;
    int refinements = 3;
    double tolerance = 1e-7;
};

Result coordinateSearch(
    const Function& f,
    double a,
    double b,
    int n,
    const CoordinateSearchOptions& options = {}
);

}
