#include <cover_curve/cover_curve.hpp>
#include "../math/src/algorithms/fast_grid_dp/grid_dp.hpp"

#include <algorithm>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <vector>

static void check(
    const cover_curve::Function& f,
    const std::vector<double>& p,
    double tol,
    double expected = std::numeric_limits<double>::quiet_NaN()
) {
    cover_curve::DirectHeightOptions opt;
    opt.maxSweeps = 8;
    opt.tolerance = 1e-9;

    const auto d = cover_curve::directHeightSolve(f, p, opt);
    const auto g =
        cover_curve::algorithms::fast_grid_dp::solveFastGridDPOnGrid(
            f, p, static_cast<int>(p.size()) - 1
        );

    const double scale = std::max({
        1.0, std::abs(d.value), std::abs(g.value)
    });

    if (std::abs(d.value - g.value) > tol * scale)
        throw std::runtime_error("direct-height mismatch");

    if (std::isfinite(expected) &&
        std::abs(d.value - expected) > tol) {
        throw std::runtime_error("direct-height exact-value mismatch");
    }

    for (const auto& s : d.segments) {
        for (int k = 0; k <= 200; ++k) {
            const double x =
                s.x0 + (s.x1 - s.x0) * k / 200.0;
            if (s.slope * x + s.intercept + 2e-5 < f(x))
                throw std::runtime_error("majorant violation");
        }
    }
}

int main() {
    check(
        [](double x) { return x * x; },
        {0.0, 0.5, 1.0},
        3e-3
    );

    check(
        [](double x) { return x * x * x * x; },
        {0.0, 0.25, 0.6, 1.0},
        3e-3
    );

    const std::vector<double> exactGrid{0.0, 0.25, 0.6, 1.0};

    check(
        [](double x) { return 1.7 * x - 0.35; },
        exactGrid,
        1e-8,
        0.0
    );

    check(
        [](double x) {
            if (x <= 0.25) return 2.0 * x + 0.1;
            if (x <= 0.6) return -0.5 * x + 0.725;
            return 1.25 * x - 0.325;
        },
        exactGrid,
        1e-8,
        0.0
    );

    check(
        [](double x) { return x * x; },
        {0.0, 1.0},
        1e-8,
        1.0 / 6.0
    );

    std::cout << "direct height comparison: PASS\n";
}
