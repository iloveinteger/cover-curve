#include "natural_cubic_spline.hpp"

#include <stdexcept>
#include <vector>

namespace cover_curve::interpolation {

Function naturalCubicSpline(
    const DataPoints& points
) {
    if (points.size() < 2) {
        throw std::invalid_argument(
            "At least two data points are required."
        );
    }

    const std::size_t n = points.size();

    for (std::size_t i = 1; i < n; ++i) {
        if (points[i - 1].first >= points[i].first) {
            throw std::invalid_argument(
                "Data-point x values must be strictly increasing."
            );
        }
    }

    std::vector<double> h(n - 1);

    for (std::size_t i = 0; i + 1 < n; ++i) {
        h[i] =
            points[i + 1].first -
            points[i].first;
    }

    std::vector<double> lower(n, 0.0);
    std::vector<double> diagonal(n, 0.0);
    std::vector<double> upper(n, 0.0);
    std::vector<double> rhs(n, 0.0);

    diagonal[0] = 1.0;
    diagonal[n - 1] = 1.0;

    for (std::size_t i = 1; i + 1 < n; ++i) {
        lower[i] = h[i - 1];
        diagonal[i] =
            2.0 * (h[i - 1] + h[i]);
        upper[i] = h[i];

        rhs[i] =
            6.0 * (
                (points[i + 1].second -
                 points[i].second) / h[i]
                -
                (points[i].second -
                 points[i - 1].second) / h[i - 1]
            );
    }

    std::vector<double> cPrime(n, 0.0);
    std::vector<double> dPrime(n, 0.0);

    cPrime[0] =
        upper[0] / diagonal[0];

    dPrime[0] =
        rhs[0] / diagonal[0];

    for (std::size_t i = 1; i < n; ++i) {
        const double denominator =
            diagonal[i] -
            lower[i] * cPrime[i - 1];

        if (denominator == 0.0) {
            throw std::runtime_error(
                "Failed to construct the cubic spline."
            );
        }

        cPrime[i] =
            i + 1 < n
                ? upper[i] / denominator
                : 0.0;

        dPrime[i] =
            (
                rhs[i] -
                lower[i] * dPrime[i - 1]
            ) / denominator;
    }

    std::vector<double> secondDerivative(n);

    secondDerivative[n - 1] =
        dPrime[n - 1];

    for (std::size_t i = n - 1; i-- > 0;) {
        secondDerivative[i] =
            dPrime[i] -
            cPrime[i] *
            secondDerivative[i + 1];
    }

    return [points, h, secondDerivative](double x) {
        if (
            x < points.front().first ||
            x > points.back().first
        ) {
            throw std::out_of_range(
                "x is outside the interpolation interval."
            );
        }

        if (x == points.back().first) {
            return points.back().second;
        }

        std::size_t lo = 0;
        std::size_t hi = points.size() - 1;

        while (lo + 1 < hi) {
            const std::size_t mid =
                lo + (hi - lo) / 2;

            if (points[mid].first <= x) {
                lo = mid;
            } else {
                hi = mid;
            }
        }

        const double interval = h[lo];

        const double A =
            (points[lo + 1].first - x) /
            interval;

        const double B =
            (x - points[lo].first) /
            interval;

        return
            A * points[lo].second
            +
            B * points[lo + 1].second
            +
            (
                (A * A * A - A) *
                    secondDerivative[lo]
                +
                (B * B * B - B) *
                    secondDerivative[lo + 1]
            ) *
            interval * interval / 6.0;
    };
}

}
