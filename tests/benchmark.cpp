#include <cover_curve/cover_curve.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>

namespace {

using Clock = std::chrono::steady_clock;

template <typename Solver>
double measure(
    Solver solver,
    const cover_curve::Function& f,
    double a,
    double b,
    int n,
    int repeats,
    double& value
) {
    // Warm-up.
    value = solver(f, a, b, n).value;

    const auto begin = Clock::now();
    for (int i = 0; i < repeats; ++i)
        value = solver(f, a, b, n).value;
    const auto end = Clock::now();

    return std::chrono::duration<double, std::milli>(end - begin).count()
        / static_cast<double>(repeats);
}

struct Case {
    const char* name;
    cover_curve::Function f;
    double a;
    double b;
    int n;
};

}

int main() {
    constexpr int repeats = 2;
    constexpr double pi = 3.1415926535897932384626433832795;

    const Case cases[] = {
        {
            "sin",
            [](double x) { return std::sin(x); },
            0.0, 2.0 * pi, 3
        },
        {
            "cos",
            [](double x) { return std::cos(x); },
            0.0, 2.0 * pi, 3
        },
        {
            "quartic",
            [](double x) {
                return x * x * x * x - 2.0 * x * x + x;
            },
            -1.0, 1.0, 3
        },
        {
            "sixth",
            [](double x) {
                return x * x * x * x * x * x - 3.0 * x * x * x + x;
            },
            -1.0, 1.0, 3
        },
        {
            "piecewise",
            [](double x) {
                if (x < 0.0)
                    return x * x;
                return -x * x + 1.0;
            },
            -1.0, 1.0, 3
        },
        {
            "wavy",
            [](double x) { return x + std::sin(x); },
            0.0, 3.0, 2
        }
    };

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "case,fast_ms,adaptive_ms,speedup,abs_value_diff\n";

    for (const Case& c : cases) {
        double fastValue = 0.0;
        double adaptiveValue = 0.0;

        const double fastMs = measure(
            [](const auto& f, double a, double b, int n) {
                return cover_curve::fastGridDP(f, a, b, n);
            },
            c.f, c.a, c.b, c.n, repeats, fastValue
        );

        const double adaptiveMs = measure(
            [](const auto& f, double a, double b, int n) {
                return cover_curve::adaptiveGridDP(f, a, b, n);
            },
            c.f, c.a, c.b, c.n, repeats, adaptiveValue
        );

        const double speedup =
            adaptiveMs > 0.0 ? adaptiveMs / fastMs : 0.0;

        std::cout
            << c.name << ','
            << fastMs << ','
            << adaptiveMs << ','
            << speedup << ','
            << std::abs(fastValue - adaptiveValue)
            << '\n';
    }

    return 0;
}
