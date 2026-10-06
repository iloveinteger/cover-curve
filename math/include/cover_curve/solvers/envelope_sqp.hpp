#pragma once

#include <cover_curve/types.hpp>

namespace cover_curve {

struct EnvelopeSQPOptions {
    int maxIterations = 20;
    int memory = 5;
    int lineSearchSteps = 12;
    int seeds = 2;
    double gradientTolerance = 1e-6;
    double stepTolerance = 1e-8;
    double sufficientDecrease = 1e-4;
    double finiteDifferenceStep = 1e-6;
    bool includeFastGridSeed = true;
    DirectHeightOptions innerOptions;
};

Result envelopeSQPSolve(
    const Function& f,
    double a,
    double b,
    int n,
    const EnvelopeSQPOptions& options = {}
);

}
