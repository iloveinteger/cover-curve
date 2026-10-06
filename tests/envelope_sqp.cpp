#include <cover_curve/solvers/envelope_sqp.hpp>
#include <cover_curve/solvers/direct_height.hpp>
#include <cover_curve/solvers/coordinate_search.hpp>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <chrono>
#include <stdexcept>
#include <vector>

namespace {

double xSquared(double x) {
    return x * x;
}

double sine(double x) {
    return std::sin(x);
}

void require(bool condition, const char* message) {
    if (!condition)
        throw std::runtime_error(message);
}

double denseViolation(
    const cover_curve::Result& result,
    const cover_curve::Function& f,
    int samples = 2001
) {
    double worst = 0.0;
    for (int k = 0; k < samples; ++k) {
        const double a = result.breakpoints.front();
        const double b = result.breakpoints.back();
        const double x = a + (b - a) * k / (samples - 1.0);

        auto it = std::upper_bound(
            result.breakpoints.begin(),
            result.breakpoints.end(),
            x
        );
        std::size_t i =
            it == result.breakpoints.begin()
                ? 0
                : static_cast<std::size_t>(
                    std::distance(result.breakpoints.begin(), it) - 1
                );
        if (i >= result.segments.size())
            i = result.segments.size() - 1;

        const auto& s = result.segments[i];
        const double g = s.slope * x + s.intercept;
        worst = std::max(worst, f(x) - g);
    }
    return worst;
}

}

int main() {
    using namespace cover_curve;

    try {
        EnvelopeSQPOptions options;
        options.maxIterations = 20;
        options.seeds = 4;
        options.includeFastGridSeed = false;
        options.gradientTolerance = 1e-5;
        options.innerOptions.maxSweeps = 40;
        options.innerOptions.tolerance = 1e-9;

        const Result x2 =
            envelopeSQPSolve(xSquared, 0.0, 1.0, 2, options);

        require(x2.breakpoints.size() == 3,
                "x^2 returned invalid breakpoint count.");
        require(std::abs(x2.breakpoints[1] - 0.5) < 5e-3,
                "x^2 breakpoint did not converge to 0.5.");
        require(
            std::abs(x2.value - 1.0 / 24.0) < 5e-4,
            "x^2 objective disagrees with 1/(6n^2)."
        );
        require(
            denseViolation(x2, xSquared) <= 2e-5,
            "x^2 envelope is not a majorant."
        );

        const double pi = std::acos(-1.0);
        const std::vector<double> fixed = {
            0.0, pi, 2.0 * pi
        };
        const auto detailed =
            directHeightSolveDetailed(
                sine, fixed, options.innerOptions
            );
        require(!detailed.contacts.empty(),
                "dual contact set is unexpectedly empty.");
        const auto t0 = std::chrono::steady_clock::now();
        // Validate the analytic envelope derivative against a finite
        // difference of the complete fixed-breakpoint value function.
        EnvelopeSQPOptions gradientOptions = options;
        gradientOptions.maxIterations = 1;
        gradientOptions.lineSearchSteps = 1;
        gradientOptions.sufficientDecrease = 1e6;
        gradientOptions.gradientTolerance = 1e-12;

        const auto gd = envelopeSQPSolveDetailed(
            sine,
            0.0,
            2.0 * std::acos(-1.0),
            2,
            gradientOptions
        );

        const double h = 1e-2;
        const auto vp = directHeightSolve(
            sine,
            std::vector<double>{0.0, pi + h, 2.0 * pi},
            options.innerOptions
        ).value;
        const auto vm = directHeightSolve(
            sine,
            std::vector<double>{0.0, pi - h, 2.0 * pi},
            options.innerOptions
        ).value;
        const double fd = (vp - vm) / (2.0 * h);
        require(gd.gradient.size() == 3,
                "envelope gradient has invalid dimension.");
        if (std::abs(gd.gradient[1] - fd) >= 1e-2) {
            std::cerr
                << "gradient=" << gd.gradient[1]
                << " finite_difference=" << fd << std::endl;
            throw std::runtime_error(
                "envelope gradient disagrees with fixed-breakpoint finite difference."
            );
        }

        const Result sin =
            envelopeSQPSolve(
                sine,
                0.0,
                2.0 * std::acos(-1.0),
                2,
                options
            );
        const auto t1 = std::chrono::steady_clock::now();
        const double envelopeMs =
            std::chrono::duration<double, std::milli>(t1 - t0).count();

        CoordinateSearchOptions coordinateOptions;
        coordinateOptions.maxSweeps = 1;
        coordinateOptions.samples = 3;
        coordinateOptions.refinements = 0;
        coordinateOptions.useDirectHeightOracle = true;
        const auto t2 = std::chrono::steady_clock::now();
        const Result coordinate =
            coordinateSearch(
                sine,
                0.0,
                2.0 * std::acos(-1.0),
                2,
                coordinateOptions
            );
        const auto t3 = std::chrono::steady_clock::now();
        const double coordinateMs =
            std::chrono::duration<double, std::milli>(t3 - t2).count();
        require(
            denseViolation(sin, sine) <= 2e-5,
            "sin envelope is not a majorant."
        );

        std::cout
            << "envelope SQP: PASS"
            << " x2_value=" << x2.value
            << " x2_mid=" << x2.breakpoints[1]
            << " sin_value=" << sin.value
            << " envelope_ms=" << envelopeMs
            << " coordinate_value=" << coordinate.value
            << " coordinate_ms=" << coordinateMs
            << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "envelope SQP: FAIL: "
                  << e.what() << std::endl;
        return 1;
    }
}
