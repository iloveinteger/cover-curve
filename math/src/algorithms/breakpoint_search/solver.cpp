#include <cover_curve/solvers/breakpoint_search.hpp>

#include "../fast_grid_dp/grid_dp.hpp"

#include <algorithm>
#include <cmath>
#include <deque>
#include <stdexcept>
#include <utility>
#include <vector>

namespace cover_curve {

namespace {

struct Node {
    std::vector<double> lower;
    std::vector<double> upper;
    int depth = 0;
};

bool strictlyIncreasing(
    const std::vector<double>& x,
    double a,
    double b
) {
    if (x.empty())
        return a < b;

    if (!(a < x.front()) || !(x.back() < b))
        return false;

    for (std::size_t i = 1; i < x.size(); ++i) {
        if (!(x[i - 1] < x[i]))
            return false;
    }

    return true;
}

// Return the box midpoint when it is feasible. Otherwise construct a
// strictly ordered point by a backward pass. The latter is only a sampling
// rule; it does not discard any part of the search domain.
bool sampleNode(
    const Node& node,
    double a,
    double b,
    std::vector<double>& sample
) {
    const std::size_t d = node.lower.size();
    sample.resize(d);

    for (std::size_t i = 0; i < d; ++i) {
        sample[i] =
            (node.lower[i] + node.upper[i]) / 2.0;
    }

    if (strictlyIncreasing(sample, a, b))
        return true;

    if (d == 0)
        return true;

    const double scale = std::max(1.0, std::abs(b - a));
    const double eps =
        std::max(1e-14 * scale, 1e-15);

    double next = b - eps;

    for (std::size_t r = d; r-- > 0;) {
        const std::size_t i = r;

        const double candidate =
            std::min(
                {
                    sample[i],
                    node.upper[i],
                    next - eps
                }
            );

        if (!(candidate > node.lower[i]))
            return false;

        sample[i] = candidate;
        next = candidate;
    }

    return strictlyIncreasing(sample, a, b);
}

int widestDimension(const Node& node) {
    int best = -1;
    double width = -1.0;

    for (std::size_t i = 0; i < node.lower.size(); ++i) {
        const double w = node.upper[i] - node.lower[i];

        if (w > width) {
            width = w;
            best = static_cast<int>(i);
        }
    }

    return best;
}

std::vector<Node> splitNode(const Node& node) {
    const int dimension = widestDimension(node);

    if (dimension < 0)
        return {};

    const double lo = node.lower[dimension];
    const double hi = node.upper[dimension];
    const double mid = (lo + hi) / 2.0;

    if (!(lo < mid && mid < hi))
        return {};

    Node left = node;
    Node right = node;

    left.upper[dimension] = mid;
    right.lower[dimension] = mid;

    left.depth = node.depth + 1;
    right.depth = node.depth + 1;

    return {std::move(left), std::move(right)};
}

}

Result breakpointSearch(
    const Function& f,
    double a,
    double b,
    int n,
    const BreakpointSearchOptions& options
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
            "Require n >= 1."
        );

    if (options.maxDepth < 0)
        throw std::invalid_argument(
            "maxDepth must be nonnegative."
        );

    if (options.maxEvaluations < 1)
        throw std::invalid_argument(
            "maxEvaluations must be positive."
        );

    // There is no breakpoint variable for a single segment.
    if (n == 1) {
        return algorithms::fast_grid_dp::solveFastGridDPOnGrid(
            f,
            std::vector<double>{a, b},
            1
        );
    }

    const int dimension = n - 1;

    Node root;
    root.lower.assign(dimension, a);
    root.upper.assign(dimension, b);

    std::deque<Node> queue;
    queue.push_back(std::move(root));

    Result best;
    bool haveBest = false;
    int evaluations = 0;

    while (!queue.empty() &&
           evaluations < options.maxEvaluations) {
        Node node = std::move(queue.front());
        queue.pop_front();

        std::vector<double> internal;
        if (sampleNode(node, a, b, internal)) {
            std::vector<double> points;
            points.reserve(n + 1);
            points.push_back(a);
            points.insert(
                points.end(),
                internal.begin(),
                internal.end()
            );
            points.push_back(b);

            try {
                const Result candidate =
                    algorithms::adaptive_grid_dp::solveGridDPOnGrid(
                        f,
                        points,
                        n
                    );

                ++evaluations;

                if (!haveBest ||
                    candidate.value < best.value) {
                    best = candidate;
                    haveBest = true;
                }
            } catch (const std::exception&) {
                // A numerically unusable inner solve must not prevent
                // the outer exhaustive subdivision from exploring the
                // remaining breakpoint domain.
            }
        }

        if (node.depth >= options.maxDepth)
            continue;

        const auto children = splitNode(node);

        for (auto& child : children)
            queue.push_back(std::move(child));
    }

    if (!haveBest) {
        throw std::runtime_error(
            "Breakpoint search found no numerically evaluable "
            "strict breakpoint sequence."
        );
    }

    return best;
}

}
