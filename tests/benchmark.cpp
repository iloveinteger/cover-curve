#include <cover_curve/cover_curve.hpp>

#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>

using Clock = std::chrono::steady_clock;

template <class Solver>
double measure(Solver solver, const cover_curve::Function& f,
               double a, double b, int n, int repeats, double& value) {
    value = solver(f, a, b, n).value;
    const auto begin = Clock::now();
    for (int i = 0; i < repeats; ++i)
        value = solver(f, a, b, n).value;
    const auto end = Clock::now();
    return std::chrono::duration<double, std::milli>(end - begin).count()
        / repeats;
}

struct Case {
    const char* name;
    cover_curve::Function f;
    double a;
    double b;
    int n;
};

int main() {
    constexpr int repeats = 2;
    constexpr double pi = 3.14159265358979323846;

    const Case cases[] = {
        {"sin", [](double x){ return std::sin(x); }, 0.0, 2*pi, 3},
        {"cos", [](double x){ return std::cos(x); }, 0.0, 2*pi, 3},
        {"quartic", [](double x){ return x*x*x*x-2*x*x+x; }, -1.0, 1.0, 3},
        {"sixth", [](double x){ return x*x*x*x*x*x-3*x*x*x+x; }, -1.0, 1.0, 3},
        {"piecewise", [](double x){ return x < 0 ? x*x : -x*x+1; }, -1.0, 1.0, 3},
        {"wavy", [](double x){ return x+std::sin(x); }, 0.0, 3.0, 2}
    };

    std::cout << std::fixed << std::setprecision(3);
    std::cout << "case,fast_ms,adaptive_ms,direct_ms,direct_over_fast,abs_fast_direct\n";

    for (const auto& c : cases) {
        double fv=0, av=0, dv=0;
        const double fm = measure([](const auto& f,double a,double b,int n){
            return cover_curve::fastGridDP(f,a,b,n);
        }, c.f,c.a,c.b,c.n,repeats,fv);
        const double am = measure([](const auto& f,double a,double b,int n){
            return cover_curve::adaptiveGridDP(f,a,b,n);
        }, c.f,c.a,c.b,c.n,repeats,av);
        const double dm = measure([](const auto& f,double a,double b,int n){
            return cover_curve::directHeightSolve(f,a,b,n);
        }, c.f,c.a,c.b,c.n,repeats,dv);

        std::cout << c.name << ',' << fm << ',' << am << ',' << dm << ','
                  << dm/fm << ',' << std::abs(fv-dv) << '\n';
    }

    return 0;
}
