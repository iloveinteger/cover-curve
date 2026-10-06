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
    opt.maxSweeps = 10;
    opt.tolerance = 1e-9;

    const auto d = cover_curve::directHeightSolve(f, p, opt);
    const auto g =
        cover_curve::algorithms::fast_grid_dp::solveFastGridDPOnGrid(
            f, p, static_cast<int>(p.size()) - 1
        );

    const double err = std::abs(d.value - g.value);
    const double scale = std::max({
        1.0, std::abs(d.value), std::abs(g.value)
    });

    if (err > tol * scale)
        throw std::runtime_error("direct-height mismatch");

    if (std::isfinite(expected) &&
        std::abs(d.value - expected) > tol) {
        throw std::runtime_error("direct-height exact-value mismatch");
    }

    for (const auto& s : d.segments) {
        for (int k = 0; k <= 400; ++k) {
            const double x =
                s.x0 + (s.x1 - s.x0) * k / 400.0;
            const double gx = s.slope * x + s.intercept;
            if (gx + 2e-5 < f(x))
                throw std::runtime_error("majorant violation");
        }
    }
}

int main() {
    const std::vector<std::vector<double>> grids = {
        {0.0, 0.5, 1.0},
        {0.0, 0.25, 0.6, 1.0},
        {0.0, 0.1, 0.3, 0.7, 1.0}
    };

    const std::vector<cover_curve::Function> fs = {
        [](double x) { return x * x; },
        [](double x) { return x * x * x * x; },
        [](double x) { return std::sin(x); },
        [](double x) { return std::exp(0.4 * x); }
    };

    for (const auto& f : fs)
        for (const auto& p : grids)
            check(f, p, 3e-3);

    const std::vector<double> exactGrid{0.0, 0.25, 0.6, 1.0};

    check(
        [](double x) { return 1.7 * x - 0.35; },
        exactGrid,
        1e-8,
        0.0
    );

    check(
        [](double x) {
            if (x <= 0.25)
                return 2.0 * x + 0.1;
            if (x <= 0.6)
                return -0.5 * x + 0.725;
            return 1.25 * x - 0.325;
        },
        exactGrid,
        1e-8,
        0.0
    );

    // A convex x^2 segment has the exact chord majorant, so the
    // one-segment fixed-breakpoint optimum is exactly 1/6.
    check(
        [](double x) { return x * x; },
        {0.0, 1.0},
        1e-8,
        1.0 / 6.0
    );

    std::cout << "direct height comparison: PASS\n";
    return 0;
}
