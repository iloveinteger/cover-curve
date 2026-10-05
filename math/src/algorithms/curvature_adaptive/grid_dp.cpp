#include "grid_dp.hpp"

#include "../adaptive_grid_dp/grid_dp.hpp"

#include <stdexcept>

namespace cover_curve::algorithms::curvature_adaptive {

Result solveGrid(
    const Function& f,
    const std::vector<double>& points,
    int n,
    int heightLevels
) {
    return adaptive_grid_dp::solveGridDPOnGrid(
        f,
        points,
        n,
        heightLevels
    );
}

}
