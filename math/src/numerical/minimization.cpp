#include "minimization.hpp"

#include <cmath>

namespace cover_curve::numerical {

Interval bracketMinimum(
    const std::function<double(double)>& objective,
    double initial,
    double initialStep,
    double growth,
    int maxIterations
) {
    const double centerValue = objective(initial);

    double left = initial - initialStep;
    double right = initial + initialStep;

    const double leftValue = objective(left);
    const double rightValue = objective(right);

    if (
        leftValue >= centerValue &&
        rightValue >= centerValue
    ) {
        return {left, right};
    }

    const int direction =
        rightValue < leftValue ? 1 : -1;

    double previous = initial;
    double current =
        direction == 1 ? right : left;

    double currentValue =
        direction == 1
            ? rightValue
            : leftValue;

    double step = initialStep;

    for (int i = 0; i < maxIterations; ++i) {
        step *= growth;

        const double next =
            current + direction * step;

        const double nextValue =
            objective(next);

        if (nextValue >= currentValue) {
            if (direction == 1) {
                return {previous, next};
            }

            return {next, previous};
        }

        previous = current;
        current = next;
        currentValue = nextValue;
    }

    if (direction == 1) {
        return {previous, current};
    }

    return {current, previous};
}

double goldenSectionMinimum(
    const std::function<double(double)>& objective,
    double lo,
    double hi,
    double tolerance,
    int maxIterations
) {
    const double phi =
        (1.0 + std::sqrt(5.0)) / 2.0;

    double x1 =
        hi - (hi - lo) / phi;

    double x2 =
        lo + (hi - lo) / phi;

    double y1 = objective(x1);
    double y2 = objective(x2);

    for (int i = 0; i < maxIterations; ++i) {
        if (hi - lo <= tolerance) {
            break;
        }

        if (y1 <= y2) {
            hi = x2;
            x2 = x1;
            y2 = y1;

            x1 =
                hi - (hi - lo) / phi;

            y1 = objective(x1);
        } else {
            lo = x1;
            x1 = x2;
            y1 = y2;

            x2 =
                lo + (hi - lo) / phi;

            y2 = objective(x2);
        }
    }

    return (lo + hi) / 2.0;
}

}
