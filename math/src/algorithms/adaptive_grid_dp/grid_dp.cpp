#include "grid_dp.hpp"

#include "one_segment_cost.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>
#include <thread>
#include <utility>
#include <vector>

namespace {

template <class F>
void parallelFor(int begin, int end, F&& fn) {
#ifdef __EMSCRIPTEN__
    for (int i = begin; i < end; ++i) fn(i);
#else
    const int count = end - begin;
    if (count <= 1) {
        for (int i = begin; i < end; ++i) fn(i);
        return;
    }

    const unsigned workers = std::max(
        1u,
        std::thread::hardware_concurrency()
    );
    const int threadCount = std::min(
        count,
        static_cast<int>(workers)
    );

    if (threadCount <= 1) {
        for (int i = begin; i < end; ++i) fn(i);
        return;
    }

    std::vector<std::thread> threads;
    threads.reserve(threadCount);

    const int chunk = (count + threadCount - 1) / threadCount;
    for (int t = 0; t < threadCount; ++t) {
        const int first = begin + t * chunk;
        const int last = std::min(end, first + chunk);
        if (first >= last) break;
        threads.emplace_back([first, last, &fn] {
            for (int i = first; i < last; ++i) fn(i);
        });
    }

    for (auto& thread : threads) thread.join();
#endif
}

}

namespace cover_curve::algorithms::adaptive_grid_dp {

Result solveGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N
) {
    if (!f) {
        throw std::invalid_argument(
            "Function must be valid."
        );
    }

    if (!std::isfinite(a) ||
        !std::isfinite(b) ||
        a >= b) {
        throw std::invalid_argument(
            "Require finite a < b."
        );
    }

    if (n < 1) {
        throw std::invalid_argument(
            "n must be positive."
        );
    }

    if (N < n) {
        throw std::invalid_argument(
            "N must satisfy N >= n."
        );
    }

    const double infinity =
        std::numeric_limits<double>::infinity();

    std::vector<double> points(N + 1);

    for (int i = 0; i <= N; ++i) {
        points[i] =
            a + (b - a) * i / N;
    }

    std::vector<std::vector<double>> costs(
        N + 1,
        std::vector<double>(N + 1, infinity)
    );

    std::vector<std::vector<Segment>> segments(
        N + 1,
        std::vector<Segment>(N + 1)
    );

    // Every segment (i, j) is independent, so the expensive one-segment
    // computations can be evaluated concurrently on native builds.
    parallelFor(0, N, [&](int i) {
        for (int j = i + 1; j <= N; ++j) {
            const Segment segment =
                oneSegmentCost(
                    f,
                    points[i],
                    points[j]
                );

            if (!std::isfinite(segment.cost)) {
                throw std::runtime_error(
                    "Failed to compute segment cost."
                );
            }

            costs[i][j] = segment.cost;
            segments[i][j] = segment;
        }
    });

    std::vector<std::vector<double>> dp(
        n + 1,
        std::vector<double>(N + 1, infinity)
    );

    std::vector<std::vector<int>> parent(
        n + 1,
        std::vector<int>(N + 1, -1)
    );

    dp[0][0] = 0.0;

    // For a fixed layer k, each destination j reads only the previous
    // layer and writes a distinct dp[k][j], so these states are independent.
    for (int k = 1; k <= n; ++k) {
        parallelFor(k, N + 1, [&](int j) {
            double best = infinity;
            int bestParent = -1;

            for (int i = k - 1; i < j; ++i) {
                if (!std::isfinite(dp[k - 1][i])) {
                    continue;
                }

                const double candidate =
                    dp[k - 1][i] +
                    costs[i][j];

                if (candidate < best) {
                    best = candidate;
                    bestParent = i;
                }
            }

            dp[k][j] = best;
            parent[k][j] = bestParent;
        });
    }

    if (!std::isfinite(dp[n][N])) {
        throw std::runtime_error(
            "No feasible solution found."
        );
    }

    std::vector<int> breakpointIndices(n + 1);

    int j = N;
    breakpointIndices[n] = N;

    for (int k = n; k >= 1; --k) {
        const int i = parent[k][j];

        if (i < 0) {
            throw std::runtime_error(
                "Failed to reconstruct solution."
            );
        }

        breakpointIndices[k - 1] = i;
        j = i;
    }

    std::vector<double> breakpoints(n + 1);

    for (int k = 0; k <= n; ++k) {
        breakpoints[k] =
            points[breakpointIndices[k]];
    }

    std::vector<Segment> resultSegments;
    resultSegments.reserve(n);

    for (int k = 0; k < n; ++k) {
        const int i =
            breakpointIndices[k];

        const int j2 =
            breakpointIndices[k + 1];

        resultSegments.push_back(
            segments[i][j2]
        );
    }

    return {
        dp[n][N],
        std::move(breakpoints),
        std::move(resultSegments)
    };
}

}
