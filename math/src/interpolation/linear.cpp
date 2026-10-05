#include "linear.hpp"

#include <stdexcept>

namespace cover_curve::interpolation {

Function linear(
    const DataPoints& points
) {
    if (points.size() < 2) {
        throw std::invalid_argument("At least two data points are required.");
    }

    for (std::size_t i = 1; i < points.size(); ++i) {
        if (points[i - 1].first >= points[i].first) {
            throw std::invalid_argument("Data-point x values must be strictly increasing.");
        }
    }

    return [points](double x) {
        if (x < points.front().first || x > points.back().first) {
            throw std::out_of_range("x is outside the interpolation interval.");
        }
        if (x == points.back().first) return points.back().second;

        std::size_t lo = 0, hi = points.size() - 1;
        while (lo + 1 < hi) {
            const std::size_t mid = lo + (hi - lo) / 2;
            if (points[mid].first <= x) lo = mid;
            else hi = mid;
        }

        const double x0 = points[lo].first;
        const double y0 = points[lo].second;
        const double x1 = points[lo + 1].first;
        const double y1 = points[lo + 1].second;
        const double t = (x - x0) / (x1 - x0);
        return y0 + t * (y1 - y0);
    };
}

}

namespace cover_curve {

Function linearInterpolation(const DataPoints& points) {
    return interpolation::linear(points);
}

}
