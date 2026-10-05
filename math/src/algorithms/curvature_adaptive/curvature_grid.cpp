#include "curvature_grid.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <vector>

namespace cover_curve::algorithms::curvature_adaptive {

namespace {

double secondDerivative(
    const Function& f,
    double x,
    double a,
    double b,
    double step
) {
    if (x - step >= a && x + step <= b) {
        return (f(x + step) - 2.0 * f(x) + f(x - step))
            / (step * step);
    }

    if (x + 2.0 * step <= b) {
        return (f(x + 2.0 * step)
                - 2.0 * f(x + step)
                + f(x))
            / (step * step);
    }

    if (x - 2.0 * step >= a) {
        return (f(x)
                - 2.0 * f(x - step)
                + f(x - 2.0 * step))
            / (step * step);
    }

    return 0.0;
}

std::vector<double> uniformGrid(double a, double b, int N) {
    std::vector<double> grid(N + 1);
    const double length = b - a;
    for (int i = 0; i <= N; ++i)
        grid[i] = a + length * i / N;
    return grid;
}

}

std::vector<double> makeCurvatureGrid(
    const Function& f,
    double a,
    double b,
    int N,
    int samples
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");
    if (!std::isfinite(a) || !std::isfinite(b) || a >= b)
        throw std::invalid_argument("Require finite a < b.");
    if (N < 1)
        throw std::invalid_argument("N must be positive.");
    if (samples < 3)
        throw std::invalid_argument("samples must be at least 3.");

    const double length = b - a;
    const double dx = length / (samples - 1);
    const double step = std::max(
        length * 1e-5,
        dx * 0.25
    );

    std::vector<double> x(samples);
    std::vector<double> weight(samples);

    for (int k = 0; k < samples; ++k) {
        x[k] = a + length * k / (samples - 1);
        const double q = secondDerivative(
            f, x[k], a, b, step
        );
        weight[k] = std::isfinite(q)
            ? std::sqrt(std::abs(q))
            : 0.0;
    }

    double positiveMin = std::numeric_limits<double>::infinity();
    double positiveMax = 0.0;

    for (double w : weight) {
        if (w > 0.0 && std::isfinite(w)) {
            positiveMin = std::min(positiveMin, w);
            positiveMax = std::max(positiveMax, w);
        }
    }

    if (!std::isfinite(positiveMin) || positiveMax <= 0.0)
        return uniformGrid(a, b, N);

    const double floorWeight = std::max(
        positiveMin * 0.05,
        positiveMax * 1e-4
    );
    const double capWeight = positiveMax * 20.0;

    for (double& w : weight) {
        if (!std::isfinite(w) || w < floorWeight)
            w = floorWeight;
        w = std::min(w, capWeight);
    }

    std::vector<double> cumulative(samples, 0.0);
    for (int k = 1; k < samples; ++k) {
        cumulative[k] = cumulative[k - 1]
            + 0.5 * (weight[k - 1] + weight[k]) * dx;
    }

    const double total = cumulative.back();
    if (!std::isfinite(total) || total <= 0.0)
        return uniformGrid(a, b, N);

    std::vector<double> grid(N + 1);
    grid[0] = a;
    grid[N] = b;

    for (int i = 1; i < N; ++i) {
        const double target = total * i / N;
        const auto it = std::lower_bound(
            cumulative.begin(),
            cumulative.end(),
            target
        );
        const int hi = static_cast<int>(
            std::distance(cumulative.begin(), it)
        );

        if (hi <= 0) {
            grid[i] = a;
        } else if (hi >= samples) {
            grid[i] = b;
        } else {
            const int lo = hi - 1;
            const double span = cumulative[hi] - cumulative[lo];
            const double t = span > 0.0
                ? (target - cumulative[lo]) / span
                : 0.0;
            grid[i] = x[lo] + t * (x[hi] - x[lo]);
        }
    }

    grid[0] = a;
    grid[N] = b;
    for (int i = 1; i < N; ++i) {
        if (!(grid[i] > grid[i - 1]))
            grid[i] = std::nextafter(grid[i - 1], b);
    }

    if (!(grid[N] > grid[N - 1]))
        return uniformGrid(a, b, N);

    return grid;
}

}
