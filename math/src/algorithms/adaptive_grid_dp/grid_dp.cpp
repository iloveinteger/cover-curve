#include "grid_dp.hpp"

#include "../../numerical/integration.hpp"
#include "../../numerical/support_max.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <map>
#include <stdexcept>
#include <tuple>
#include <utility>
#include <vector>

namespace cover_curve::algorithms::adaptive_grid_dp {

namespace {

constexpr double kHeightTolerance = 1e-7;

// Numerical search controls. Heights remain continuous; these parameters
// only reduce repeated one-dimensional objective evaluations.
constexpr int kGlobalSamples = 3;
constexpr int kLocalIntervals = 1;
constexpr int kGoldenIterations = 6;
constexpr int kTransitionSamples = 8;
constexpr int kTransitionDepth = 2;
constexpr int kTransitionRefinements = 2;

struct Transition {
    double threshold = std::numeric_limits<double>::infinity();
    double contact = std::numeric_limits<double>::quiet_NaN();
};

struct StateKey {
    int k;
    int j;
    double q;

    bool operator<(const StateKey& other) const {
        if (k != other.k) return k < other.k;
        if (j != other.j) return j < other.j;
        return q < other.q;
    }
};

struct StateValue {
    double value = std::numeric_limits<double>::infinity();
    int parentGrid = -1;
    double parentHeight = std::numeric_limits<double>::quiet_NaN();
};

struct TransitionKey {
    int i;
    int j;
    double p;

    bool operator<(const TransitionKey& other) const {
        if (i != other.i) return i < other.i;
        if (j != other.j) return j < other.j;
        return p < other.p;
    }
};

struct LowerHeightKey {
    int i;
    int j;
    double q;

    bool operator<(const LowerHeightKey& other) const {
        if (i != other.i) return i < other.i;
        if (j != other.j) return j < other.j;
        return q < other.q;
    }
};

class ContinuousHeightDP {
public:
    ContinuousHeightDP(
        const Function& f,
        const std::vector<double>& points,
        int n
    )
        : f_(f), points_(points), n_(n) {
        minimum_ = std::numeric_limits<double>::infinity();
        maximum_ = -std::numeric_limits<double>::infinity();

        for (double x : points_) {
            const double y = f_(x);
            minimum_ = std::min(minimum_, y);
            maximum_ = std::max(maximum_, y);
        }

        const auto maxSupport =
            numerical::adaptiveSupportMaximum(
                f_, points_.front(), points_.back(), 0.0,
                8, 2, 2
            );
        const auto minSupport =
            numerical::adaptiveSupportMaximum(
                [&](double x) { return -f_(x); },
                points_.front(), points_.back(), 0.0,
                8, 2, 2
            );

        if (std::isfinite(maxSupport.value))
            maximum_ = std::max(maximum_, maxSupport.value);
        if (std::isfinite(minSupport.value))
            minimum_ = std::min(minimum_, -minSupport.value);

        if (!std::isfinite(minimum_) ||
            !std::isfinite(maximum_) ||
            minimum_ > maximum_) {
            throw std::runtime_error(
                "Failed to determine a finite function range."
            );
        }

        rho_ = std::numeric_limits<double>::infinity();
        for (std::size_t i = 1; i < points_.size(); ++i)
            rho_ = std::min(
                rho_, points_[i] - points_[i - 1]
            );

        const double C =
            (points_.back() - points_.front()) *
            std::max(0.0, maximum_ - minimum_);

        upper_ =
            minimum_ +
            4.0 * C /
            std::max(rho_, std::numeric_limits<double>::min());

        if (!std::isfinite(upper_) || upper_ < maximum_)
            upper_ = std::max(maximum_, minimum_ + 1.0);

        if (!std::isfinite(upper_))
            throw std::runtime_error(
                "Failed to construct a finite height bound."
            );
    }

    Result solve() {
        const double lowerFinal = f_(points_.back());
        const auto final =
            minimizeGlobal(
                lowerFinal,
                upper_,
                [&](double q) {
                    return value(n_, static_cast<int>(points_.size()) - 1, q);
                }
            );

        if (!std::isfinite(final.value) || !std::isfinite(final.x))
            throw std::runtime_error(
                "No feasible continuous-height solution found."
            );

        std::vector<double> heights(n_ + 1);
        std::vector<int> indices(n_ + 1);

        heights[n_] = final.x;
        indices[n_] =
            static_cast<int>(points_.size()) - 1;

        for (int k = n_; k >= 1; --k) {
            const StateKey key{k, indices[k], heights[k]};
            const auto it = memo_.find(key);
            if (it == memo_.end() ||
                it->second.parentGrid < 0 ||
                !std::isfinite(it->second.parentHeight)) {
                throw std::runtime_error(
                    "Failed to reconstruct continuous-height solution."
                );
            }

            indices[k - 1] = it->second.parentGrid;
            heights[k - 1] = it->second.parentHeight;
        }

        std::vector<double> breakpoints(n_ + 1);
        std::vector<Segment> segments;
        segments.reserve(n_);

        double totalCost = 0.0;

        for (int k = 0; k <= n_; ++k)
            breakpoints[k] = points_[indices[k]];

        for (int k = 0; k < n_; ++k) {
            const double x0 = breakpoints[k];
            const double x1 = breakpoints[k + 1];
            const double y0 = heights[k];
            const double y1 = heights[k + 1];
            const double dx = x1 - x0;

            const double slope = (y1 - y0) / dx;
            const double intercept = y0 - slope * x0;

            const Transition transition =
                evaluateTransition(indices[k], indices[k + 1], y0);

            const double segmentIntegral =
                dx * (y0 + y1) / 2.0;

            const double segmentF =
                numerical::adaptiveIntegral(f_, x0, x1);

            const double cost =
                segmentIntegral - segmentF;

            segments.push_back({
                x0,
                x1,
                slope,
                intercept,
                cost,
                transition.contact
            });

            totalCost += cost;
        }

        if (totalCost < -1e-5)
            throw std::runtime_error(
                "Numerical result violates nonnegative majorant cost."
            );

        return {
            std::max(0.0, totalCost),
            std::move(breakpoints),
            std::move(segments)
        };
    }

private:
    struct SearchResult {
        double x;
        double value;
    };

    Transition evaluateTransition(
        int i,
        int j,
        double p
    ) const {
        const TransitionKey key{i, j, p};
        const auto cached = transitionCache_.find(key);
        if (cached != transitionCache_.end())
            return cached->second;

        const double u = points_[i];
        const double v = points_[j];

        if (p < f_(u) - kHeightTolerance) {
            const Transition result{};
            transitionCache_.emplace(key, result);
            return result;
        }

        const double width = v - u;
        const double eps =
            std::max(1e-12 * width, 1e-12);
        const double lo = u + eps;

        auto ratio = [&](double x) {
            return (f_(x) - p) / (x - u);
        };

        double bestX = v;
        double bestRatio = ratio(v);

        for (int s = 1; s < kTransitionSamples; ++s) {
            const double x =
                lo + (v - lo) * s / kTransitionSamples;
            const double r = ratio(x);
            if (r > bestRatio) {
                bestRatio = r;
                bestX = x;
            }
        }

        const auto support =
            numerical::adaptiveSupportMaximum(
                ratio,
                lo,
                v,
                0.0,
                kTransitionSamples,
                kTransitionDepth,
                kTransitionRefinements
            );

        if (std::isfinite(support.value) &&
            support.value > bestRatio) {
            bestRatio = support.value;
            bestX = support.x;
        }

        const Transition result{
            p + width * bestRatio,
            bestX
        };
        transitionCache_.emplace(key, result);
        return result;
    }

    double feasibleLowerHeight(
        int i,
        int j,
        double q
    ) const {
        const LowerHeightKey key{i, j, q};
        const auto cached = lowerHeightCache_.find(key);
        if (cached != lowerHeightCache_.end())
            return cached->second;

        double lo = f_(points_[i]);
        double hi = upper_;

        if (evaluateTransition(i, j, hi).threshold > q) {
            const double result =
                std::numeric_limits<double>::infinity();
            lowerHeightCache_.emplace(key, result);
            return result;
        }

        if (evaluateTransition(i, j, lo).threshold <= q) {
            lowerHeightCache_.emplace(key, lo);
            return lo;
        }

        for (int it = 0; it < 18; ++it) {
            const double mid = (lo + hi) / 2.0;
            if (evaluateTransition(i, j, mid).threshold <= q)
                hi = mid;
            else
                lo = mid;
        }

        lowerHeightCache_.emplace(key, hi);
        return hi;
    }

    SearchResult minimizeGlobal(
        double lo,
        double hi,
        const std::function<double(double)>& objective
    ) {
        std::map<double, double> evaluations;
        const auto eval = [&](double x) {
            const auto it = evaluations.find(x);
            if (it != evaluations.end())
                return it->second;
            const double y = eval(x);
            evaluations.emplace(x, y);
            return y;
        };

        if (!(lo <= hi))
            return {lo, std::numeric_limits<double>::infinity()};

        if (hi - lo <= kHeightTolerance)
            return {lo, eval(lo)};

        std::vector<double> x(kGlobalSamples);
        std::vector<double> y(kGlobalSamples);

        for (int s = 0; s < kGlobalSamples; ++s) {
            x[s] =
                lo + (hi - lo) * s /
                (kGlobalSamples - 1);
            y[s] = eval(x[s]);
        }

        int best = 0;
        for (int s = 1; s < kGlobalSamples; ++s)
            if (y[s] < y[best])
                best = s;

        SearchResult result{x[best], y[best]};

        const int left =
            std::max(0, best - kLocalIntervals);
        const int right =
            std::min(kGlobalSamples - 1, best + kLocalIntervals);

        for (int s = left; s < right; ++s) {
            const double a = x[s];
            const double b = x[s + 1];

            const double fa = eval(a);
            const double fb = eval(b);

            // Golden-section is used only locally. The outer sampling
            // preserves the fact that the global value function need
            // not be convex.
            const double phi =
                (1.0 + std::sqrt(5.0)) / 2.0;

            double l = a;
            double r = b;
            double x1 = r - (r - l) / phi;
            double x2 = l + (r - l) / phi;
            double f1 = eval(x1);
            double f2 = eval(x2);

            for (int it = 0; it < kGoldenIterations; ++it) {
                if (f1 <= f2) {
                    r = x2;
                    x2 = x1;
                    f2 = f1;
                    x1 = r - (r - l) / phi;
                    f1 = eval(x1);
                } else {
                    l = x1;
                    x1 = x2;
                    f1 = f2;
                    x2 = l + (r - l) / phi;
                    f2 = eval(x2);
                }
            }

            const double candidateX =
                (l + r) / 2.0;
            const double candidateY =
                eval(candidateX);

            if (candidateY < result.value) {
                result = {candidateX, candidateY};
            }

            if (fa < result.value)
                result = {a, fa};
            if (fb < result.value)
                result = {b, fb};
        }

        return result;
    }

    double value(int k, int j, double q) {
        const StateKey key{k, j, q};
        const auto found = memo_.find(key);
        if (found != memo_.end())
            return found->second.value;

        if (j < k ||
            j > static_cast<int>(points_.size()) - 1 ||
            k < 0 || k > n_) {
            return std::numeric_limits<double>::infinity();
        }

        if (q < f_(points_[j]) - kHeightTolerance ||
            q < minimum_ - kHeightTolerance ||
            q > upper_ + kHeightTolerance) {
            return std::numeric_limits<double>::infinity();
        }

        if (k == 0) {
            const double result =
                j == 0 && q >= f_(points_.front()) - kHeightTolerance
                    ? 0.0
                    : std::numeric_limits<double>::infinity();

            memo_[key] = {result, -1, q};
            return result;
        }

        if (k == n_ &&
            j == static_cast<int>(points_.size()) - 1) {
            if (q < f_(points_.back()) - kHeightTolerance)
                return std::numeric_limits<double>::infinity();

            // This state represents the completed path. No extra cost
            // is added after the last segment.
        }

        StateValue best;

        for (int i = k - 1; i < j; ++i) {
            if (j - i < 1)
                continue;

            const double u = points_[i];
            const double v = points_[j];
            const double dx = v - u;

            const double pLo =
                feasibleLowerHeight(i, j, q);

            if (!std::isfinite(pLo) ||
                pLo > upper_ + kHeightTolerance)
                continue;

            const SearchResult inner =
                minimizeGlobal(
                    pLo,
                    upper_,
                    [&](double p) {
                        const double threshold =
                            evaluateTransition(i, j, p).threshold;

                        if (threshold > q + kHeightTolerance)
                            return std::numeric_limits<double>::infinity();

                        const double previous =
                            value(k - 1, i, p);

                        if (!std::isfinite(previous))
                            return std::numeric_limits<double>::infinity();

                        return previous + dx * (p + q) / 2.0;
                    }
                );

            if (inner.value < best.value) {
                best.value = inner.value;
                best.parentGrid = i;
                best.parentHeight = inner.x;
            }
        }

        memo_[key] = best;
        return best.value;
    }

    const Function& f_;
    const std::vector<double>& points_;
    int n_;

    double minimum_ = 0.0;
    double maximum_ = 0.0;
    double rho_ = 0.0;
    double upper_ = 0.0;

    std::map<StateKey, StateValue> memo_;
    mutable std::map<TransitionKey, Transition> transitionCache_;
    mutable std::map<LowerHeightKey, double> lowerHeightCache_;
};

} // namespace

Result solveGridDPOnGrid(
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

    ContinuousHeightDP dp(f, points, n);
    return dp.solve();
}

Result solveGridDP(
    const Function& f,
    double a,
    double b,
    int n,
    int N
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
        points[i] = a + (b - a) * i / N;

    return solveGridDPOnGrid(f, points, n);
}

} // namespace cover_curve::algorithms::adaptive_grid_dp
