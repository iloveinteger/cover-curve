#include "integration.hpp"

#include <cmath>

namespace cover_curve::numerical {

namespace {

double simpson(
    double a,
    double b,
    double fa,
    double fm,
    double fb
) {
    return (b - a) * (fa + 4.0 * fm + fb) / 6.0;
}

double adaptiveSimpson(
    const Function& f,
    double a,
    double b,
    double fa,
    double fm,
    double fb,
    double whole,
    double tolerance,
    int depth
) {
    const double m = (a + b) / 2.0;
    const double lm = (a + m) / 2.0;
    const double rm = (m + b) / 2.0;

    const double flm = f(lm);
    const double frm = f(rm);

    const double left =
        simpson(a, m, fa, flm, fm);

    const double right =
        simpson(m, b, fm, frm, fb);

    const double refined = left + right;
    const double error = refined - whole;

    if (
        depth <= 0 ||
        std::abs(error) <= 15.0 * tolerance
    ) {
        return refined + error / 15.0;
    }

    return
        adaptiveSimpson(
            f,
            a,
            m,
            fa,
            flm,
            fm,
            left,
            tolerance / 2.0,
            depth - 1
        )
        +
        adaptiveSimpson(
            f,
            m,
            b,
            fm,
            frm,
            fb,
            right,
            tolerance / 2.0,
            depth - 1
        );
}

}

double adaptiveIntegral(
    const Function& f,
    double a,
    double b,
    double tolerance,
    int maxDepth
) {
    if (a == b) {
        return 0.0;
    }

    if (a > b) {
        return -adaptiveIntegral(
            f,
            b,
            a,
            tolerance,
            maxDepth
        );
    }

    const double m = (a + b) / 2.0;

    const double fa = f(a);
    const double fm = f(m);
    const double fb = f(b);

    const double whole =
        simpson(a, b, fa, fm, fb);

    return adaptiveSimpson(
        f,
        a,
        b,
        fa,
        fm,
        fb,
        whole,
        tolerance,
        maxDepth
    );
}

}
