#pragma once

#include <cover_curve/types.hpp>
#include <cover_curve/solvers/direct_height.hpp>

namespace cover_curve {

struct EnvelopeSQPOptions {
    int maxIterations = 20;
    int memory = 5;
    int lineSearchSteps = 12;
    int seeds = 1;
    double gradientTolerance = 1e-6;
    double stepTolerance = 1e-8;
    double sufficientDecrease = 1e-4;
    double finiteDifferenceStep = 1e-6;
    bool includeFastGridSeed = false;
    DirectHeightOptions innerOptions;
};

struct EnvelopeSQPDetailedResult {
    Result result;
    std::vector<double> gradient;
    int iterations = 0;
    int startIndex = 0;
};

EnvelopeSQPDetailedResult envelopeSQPSolveDetailed(
    const Function& f,
    double a,
    double b,
    int n,
    const EnvelopeSQPOptions& options = {}
);

Result envelopeSQPSolve(
    const Function& f,
    double a,
    double b,
    int n,
    const EnvelopeSQPOptions& options = {}
);

}
