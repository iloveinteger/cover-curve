#include <cover_curve/solvers/coordinate_search.hpp>

#include "../fast_grid_dp/grid_dp.hpp"
#include "../direct_height/solver.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cover_curve {

namespace {

Result evaluateFixed(
    const Function& f,
    const std::vector<double>& points,
    int n,
    bool useDirectHeightOracle
) {
    if (useDirectHeightOracle)
        return directHeightSolve(f, points);

    return algorithms::fast_grid_dp::solveFastGridDPOnGrid(f, points, n);
}

bool better(double candidate, double incumbent, double tolerance) {
    return candidate + tolerance < incumbent;
}

}

Result coordinateSearch(
    const Function& f,
    double a,
    double b,
    int n,
    const CoordinateSearchOptions& options
) {
    if (!f)
        throw std::invalid_argument("Function must be valid.");

    if (!std::isfinite(a) || !std::isfinite(b) || a >= b)
        throw std::invalid_argument("Require finite a < b.");

    if (n < 1)
        throw std::invalid_argument("n must be positive.");

    if (options.maxSweeps < 0)
        throw std::invalid_argument("maxSweeps must be nonnegative.");

    if (options.samples < 3)
        throw std::invalid_argument("samples must be at least 3.");

    if (options.refinements < 0)
        throw std::invalid_argument("refinements must be nonnegative.");

    if (!std::isfinite(options.tolerance) ||
        options.tolerance <= 0.0) {
        throw std::invalid_argument("tolerance must be positive.");
    }

    if (n == 1) {
        if (options.useDirectHeightOracle)
            return directHeightSolve(f, a, b, 1);
        return algorithms::fast_grid_dp::solveFastGridDP(f, a, b, 1, 8);
    }

    // Start from the optimized grid-DP solution. Coordinate search is
    // therefore a refinement layer and never needs to discover a good
    // breakpoint configuration from scratch.
    Result incumbent =
        algorithms::fast_grid_dp::solveFastGridDP(f, a, b, n, 32);

    if (incumbent.breakpoints.size() !=
        static_cast<std::size_t>(n + 1)) {
        throw std::runtime_error(
            "fastGridDP returned an invalid breakpoint vector."
        );
    }

    std::vector<double> points = incumbent.breakpoints;

    for (int sweep = 0; sweep < options.maxSweeps; ++sweep) {
        const double sweepStart = incumbent.value;

        for (int i = 1; i < n; ++i) {
            const double left = points[i - 1];
            const double right = points[i + 1];

            const double width = right - left;
            const double eps =
                std::max(
                    1e-12 * std::max(1.0, std::abs(b - a)),
                    1e-14 * width
                );

            double lo = left + eps;
            double hi = right - eps;

            if (!(lo < hi))
                continue;

            std::vector<double> bestPoints = points;
            Result bestResult = incumbent;

            for (int refinement = 0;
                 refinement <= options.refinements;
                 ++refinement) {

                if (!(lo < hi))
                    break;

                const int count = options.samples;
                int bestSample = -1;
                double bestSampleValue =
                    std::numeric_limits<double>::infinity();
                Result bestSampleResult{};

                for (int s = 0; s < count; ++s) {
                    const double t =
                        lo + (hi - lo) *
                        static_cast<double>(s) /
                        static_cast<double>(count - 1);

                    std::vector<double> candidatePoints = points;
                    candidatePoints[i] = t;

                    Result candidate =
                        evaluateFixed(
                            f,
                            candidatePoints,
                            n,
                            options.useDirectHeightOracle
                        );

                    if (candidate.value < bestSampleValue) {
                        bestSampleValue = candidate.value;
                        bestSample = s;
                        bestSampleResult = std::move(candidate);
                    }

                    if (better(
                            candidate.value,
                            bestResult.value,
                            options.tolerance)) {
                        bestResult = candidate;
                        bestPoints = std::move(candidatePoints);
                    }
                }

                if (bestSample < 0)
                    break;

                const double spacing =
                    (hi - lo) /
                    static_cast<double>(count - 1);

                double nextLo;
                double nextHi;

                if (bestSample == 0) {
                    nextLo = lo;
                    nextHi = lo + spacing;
                } else if (bestSample == count - 1) {
                    nextLo = hi - spacing;
                    nextHi = hi;
                } else {
                    nextLo =
                        lo + spacing *
                        static_cast<double>(bestSample - 1);
                    nextHi =
                        lo + spacing *
                        static_cast<double>(bestSample + 1);
                }

                lo = std::max(left + eps, nextLo);
                hi = std::min(right - eps, nextHi);

                if (better(
                        bestSampleResult.value,
                        incumbent.value,
                        options.tolerance)) {
                    points = bestSampleResult.breakpoints;
                    incumbent = std::move(bestSampleResult);
                }
            }

            if (better(
                    bestResult.value,
                    incumbent.value,
                    options.tolerance)) {
                points = bestPoints;
                incumbent = std::move(bestResult);
            }
        }

        const double scale =
            std::max({
                1.0,
                std::abs(sweepStart),
                std::abs(incumbent.value)
            });

        if ((sweepStart - incumbent.value) / scale <
            options.tolerance) {
            break;
        }
    }

    return incumbent;
}

}
