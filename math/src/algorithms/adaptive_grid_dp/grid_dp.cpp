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

Result solveGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N,
    int heightLevels
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (!std::isfinite(a) ||
        !std::isfinite(b) ||
        a >= b) {
        throw std::invalid_argument(
            "Require finite a < b."
        );
    }

    if (n < 1)
        throw std::invalid_argument(
            "n must be positive."
        );

    if (N < n)
        throw std::invalid_argument(
            "N must satisfy N >= n."
        );

    if (heightLevels < 2)
        throw std::invalid_argument(
            "heightLevels must be at least 2."
        );

    const double infinity =
        std::numeric_limits<double>::infinity();

    std::vector<double> points(N + 1);

    for (int i = 0; i <= N; ++i) {
        points[i] =
            a + (b - a) * i / N;
    }

    const double rho =
        (b - a) / N;

    const double minimum =
        safeFunctionMinimum(f, points);

    const double maximum =
        safeFunctionMaximum(f, points);

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

    // For every candidate segment (i,j) and every left height p,
    // store the minimum right height required by the sampled numerical
    // transition evaluator.
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
            const double u = points[i];
            const double v = points[j];

            for (int hp = 0; hp < H; ++hp) {
                const double p = heights[hp];

                if (p < f(u))
                    continue;

                transitions[i][j][hp] =
                    transitionConstraint(
                        f,
                        u,
                        v,
                        p
                    );
            }
        }
    }

    std::vector<
        std::vector<DPState>
    > previous(
        N + 1,
        std::vector<DPState>(H)
    );

    std::vector<
        std::vector<DPState>
    > current(
        N + 1,
        std::vector<DPState>(H)
    );

    for (int hp = 0; hp < H; ++hp) {
        if (heights[hp] >= f(a)) {
            previous[0][hp].value = 0.0;
        }
    }

    for (int k = 1; k <= n; ++k) {
        for (int j = 0; j <= N; ++j) {
            for (int hp = 0; hp < H; ++hp) {
                current[j][hp] = {};
            }
        }

        for (int j = k; j <= N; ++j) {
            const double x1 = points[j];

            for (int hq = 0; hq < H; ++hq) {
                const double q = heights[hq];

                if (q < f(x1))
                    continue;

                double best = infinity;
                int bestGrid = -1;
                int bestHeight = -1;

                for (int i = k - 1; i < j; ++i) {
                    const double dx =
                        x1 - points[i];

                    const double qCoefficient =
                        dx / 2.0;

                    for (int hp = 0; hp < H; ++hp) {
                        const DPState& state =
                            previous[i][hp];

                        if (!std::isfinite(state.value))
                            continue;

                        const TransitionConstraint& transition =
                            transitions[i][j][hp];

                        if (q < transition.height)
                            continue;

                        const double candidate =
                            state.value
                            + qCoefficient * (heights[hp] + q);

                        if (candidate < best) {
                            best = candidate;
                            bestGrid = i;
                            bestHeight = hp;
                        }
                    }
                }

                current[j][hq].value = best;
                current[j][hq].parentGrid = bestGrid;
                current[j][hq].parentHeight = bestHeight;
            }
        }

        previous.swap(current);
    }

    double bestIntegral = infinity;
    int finalHeight = -1;

    for (int hq = 0; hq < H; ++hq) {
        if (previous[N][hq].value < bestIntegral) {
            bestIntegral = previous[N][hq].value;
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

    // The current layer stores only parents for the final k. Recompute
    // predecessor layers during backtracking using the same recurrence.
    // This keeps the persistent memory independent of n.
    //
    // For the current implementation, retain the complete DP history
    // instead of attempting to reconstruct through recomputation.
    std::vector<
        std::vector<
            std::vector<DPState>
        >
    > history(
        n + 1,
        std::vector<std::vector<DPState>>(
            N + 1,
            std::vector<DPState>(H)
        )
    );

    for (int hp = 0; hp < H; ++hp) {
        if (heights[hp] >= f(a))
            history[0][0][hp].value = 0.0;
    }

    for (int k = 1; k <= n; ++k) {
        for (int j = k; j <= N; ++j) {
            const double x1 = points[j];

            for (int hq = 0; hq < H; ++hq) {
                const double q = heights[hq];

                if (q < f(x1))
                    continue;

                DPState bestState;

                for (int i = k - 1; i < j; ++i) {
                    const double dx =
                        x1 - points[i];

                    for (int hp = 0; hp < H; ++hp) {
                        const DPState& state =
                            history[k - 1][i][hp];

                        if (!std::isfinite(state.value))
                            continue;

                        const auto& transition =
                            transitions[i][j][hp];

                        if (q < transition.height)
                            continue;

                        const double candidate =
                            state.value
                            + dx * (heights[hp] + q) / 2.0;

                        if (candidate < bestState.value) {
                            bestState.value = candidate;
                            bestState.parentGrid = i;
                            bestState.parentHeight = hp;
                        }
                    }
                }

                history[k][j][hq] = bestState;
            }
        }
    }

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

    const double integralF =
        numerical::adaptiveIntegral(f, a, b);

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

        if (cost < -1e-9) {
            throw std::runtime_error(
                "Constructed segment violates numerical majorant constraints."
            );
        }

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

} // namespace cover_curve::algorithms::adaptive_grid_dp
