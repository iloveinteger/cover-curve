#include "grid_dp.hpp"

#include "../../numerical/integration.hpp"
#include "../../numerical/support_max.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

using cover_curve::Function;
using cover_curve::Segment;

struct TransitionConstraint {
    double height;
    double contact;
};

struct DPState {
    double value = std::numeric_limits<double>::infinity();
    int parentGrid = -1;
    int parentHeight = -1;
};

double safeFunctionMinimum(
    const Function& f,
    const std::vector<double>& points
) {
    double minimum = std::numeric_limits<double>::infinity();

    for (double x : points)
        minimum = std::min(minimum, f(x));

    const Function negated = [&](double x) {
        return -f(x);
    };

    const auto support = cover_curve::numerical::adaptiveSupportMaximum(
        negated,
        points.front(),
        points.back(),
        0.0
    );

    if (std::isfinite(support.value))
        minimum = std::min(minimum, -support.value);

    return minimum;
}

double safeFunctionMaximum(
    const Function& f,
    const std::vector<double>& points
) {
    double maximum = -std::numeric_limits<double>::infinity();

    for (double x : points)
        maximum = std::max(maximum, f(x));

    const auto support = cover_curve::numerical::adaptiveSupportMaximum(
        f,
        points.front(),
        points.back(),
        0.0
    );

    if (std::isfinite(support.value))
        maximum = std::max(maximum, support.value);

    return maximum;
}

std::vector<double> makeHeightGrid(
    double minimum,
    double maximum,
    double rho,
    int heightLevels
) {
    if (heightLevels < 2)
        throw std::invalid_argument(
            "heightLevels must be at least 2."
        );

    const double C =
        std::max(0.0, maximum - minimum);

    const double lengthScale =
        std::max(rho, std::numeric_limits<double>::min());

    double upper =
        minimum + 4.0 * C / lengthScale;

    if (!std::isfinite(upper) || upper < maximum) {
        upper = maximum;
    }

    if (upper <= minimum) {
        return {minimum};
    }

    std::vector<double> heights(heightLevels);

    for (int i = 0; i < heightLevels; ++i) {
        heights[i] =
            minimum
            + (upper - minimum) * i
                / (heightLevels - 1);
    }

    return heights;
}

TransitionConstraint transitionConstraint(
    const Function& f,
    double u,
    double v,
    double p
) {
    const double width = v - u;

    // T(u,v;p) uses x>u.  The support-search primitive evaluates the
    // interval endpoints, so replace the singular endpoint by a point
    // infinitesimally inside the interval.
    const double epsilon =
        std::max(
            1e-12 * width,
            32.0 * std::numeric_limits<double>::epsilon()
                * std::max({1.0, std::abs(u), std::abs(v)})
        );

    const double left =
        std::min(v, u + epsilon);

    const Function ratio = [&](double x) {
        const double xx = std::max(x, left);
        return
            (f(xx) - p) / (xx - u);
    };

    const auto support =
        cover_curve::numerical::adaptiveSupportMaximum(
            ratio,
            left,
            v,
            0.0
        );

    if (!std::isfinite(support.value))
        return {
            std::numeric_limits<double>::infinity(),
            support.x
        };

    return {
        p + width * support.value,
        support.x
    };
}

} // namespace

namespace cover_curve::algorithms::adaptive_grid_dp {

namespace {

struct TransitionConstraint {
    double height = std::numeric_limits<double>::infinity();
    double contact = std::numeric_limits<double>::quiet_NaN();
};

struct DPState {
    double value = std::numeric_limits<double>::infinity();
    int parentGrid = -1;
    int parentHeight = -1;
};

double sampledMinimum(
    const Function& f,
    const std::vector<double>& points
) {
    double minimum = std::numeric_limits<double>::infinity();
    for (double x : points)
        minimum = std::min(minimum, f(x));

    const Function negated = [&](double x) {
        return -f(x);
    };
    const auto support =
        numerical::adaptiveSupportMaximum(
            negated,
            points.front(),
            points.back(),
            0.0
        );

    if (std::isfinite(support.value))
        minimum = std::min(minimum, -support.value);

    return minimum;
}

double sampledMaximum(
    const Function& f,
    const std::vector<double>& points
) {
    double maximum = -std::numeric_limits<double>::infinity();
    for (double x : points)
        maximum = std::max(maximum, f(x));

    const auto support =
        numerical::adaptiveSupportMaximum(
            f,
            points.front(),
            points.back(),
            0.0
        );

    if (std::isfinite(support.value))
        maximum = std::max(maximum, support.value);

    return maximum;
}

std::vector<double> makeHeightGrid(
    double minimum,
    double maximum,
    double rho,
    int heightLevels
) {
    if (heightLevels < 2)
        throw std::invalid_argument(
            "heightLevels must be at least 2."
        );

    const double C =
        std::max(0.0, maximum - minimum);

    const double upper =
        minimum
        + 4.0 * C
            / std::max(
                rho,
                std::numeric_limits<double>::min()
            );

    if (!std::isfinite(upper) || upper <= minimum)
        return {minimum};

    std::vector<double> heights(heightLevels);
    for (int i = 0; i < heightLevels; ++i) {
        heights[i] =
            minimum
            + (upper - minimum) * i
                / (heightLevels - 1);
    }
    return heights;
}

TransitionConstraint computeTransition(
    const Function& f,
    double u,
    double v,
    double p
) {
    const double width = v - u;
    const double epsilon =
        std::max(
            1e-12 * width,
            32.0 * std::numeric_limits<double>::epsilon()
                * std::max({1.0, std::abs(u), std::abs(v)})
        );

    const double left =
        std::min(v, u + epsilon);

    const Function ratio = [&](double x) {
        const double xx = std::max(x, left);
        return (f(xx) - p) / (xx - u);
    };

    const auto support =
        numerical::adaptiveSupportMaximum(
            ratio,
            left,
            v,
            0.0
        );

    if (!std::isfinite(support.value))
        return {};

    return {
        p + width * support.value,
        support.x
    };
}

} // namespace

Result solveGridDPOnGrid(
    const Function& f,
    const std::vector<double>& points,
    int n,
    int heightLevels
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (points.size() < 2)
        throw std::invalid_argument(
            "Grid must contain at least two points."
        );

    if (n < 1 ||
        static_cast<int>(points.size()) - 1 < n) {
        throw std::invalid_argument(
            "Require at least n grid cells."
        );
    }

    for (std::size_t i = 1; i < points.size(); ++i) {
        if (!std::isfinite(points[i - 1]) ||
            !std::isfinite(points[i]) ||
            points[i] <= points[i - 1]) {
            throw std::invalid_argument(
                "Grid points must be finite and strictly increasing."
            );
        }
    }

    const int N =
        static_cast<int>(points.size()) - 1;

    double rho = std::numeric_limits<double>::infinity();
    for (int i = 0; i < N; ++i)
        rho = std::min(
            rho,
            points[i + 1] - points[i]
        );

    const double minimum =
        sampledMinimum(f, points);
    const double maximum =
        sampledMaximum(f, points);

    if (!std::isfinite(minimum) ||
        !std::isfinite(maximum) ||
        minimum > maximum) {
        throw std::runtime_error(
            "Failed to determine a finite function range."
        );
    }

    const std::vector<double> heights =
        makeHeightGrid(
            minimum,
            maximum,
            rho,
            heightLevels
        );

    const int H =
        static_cast<int>(heights.size());

    std::vector<
        std::vector<std::vector<TransitionConstraint>>
    > transitions(
        N + 1,
        std::vector<std::vector<TransitionConstraint>>(
            N + 1,
            std::vector<TransitionConstraint>(H)
        )
    );

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            for (int hp = 0; hp < H; ++hp) {
                const double p = heights[hp];

                if (p < f(points[i]))
                    continue;

                transitions[i][j][hp] =
                    computeTransition(
                        f,
                        points[i],
                        points[j],
                        p
                    );
            }
        }
    }

    std::vector<
        std::vector<std::vector<DPState>>
    > history(
        n + 1,
        std::vector<std::vector<DPState>>(
            N + 1,
            std::vector<DPState>(H)
        )
    );

    for (int hp = 0; hp < H; ++hp) {
        if (heights[hp] >= f(points.front()))
            history[0][0][hp].value = 0.0;
    }

    for (int k = 1; k <= n; ++k) {
        for (int j = k; j <= N; ++j) {
            const double x1 = points[j];

            for (int hq = 0; hq < H; ++hq) {
                const double q = heights[hq];

                if (q < f(x1))
                    continue;

                DPState best;

                for (int i = k - 1; i < j; ++i) {
                    const double dx =
                        x1 - points[i];

                    for (int hp = 0; hp < H; ++hp) {
                        const DPState& previous =
                            history[k - 1][i][hp];

                        if (!std::isfinite(previous.value))
                            continue;

                        const auto& transition =
                            transitions[i][j][hp];

                        if (q < transition.height)
                            continue;

                        const double candidate =
                            previous.value
                            + dx
                                * (heights[hp] + q)
                                / 2.0;

                        if (candidate < best.value) {
                            best.value = candidate;
                            best.parentGrid = i;
                            best.parentHeight = hp;
                        }
                    }
                }

                history[k][j][hq] = best;
            }
        }
    }

    double bestIntegral = std::numeric_limits<double>::infinity();
    int finalHeight = -1;

    for (int hq = 0; hq < H; ++hq) {
        const double value =
            history[n][N][hq].value;

        if (value < bestIntegral) {
            bestIntegral = value;
            finalHeight = hq;
        }
    }

    if (!std::isfinite(bestIntegral) ||
        finalHeight < 0) {
        throw std::runtime_error(
            "No feasible shared-height solution found."
        );
    }

    std::vector<int> gridIndices(n + 1);
    std::vector<int> heightIndices(n + 1);

    gridIndices[n] = N;
    heightIndices[n] = finalHeight;

    for (int k = n; k >= 1; --k) {
        const DPState& state =
            history[k][gridIndices[k]][heightIndices[k]];

        if (state.parentGrid < 0 ||
            state.parentHeight < 0) {
            throw std::runtime_error(
                "Failed to reconstruct shared-height solution."
            );
        }

        gridIndices[k - 1] = state.parentGrid;
        heightIndices[k - 1] = state.parentHeight;
    }

    std::vector<double> breakpoints(n + 1);
    std::vector<double> vertexHeights(n + 1);
    std::vector<Segment> segments;
    segments.reserve(n);

    for (int k = 0; k <= n; ++k) {
        breakpoints[k] = points[gridIndices[k]];
        vertexHeights[k] = heights[heightIndices[k]];
    }

    double totalCost = 0.0;

    for (int k = 0; k < n; ++k) {
        const double x0 = breakpoints[k];
        const double x1 = breakpoints[k + 1];
        const double y0 = vertexHeights[k];
        const double y1 = vertexHeights[k + 1];
        const double dx = x1 - x0;

        const double slope =
            (y1 - y0) / dx;
        const double intercept =
            y0 - slope * x0;

        const auto& transition =
            transitions[
                gridIndices[k]
            ][
                gridIndices[k + 1]
            ][
                heightIndices[k]
            ];

        const double segmentIntegral =
            dx * (y0 + y1) / 2.0;

        const double segmentF =
            numerical::adaptiveIntegral(
                f,
                x0,
                x1
            );

        const double cost =
            segmentIntegral - segmentF;

        segments.push_back({
            x0,
            x1,
            slope,
            intercept,
            std::max(0.0, cost),
            transition.contact
        });

        totalCost += cost;
    }

    return {
        std::max(0.0, totalCost),
        std::move(breakpoints),
        std::move(segments)
    };
}

Result solveGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N,
    int heightLevels
) {
    if (!std::isfinite(a) ||
        !std::isfinite(b) ||
        a >= b) {
        throw std::invalid_argument(
            "Require finite a < b."
        );
    }

    if (N < n)
        throw std::invalid_argument(
            "N must satisfy N >= n."
        );

    std::vector<double> points(N + 1);
    for (int i = 0; i <= N; ++i)
        points[i] =
            a + (b - a) * i / N;

    return solveGridDPOnGrid(
        f,
        points,
        n,
        heightLevels
    );
}

} // namespace cover_curve::algorithms::adaptive_grid_dp

