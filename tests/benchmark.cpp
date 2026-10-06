#include <cover_curve/cover_curve.hpp>

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

using Clock = std::chrono::steady_clock;

template <class Solver>
double measure(Solver solver, const cover_curve::Function& f,
               double a, double b, int n, double& value) {
    const auto begin = Clock::now();
    value = solver(f, a, b, n).value;
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count();
}

int main() {
    const auto x2 = [](double x) { return x * x; };
    const auto sinf = [](double x) { return std::sin(x); };

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "case,fast_ms,adaptive_ms,direct_ms,abs_fast_direct\n";

    for (const auto& c : {
        std::pair<const char*, cover_curve::Function>{"x2_n1", x2},
        {"sin_n2", sinf}
    }) {
        const double a = 0.0;
        const double b = c.first[0] == 'x' ? 1.0 : 2.0 * 3.141592653589793;
        const int n = c.first[0] == 'x' ? 1 : 2;

        double fv=0, av=0, dv=0;
        const double fm = measure(
            [](const auto& f,double a,double b,int n){
                return cover_curve::fastGridDP(f,a,b,n);
            }, c.second,a,b,n,fv);
        const double am = measure(
            [](const auto& f,double a,double b,int n){
                return cover_curve::adaptiveGridDP(f,a,b,n);
            }, c.second,a,b,n,av);
        const double dm = measure(
            [](const auto& f,double a,double b,int n){
                return cover_curve::directHeightSolve(f,a,b,n);
            }, c.second,a,b,n,dv);

        std::cout << c.first << ',' << fm << ',' << am << ',' << dm << ','
                  << std::abs(fv-dv) << '\n';
    }
}
