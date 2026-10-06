#include "support_max.hpp"

#include <algorithm>
#include <cmath>
#include <vector>

namespace cover_curve::numerical {

namespace {

struct Interval {
    double a;
    double b;
    double fa;
    double fm;
    double fb;
};

struct ScoredInterval {
    Interval interval;
    double score;
};

}

SupportMaximum adaptiveSupportMaximum(
    const Function& f,
    double u,
    double v,
    double beta,
    int initialSamples,
    int maxDepth,
    int refinementCount,
    double seedX
) {
    if (u == v) {
        const double value = f(u) - beta * u;
        return {u, value};
    }

    if (u > v) {
        return adaptiveSupportMaximum(
            f,
            v,
            u,
            beta,
            initialSamples,
            maxDepth,
            refinementCount,
            seedX
        );
    }

    const auto g = [&](double x) {
        return f(x) - beta * x;
    };

    SupportMaximum best{
        u,
        g(u)
    };

    const double gv = g(v);

    if (std::isfinite(seedX) && seedX > u && seedX < v) {
        const double gs = g(seedX);
        if (gs > best.value)
            best = {seedX, gs};
    }

    if (gv > best.value) {
        best = {v, gv};
    }

    std::vector<Interval> intervals;
    intervals.reserve(initialSamples);

    for (int i = 0; i < initialSamples; ++i) {
        const double a =
            u + (v - u) * i / initialSamples;

        const double b =
            u + (v - u) * (i + 1) / initialSamples;

        const double m =
            (a + b) / 2.0;

        const double fa = g(a);
        const double fm = g(m);
        const double fb = g(b);

        if (fa > best.value) {
            best = {a, fa};
        }

        if (fm > best.value) {
            best = {m, fm};
        }

        if (fb > best.value) {
            best = {b, fb};
        }

        intervals.push_back({
            a,
            b,
            fa,
            fm,
            fb
        });
    }

    for (int depth = 0; depth < maxDepth; ++depth) {
        std::vector<ScoredInterval> scored;
        scored.reserve(intervals.size());

        for (const auto& interval : intervals) {
            const double endpointMaximum =
                std::max({
                    interval.fa,
                    interval.fm,
                    interval.fb
                });

            const double variation =
                std::abs(
                    interval.fa -
                    2.0 * interval.fm +
                    interval.fb
                );

            scored.push_back({
                interval,
                endpointMaximum + variation
            });
        }

        const int count =
            std::min(
                refinementCount,
                static_cast<int>(scored.size())
            );

        // Only the best 'count' intervals are refined. Full sorting is
        // unnecessary; nth_element gives the same selected set in linear
        // average time and avoids sorting every interval at every depth.
        if (count > 0 && count < static_cast<int>(scored.size())) {
            std::nth_element(
                scored.begin(),
                scored.begin() + count,
                scored.end(),
                [](const auto& x, const auto& y) {
                    return x.score > y.score;
                }
            );
        }

        std::vector<Interval> next;
        next.reserve(intervals.size() + count);

        // Keep unselected intervals: a global maximum may lie in any region.
        for (int i = count; i < static_cast<int>(scored.size()); ++i) {
            next.push_back(scored[i].interval);
        }

        for (int i = 0; i < count; ++i) {
            const auto& interval =
                scored[i].interval;

            const double a = interval.a;
            const double b = interval.b;
            const double m = (a + b) / 2.0;

            const double lm =
                (a + m) / 2.0;

            const double rm =
                (m + b) / 2.0;

            const double flm = g(lm);
            const double frm = g(rm);

            if (flm > best.value) {
                best = {lm, flm};
            }

            if (frm > best.value) {
                best = {rm, frm};
            }

            next.push_back({
                a,
                m,
                interval.fa,
                flm,
                interval.fm
            });

            next.push_back({
                m,
                b,
                interval.fm,
                frm,
                interval.fb
            });
        }

        intervals = std::move(next);

        if (intervals.empty()) {
            break;
        }
    }

    for (const auto& interval : intervals) {
        if (interval.fa > best.value) {
            best = {interval.a, interval.fa};
        }

        if (interval.fm > best.value) {
            best = {
                (interval.a + interval.b) / 2.0,
                interval.fm
            };
        }

        if (interval.fb > best.value) {
            best = {interval.b, interval.fb};
        }
    }

    return best;
}

}
