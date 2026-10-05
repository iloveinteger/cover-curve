#include "grid_dp.hpp"

#include "../adaptive_grid_dp/one_segment_cost.hpp"
#include "../../numerical/integration.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace {

cover_curve::Result makeContinuousResult(
    const cover_curve::Function& f,
    const std::vector<double>& breakpoints,
    const std::vector<cover_curve::Segment>& rawSegments
) {
    const int n = static_cast<int>(rawSegments.size());
    std::vector<double> heights(n + 1);

    for (int k = 0; k <= n; ++k) {
        double height = f(breakpoints[k]);

        if (k > 0) {
            const auto& left = rawSegments[k - 1];
            height = std::max(
                height,
                left.intercept + left.slope * breakpoints[k]
            );
        }

        if (k < n) {
            const auto& right = rawSegments[k];
            height = std::max(
                height,
                right.intercept + right.slope * breakpoints[k]
            );
        }

        heights[k] = height;
    }

    std::vector<cover_curve::Segment> segments;
    segments.reserve(n);

    double totalCost = 0.0;

    for (int k = 0; k < n; ++k) {
        const double x0 = breakpoints[k];
        const double x1 = breakpoints[k + 1];
        const double dx = x1 - x0;

        const double slope =
            (heights[k + 1] - heights[k]) / dx;
        const double intercept =
            heights[k] - slope * x0;

        const double integral =
            cover_curve::numerical::adaptiveIntegral(f, x0, x1);

        const double cost = std::max(
            0.0,
            dx * (heights[k] + heights[k + 1]) / 2.0
                - integral
        );

        segments.push_back({
            x0,
            x1,
            slope,
            intercept,
            cost,
            rawSegments[k].contact
        });

        totalCost += cost;
    }

    return {
        totalCost,
        breakpoints,
        std::move(segments)
    };
}

}

namespace cover_curve::algorithms::curvature_adaptive {

Result solveGrid(
    const Function& f,
    const std::vector<double>& points,
    int n
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");
    if (points.size() < 2)
        throw std::invalid_argument(
            "Grid must contain at least two points."
        );
    if (n < 1 || static_cast<int>(points.size()) - 1 < n)
        throw std::invalid_argument(
            "Require at least n grid cells."
        );

    const int N = static_cast<int>(points.size()) - 1;
    const double infinity =
        std::numeric_limits<double>::infinity();

    std::vector<std::vector<double>> costs(
        N + 1,
        std::vector<double>(N + 1, infinity)
    );
    std::vector<std::vector<Segment>> segments(
        N + 1,
        std::vector<Segment>(N + 1)
    );

    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j <= N; ++j) {
            const Segment segment =
                adaptive_grid_dp::oneSegmentCost(
                    f, points[i], points[j]
                );
            if (!std::isfinite(segment.cost))
                throw std::runtime_error(
                    "Failed to compute segment cost."
                );
            costs[i][j] = segment.cost;
            segments[i][j] = segment;
        }
    }

    std::vector<std::vector<double>> dp(
        n + 1,
        std::vector<double>(N + 1, infinity)
    );
    std::vector<std::vector<int>> parent(
        n + 1,
        std::vector<int>(N + 1, -1)
    );

    dp[0][0] = 0.0;

    for (int k = 1; k <= n; ++k) {
        for (int j = k; j <= N; ++j) {
            for (int i = k - 1; i < j; ++i) {
                if (!std::isfinite(dp[k - 1][i]))
                    continue;

                const double candidate =
                    dp[k - 1][i] + costs[i][j];

                if (candidate < dp[k][j]) {
                    dp[k][j] = candidate;
                    parent[k][j] = i;
                }
            }
        }
    }

    if (!std::isfinite(dp[n][N]))
        throw std::runtime_error(
            "No feasible solution found."
        );

    std::vector<int> indices(n + 1);
    int j = N;
    indices[n] = N;

    for (int k = n; k >= 1; --k) {
        const int i = parent[k][j];
        if (i < 0)
            throw std::runtime_error(
                "Failed to reconstruct solution."
            );
        indices[k - 1] = i;
        j = i;
    }

    std::vector<double> breakpoints(n + 1);
    std::vector<Segment> rawSegments;
    rawSegments.reserve(n);

    for (int k = 0; k <= n; ++k)
        breakpoints[k] = points[indices[k]];

    for (int k = 0; k < n; ++k)
        rawSegments.push_back(
            segments[indices[k]][indices[k + 1]]
        );

    return makeContinuousResult(
        f,
        breakpoints,
        rawSegments
    );
}

}
