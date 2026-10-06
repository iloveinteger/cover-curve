#include <cover_curve/cover_curve.hpp>

#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <cmath>
#include <exception>
#include <stdexcept>
#include <string>

using emscripten::val;

namespace {

val solveWeb(const std::string& expression, double a, double b, int n) {
    try {
        const auto f = cover_curve::parseExpression(expression);
        const auto baseline = cover_curve::fastGridDP(f, a, b, n);

        // Refine the DP solution by optimizing breakpoints one at a time.
        // Coordinate search starts from the baseline and only accepts
        // improvements, so it cannot worsen the returned objective.
        if (n <= 4) {
            const auto refined =
                cover_curve::coordinateSearch(f, a, b, n);

            const auto& result =
                refined.value < baseline.value ? refined : baseline;

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
        }

        const auto& result = baseline;

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
