#include "grid_dp.hpp"

#include "../adaptive_grid_dp/grid_dp.hpp"

namespace cover_curve::algorithms::curvature_adaptive {

Result solveGrid(
    const Function& f,
    const std::vector<double>& points,
    int n
) {
    return adaptive_grid_dp::solveGridDPOnGrid(
        f, points, n
    );
}

}
