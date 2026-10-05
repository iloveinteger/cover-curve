#include <cover_curve/cover_curve.hpp>

#include <cassert>
#include <cmath>
#include <iostream>

namespace {
void expectNear(double actual, double expected, double tol, const char* name) {
    if (std::abs(actual - expected) > tol) {
        std::cerr << name << ": expected " << expected << ", got " << actual << "\n";
        std::exit(1);
    }
}
}

int main() {
    using cover_curve::Function;

    const Function zero = [](double) { return 0.0; };
    const auto r0 = cover_curve::adaptiveGridDP(zero, 0.0, 1.0, 2);
    expectNear(r0.value, 0.0, 1e-8, "zero");

    const Function linear = [](double x) { return 2.0 * x + 3.0; };
    const auto r1 = cover_curve::adaptiveGridDP(linear, 0.0, 1.0, 3);
    expectNear(r1.value, 0.0, 1e-7, "linear");

    const Function square = [](double x) { return x * x; };
    const auto r2 = cover_curve::adaptiveGridDP(square, 0.0, 1.0, 1);
    expectNear(r2.value, 1.0 / 6.0, 2e-3, "square n=1");

    const auto r3 = cover_curve::adaptiveGridDP(square, 0.0, 1.0, 2);
    expectNear(r3.value, 1.0 / 24.0, 2e-3, "square n=2");

    if (r3.breakpoints.size() != 3 || r3.segments.size() != 2) {
        std::cerr << "unexpected result structure\n";
        return 1;
    }

    std::cout << "All smoke tests passed.\n";
    return 0;
}

    const auto r4 = cover_curve::curvatureAdaptive(square, 0.0, 1.0, 2);
    expectNear(r4.value, 1.0 / 24.0, 3e-3, "curvature square n=2");
