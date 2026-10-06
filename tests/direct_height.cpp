#include <cover_curve/cover_curve.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>

static void check(
    const cover_curve::Function& f,
    double expected,
    double tolerance
) {
    cover_curve::DirectHeightOptions opt;
    opt.maxSweeps = 8;
    opt.tolerance = 1e-9;

    const auto r = cover_curve::directHeightSolve(f, 0.0, 1.0, 1, opt);
    if (std::abs(r.value - expected) > tolerance)
        throw std::runtime_error("direct-height exact-value mismatch");

    for (const auto& s : r.segments) {
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
        1.0 / 6.0,
        1e-7
    );

    // Affine functions are represented exactly.
    check(
        [](double x) { return 1.7 * x - 0.35; },
        0.0,
        1e-8
    );

    std::cout << "direct height correctness: PASS\n";
}
