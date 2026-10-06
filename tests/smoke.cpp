#include <cover_curve/cover_curve.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>

namespace {

void expectNear(
    double actual,
    double expected,
    double tol,
    const char* name
) {
    if (std::abs(actual - expected) > tol) {
        std::cerr << name
                  << ": expected " << expected
                  << ", got " << actual << "\n";
        std::exit(1);
    }
}

void expectContinuous(
    const cover_curve::Result& result,
    double tol,
    const char* name
) {
    for (std::size_t i = 1; i < result.segments.size(); ++i) {
        const auto& left = result.segments[i - 1];
        const auto& right = result.segments[i];
        const double x = result.breakpoints[i];
        const double gap =
            std::abs(
                (left.slope * x + left.intercept)
                - (right.slope * x + right.intercept)
            );

        if (gap > tol) {
            std::cerr << name
                      << ": discontinuity " << gap << "\n";
            std::exit(1);
        }
    }
}

}

int main() {
    using cover_curve::Function;

    const Function zero = [](double) { return 0.0; };
    const auto r0 =
        cover_curve::adaptiveGridDP(zero, 0.0, 1.0, 2);
    expectNear(r0.value, 0.0, 1e-8, "zero");

    const Function linear =
        [](double x) { return 2.0 * x + 3.0; };
    const auto r1 =
        cover_curve::adaptiveGridDP(linear, 0.0, 1.0, 3);
    expectNear(r1.value, 0.0, 1e-7, "linear");

    const Function square =
        [](double x) { return x * x; };
    const auto r2 =
        cover_curve::adaptiveGridDP(square, 0.0, 1.0, 1);
    expectNear(r2.value, 1.0 / 6.0, 2e-3, "square n=1");

    const auto r3 =
        cover_curve::adaptiveGridDP(square, 0.0, 1.0, 2);
    const auto rf =
        cover_curve::fastGridDP(square, 0.0, 1.0, 2);
    expectNear(r3.value, 1.0 / 24.0, 2e-3, "square n=2");
    expectNear(rf.value, r3.value, 1e-10, "fast/adaptive agreement");

    if (r3.breakpoints.size() != 3 ||
        r3.segments.size() != 2) {
        std::cerr << "unexpected result structure\n";
        return 1;
    }
    expectContinuous(r3, 1e-10, "square");
    expectContinuous(rf, 1e-10, "fast square");

    // The new solver is a direct non-DP search over the internal breakpoint.
    // Its first sample is x=1/2, which is already the exact optimum for x^2
    // with two segments.
    cover_curve::BreakpointSearchOptions searchOptions;
    searchOptions.maxDepth = 4;
    searchOptions.maxEvaluations = 32;

    const auto rSearch =
        cover_curve::breakpointSearch(
            square,
            0.0,
            1.0,
            2,
            searchOptions
        );
    expectNear(
        rSearch.value,
        1.0 / 24.0,
        2e-3,
        "breakpoint search square n=2"
    );
    expectContinuous(
        rSearch,
        1e-10,
        "breakpoint search square"
    );

    const auto rSearchSmall =
        cover_curve::breakpointSearch(
            square,
            0.0,
            1.0,
            2,
            cover_curve::BreakpointSearchOptions{
                2,
                8
            }
        );
    if (rSearch.value > rSearchSmall.value + 1e-12) {
        std::cerr << "breakpoint search lost incumbent when budget increased\n";
        return 1;
    }

    const Function wavy =
        [](double x) { return x + std::sin(x); };
    const auto rw =
        cover_curve::adaptiveGridDP(wavy, 0.0, 4.0, 2);
    expectContinuous(rw, 1e-10, "wavy");

    // Trigonometric and higher-order polynomial regression cases.
    constexpr double pi = 3.1415926535897932384626433832795;
    const Function sine =
        [](double x) { return std::sin(x); };
    const auto rsFast =
        cover_curve::fastGridDP(sine, 0.0, 2.0 * pi, 3);
    const auto rsReference =
        cover_curve::adaptiveGridDP(sine, 0.0, 2.0 * pi, 3);
    expectNear(rsFast.value, rsReference.value, 8e-5, "fast sine agreement");
    expectContinuous(rsFast, 1e-10, "fast sine");

    const Function cosine =
        [](double x) { return std::cos(x); };
    const auto rcFast =
        cover_curve::fastGridDP(cosine, 0.0, 2.0 * pi, 3);
    const auto rcReference =
        cover_curve::adaptiveGridDP(cosine, 0.0, 2.0 * pi, 3);
    expectNear(rcFast.value, rcReference.value, 8e-5, "fast cosine agreement");
    expectContinuous(rcFast, 1e-10, "fast cosine");

    const Function quartic =
        [](double x) { return x * x * x * x - 2.0 * x * x + x; };
    const auto rqFast =
        cover_curve::fastGridDP(quartic, -1.0, 1.0, 3);
    const auto rqReference =
        cover_curve::adaptiveGridDP(quartic, -1.0, 1.0, 3);
    expectNear(rqFast.value, rqReference.value, 8e-5, "fast quartic agreement");
    expectContinuous(rqFast, 1e-10, "fast quartic");

    const Function sixth =
        [](double x) { return x * x * x * x * x * x - 3.0 * x * x * x + x; };
    const auto r6Fast =
        cover_curve::fastGridDP(sixth, -1.0, 1.0, 3);
    const auto r6Reference =
        cover_curve::adaptiveGridDP(sixth, -1.0, 1.0, 3);
    expectNear(r6Fast.value, r6Reference.value, 1e-4, "fast sixth agreement");
    expectContinuous(r6Fast, 1e-10, "fast sixth");

    const auto rwFast =
        cover_curve::fastGridDP(wavy, 0.0, 3.0, 2);
    expectContinuous(rwFast, 1e-10, "fast wavy");
    const auto rwReference =
        cover_curve::adaptiveGridDP(wavy, 0.0, 3.0, 2);
    expectNear(
        rwFast.value,
        rwReference.value,
        5e-5,
        "fast wavy agreement"
    );

    const Function piecewise =
        [](double x) {
            if (x < 0.0)
                return x * x;
            return -x * x + 1.0;
        };
    const auto rpFast =
        cover_curve::fastGridDP(piecewise, -1.0, 1.0, 3);
    const auto rpReference =
        cover_curve::adaptiveGridDP(piecewise, -1.0, 1.0, 3);
    expectNear(
        rpFast.value,
        rpReference.value,
        1e-4,
        "fast piecewise curvature agreement"
    );
    expectContinuous(rpFast, 1e-10, "fast piecewise curvature");

    cover_curve::CoordinateSearchOptions coordinateOptions;
    coordinateOptions.maxSweeps = 1;
    coordinateOptions.samples = 3;
    coordinateOptions.refinements = 0;

    const auto rCoordinate =
        cover_curve::coordinateSearch(
            square,
            0.0,
            1.0,
            2,
            coordinateOptions
        );
    expectNear(
        rCoordinate.value,
        1.0 / 24.0,
        2e-3,
        "coordinate search square n=2"
    );
    expectContinuous(
        rCoordinate,
        1e-10,
        "coordinate search square"
    );

    if (rCoordinate.value > rf.value + 1e-10) {
        std::cerr << "coordinate search degraded fastGridDP result\n";
        return 1;
    }

    const auto r4 =
        cover_curve::curvatureAdaptive(square, 0.0, 1.0, 2);
    expectNear(
        r4.value,
        1.0 / 24.0,
        3e-3,
        "curvature square n=2"
    );
    expectContinuous(r4, 1e-10, "curvature square");

    std::cout << "All smoke tests passed.\n";
    return 0;
}
