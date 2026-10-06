#include <cover_curve/cover_curve.hpp>
#include <cmath>
#include <iostream>
#include <stdexcept>

int main() {
    const auto r = cover_curve::directHeightSolve(
        [](double x) { return x * x; }, 0.0, 1.0, 1
    );
    if (std::abs(r.value - 1.0 / 6.0) > 1e-7)
        throw std::runtime_error("wrong x^2 one-segment optimum");
    std::cout << "direct height smoke: PASS value=" << r.value << "\n";
}
