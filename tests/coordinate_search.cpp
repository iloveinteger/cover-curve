#include <cover_curve/cover_curve.hpp>

#include <cmath>
#include <iostream>
#include <stdexcept>

int main() {
    const auto f = [](double x) { return x * x; };

    for (int n = 1; n <= 4; ++n) {
        const auto r = cover_curve::coordinateSearch(f, 0.0, 1.0, n);
        const double expected = 1.0 / (6.0 * n * n);
        if (std::abs(r.value - expected) > 2e-3 * expected + 2e-5)
            throw std::runtime_error("coordinate search missed known x^2 optimum");

        if (r.breakpoints.size() != static_cast<std::size_t>(n + 1))
            throw std::runtime_error("invalid breakpoint count");

        for (int i = 0; i <= n; ++i) {
            const double expectedX = static_cast<double>(i) / n;
            if (std::abs(r.breakpoints[i] - expectedX) > 0.08)
                throw std::runtime_error("x^2 breakpoints are not near uniform");
        }
    }

    std::cout << "coordinate search x^2 optimum: PASS\n";
}
