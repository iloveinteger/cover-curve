#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve {

struct CoordinateSearchOptions {
    int maxSweeps = 2;
    int samples = 5;
    int refinements = 1;
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
