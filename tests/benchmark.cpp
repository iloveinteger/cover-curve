#include <cover_curve/cover_curve.hpp>
#include <cover_curve/solvers/envelope_sqp.hpp>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <utility>
#include <vector>

using Clock = std::chrono::steady_clock;

template <class Solver>
double measure(Solver solver, const cover_curve::Function& f,
               double a, double b, int n, double& value) {
    const auto begin = Clock::now();
    value = solver(f, a, b, n).value;
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

template <class Solver>
double measureFixed(Solver solver, const cover_curve::Function& f,
                    const std::vector<double>& points, int n, double& value) {
    const auto begin = Clock::now();
    value = solver(f, points, n).value;
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

double denseViolation(const cover_curve::Result& result,
                      const cover_curve::Function& f,
                      int samples = 20001) {
    double worst = 0.0;
    for (int k = 0; k < samples; ++k) {
        const double a = result.breakpoints.front();
        const double b = result.breakpoints.back();
        const double x = a + (b - a) * k / (samples - 1.0);
        auto it = std::upper_bound(result.breakpoints.begin(),
                                   result.breakpoints.end(), x);
        std::size_t i = it == result.breakpoints.begin()
            ? 0 : static_cast<std::size_t>(
                std::distance(result.breakpoints.begin(), it) - 1);
        if (i >= result.segments.size())
            i = result.segments.size() - 1;
        const auto& s = result.segments[i];
        worst = std::max(worst, f(x) - (s.slope * x + s.intercept));
    }
    return worst;
}

int main() {
    const auto x2 = [](double x) { return x * x; };
    const auto negx2 = [](double x) { return -x * x; };
    const auto x4 = [](double x) { return x * x * x * x; };
    const auto sinf = [](double x) { return std::sin(x); };

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "case,fast_ms,adaptive_ms,direct_ms,envelope_ms,abs_fast_direct\n";

    for (const auto& c : {
        std::pair<const char*, cover_curve::Function>{"x2_n1", x2},
        {"sin_n2", sinf}
    }) {
        const double a = 0.0;
        const double b = c.first[0] == 'x' ? 1.0 : 2.0 * 3.141592653589793;
        const int n = c.first[0] == 'x' ? 1 : 2;
        double fv = 0.0, av = 0.0, dv = 0.0, ev = 0.0;
        const double fm = measure(
            [](const auto& f,double a,double b,int n) {
                return cover_curve::fastGridDP(f,a,b,n);
            }, c.second,a,b,n,fv);
        const double am = measure(
            [](const auto& f,double a,double b,int n) {
                return cover_curve::adaptiveGridDP(f,a,b,n);
            }, c.second,a,b,n,av);
        try {
            const double dm = measure(
                [](const auto& f,double a,double b,int n) {
                    return cover_curve::directHeightSolve(f,a,b,n);
                }, c.second,a,b,n,dv);
            const double em = measure(
                [](const auto& f,double a,double b,int n) {
                    return cover_curve::envelopeSQPSolve(f,a,b,n);
                }, c.second,a,b,n,ev);
            std::cout << c.first << ',' << fm << ',' << am << ','
                      << dm << ',' << em << ',' << std::abs(fv-dv) << '\n';
        } catch (const std::exception& e) {
            std::cout << c.first << ",ERROR," << e.what() << '\n';
        }
    }

    const std::vector<double> fixedPoints{
        0.0, 3.141592653589793, 2.0 * 3.141592653589793
    };
    const auto fixedFunction = [](double x) { return std::sin(x); };
    double fixedFast = 0.0, fixedDirect = 0.0;
    const double fixedFastMs = measureFixed(
        [](const auto& f, const auto& points, int n) {
            return cover_curve::fastGridDPOnGrid(f, points, n);
        }, fixedFunction, fixedPoints, 2, fixedFast);
    const double fixedDirectMs = measureFixed(
        [](const auto& f, const auto& points, int) {
            return cover_curve::directHeightSolve(f, points, {});
        }, fixedFunction, fixedPoints, 2, fixedDirect);
    std::cout << "fixed_sin_2_uniform," << fixedFastMs
              << ",NA," << fixedDirectMs << ",NA,"
              << std::abs(fixedFast - fixedDirect) << '\n';

    // Large-n accuracy stress test; CI executes this benchmark on every main push.  x^2 and -x^2 have exact finite-n
    // optima 1/(6 n^2) and 1/(12 n^2), respectively.  x^4 is also checked
    // against its asymptotic constant 27/125.
    cover_curve::EnvelopeSQPOptions options;
    options.maxIterations = 12;
    options.seeds = 2;
    options.includeFastGridSeed = false;
    options.useCurvatureSeed = true;
    options.curvatureSamples = 129;
    options.gradientTolerance = 2e-5;
    options.stepTolerance = 1e-9;
    options.innerOptions.maxSweeps = 60;
    options.innerOptions.tolerance = 1e-9;

    std::cout << "large_n,func,n,value,expected_or_limit,rel_error,violation,ms\n";
    // Periodic mixed-curvature validation. For sin on [0,2pi], f'' changes
    // sign at pi; test feasibility and convergence to the mixed-curvature constant.
    std::cout << "large_n_periodic,func,n,value,n2_value,asymptotic,rel_error,violation,ms\n";
    const double halfSinIntegral = std::sqrt(3.141592653589793) *
        std::tgamma(2.0 / 3.0) / std::tgamma(7.0 / 6.0);
    const double mixedConstant = std::pow(
        halfSinIntegral * (std::cbrt(1.0 / 12.0) + std::cbrt(1.0 / 24.0)), 3.0);
    for (const int n : {8, 16, 32, 64, 128}) {
        const auto begin = Clock::now();
        const auto result =
            cover_curve::envelopeSQPSolve(sinf, 0.0, 2.0 * 3.141592653589793, n, options);
        const auto end = Clock::now();
        const double ms = std::chrono::duration<double, std::milli>(end - begin).count();
        const double n2value = result.value * n * n;
        const double rel = std::abs(n2value - mixedConstant) / mixedConstant;
        const double violation = denseViolation(result, sinf);
        std::cout << "large_n_periodic,sin," << n << ',' << result.value << ','
                  << n2value << ',' << mixedConstant << ',' << rel << ','
                  << violation << ',' << ms << '\n';
        if (!std::isfinite(result.value) || !std::isfinite(violation) || violation > 5e-5)
            throw std::runtime_error("large-n sin accuracy/feasibility check failed");
    }

    for (const auto& c : {
        std::pair<const char*, cover_curve::Function>{"x2", x2},
        {"-x2", negx2},
        {"x4", x4}
    }) {
        for (const int n : {8, 16, 32, 64, 128}) {
            const auto begin = Clock::now();
            const auto result =
                cover_curve::envelopeSQPSolve(c.second, 0.0, 1.0, n, options);
            const auto end = Clock::now();
            const double ms =
                std::chrono::duration<double, std::milli>(end - begin).count();

            double expected = 0.0;
            if (c.first[0] == 'x' && c.first[1] == '2')
                expected = 1.0 / (6.0 * n * n);
            else if (c.first[0] == '-')
                expected = 1.0 / (12.0 * n * n);
            else
                expected = 27.0 / (125.0 * n * n);

            const double rel = std::abs(result.value - expected) /
                std::max(expected, 1e-30);
            const double violation = denseViolation(result, c.second);
            std::cout << "large_n," << c.first << ',' << n << ','
                      << result.value << ',' << expected << ',' << rel
                      << ',' << violation << ',' << ms << '\n';

            if (!std::isfinite(result.value) ||
                !std::isfinite(violation) ||
                violation > 5e-5)
                throw std::runtime_error("large-n accuracy/feasibility check failed");
        }
    }
}
