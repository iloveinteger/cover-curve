#include <cover_curve/cover_curve.hpp>

#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <cmath>
#include <exception>
#include <string>

using emscripten::val;

namespace {

val solveWeb(const std::string& expression, double a, double b, int n) {
    try {
        const auto f = cover_curve::parseExpression(expression);

        // Theory-driven curvature-density initialization plus a uniform seed.
        // Avoid the expensive global grid DP on the web path.
        cover_curve::EnvelopeSQPOptions options;
        options.maxIterations = n <= 3 ? 10 : 8;
        options.seeds = 2;
        options.includeFastGridSeed = false;
        options.useCurvatureSeed = true;
        options.curvatureSamples = 65;
        options.gradientTolerance = 2e-5;
        options.innerOptions.maxSweeps = n <= 3 ? 40 : 30;
        options.innerOptions.tolerance = 1e-8;

        const auto result =
            cover_curve::envelopeSQPSolve(f, a, b, n, options);

        val output = val::object();
        output.set("value", result.value);

        val breakpoints = val::array();
        for (double x : result.breakpoints)
            breakpoints.call<void>("push", x);
        output.set("breakpoints", breakpoints);

        val segments = val::array();
        for (const auto& segment : result.segments) {
            val item = val::object();
            item.set("x0", segment.x0);
            item.set("x1", segment.x1);
            item.set("slope", segment.slope);
            item.set("intercept", segment.intercept);
            item.set("cost", segment.cost);
            item.set("contact", segment.contact);
            segments.call<void>("push", item);
        }
        output.set("segments", segments);
        output.set("error", val::null());
        return output;
    } catch (const std::exception& e) {
        val output = val::object();
        output.set("value", std::nan(""));
        output.set("breakpoints", val::array());
        output.set("segments", val::array());
        output.set("error", std::string(e.what()));
        return output;
    }
}

}

EMSCRIPTEN_BINDINGS(cover_curve_web) {
    emscripten::function("solve", &solveWeb);
}
