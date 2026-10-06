#include <cover_curve/cover_curve.hpp>

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
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

int main() {
    const auto x2 = [](double x) { return x * x; };
    const auto sinf = [](double x) { return std::sin(x); };

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "case,fast_ms,adaptive_ms,direct_ms,envelope_ms,abs_fast_direct\n";

    for (const auto& c : {
        std::pair<const char*, cover_curve::Function>{"x2_n1", x2},
        {"sin_n2", sinf}
    }) {
        const double a = 0.0;
        const double b =
            c.first[0] == 'x' ? 1.0 : 2.0 * 3.141592653589793;
        const int n = c.first[0] == 'x' ? 1 : 2;

        double fv = 0.0;
        double av = 0.0;
        double dv = 0.0;
        double ev = 0.0;

        const double fm = measure(
            [](const auto& f,double a,double b,int n) {
                return cover_curve::fastGridDP(f,a,b,n);
            }, c.second,a,b,n,fv);

        const double am = measure(
            [](const auto& f,double a,double b,int n) {
                return cover_curve::adaptiveGridDP(f,a,b,n);
            }, c.second,a,b,n,av);

        std::cout << c.first << ',' << fm << ',' << am
                  << ",direct_start\n" << std::flush;

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
            std::cout << c.first << ",direct_error," << e.what() << '\n';
        }
    }

    const std::vector<double> fixedPoints{0.0, 3.141592653589793, 2.0 * 3.141592653589793};
    const auto fixedFunction = [](double x) { return std::sin(x); };
    double fixedFast = 0.0;
    double fixedDirect = 0.0;
    const double fixedFastMs = measureFixed(
        [](const auto& f, const auto& points, int n) {
            return cover_curve::fastGridDPOnGrid(f, points, n);
        },
        fixedFunction, fixedPoints, 2, fixedFast
    );
    const double fixedDirectMs = measureFixed(
        [](const auto& f, const auto& points, int) {
            return cover_curve::directHeightSolve(f, points, {});
        },
        fixedFunction, fixedPoints, 2, fixedDirect
    );
    std::cout << "fixed_sin_2_uniform," << fixedFastMs
              << ",NA," << fixedDirectMs
              << ",NA," << std::abs(fixedFast - fixedDirect) << '\n';
}
