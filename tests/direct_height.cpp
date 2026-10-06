#include <cover_curve/cover_curve.hpp>

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

namespace {

double lineValue(const cover_curve::Segment& s, double x) {
    return s.slope * x + s.intercept;
}

void checkMajorant(
    const cover_curve::Function& f,
    const cover_curve::Result& r
) {
    for (const auto& s : r.segments) {
        for (int k = 0; k <= 200; ++k) {
            const double x =
                s.x0 + (s.x1 - s.x0) * k / 200.0;
            assert(lineValue(s, x) + 1e-7 >= f(x));
        }
    }
}

}

int main() {
    const cover_curve::Function f =
        [](double x) { return x * x; };

    const std::vector<double> points{0.0, 0.5, 1.0};

    const auto direct =
        cover_curve::directHeightSolve(f, points);

    checkMajorant(f, direct);
    assert(direct.value >= -1e-8);
    assert(std::abs(direct.value - 1.0 / 24.0) < 2e-4);

    const auto dp =
        cover_curve::algorithms::fast_grid_dp::solveFastGridDPOnGrid(
            f, points, 2
        );

    assert(std::abs(direct.value - dp.value) < 2e-4);

    std::cout << "direct height smoke: "
              << direct.value << "\n";
    return 0;
}
