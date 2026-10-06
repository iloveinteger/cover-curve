# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, find a continuous piecewise-linear majorant $g\\\ge f$ with $n$ segments minimizing

$$\nE(g)=\int_a^b (g(x)-f(x))\,dx.\n$$

## Problem

Choose shared breakpoints and vertex heights

$$\na=x_0<x_1<\cdots<x_n=b,\n\\\qquad y_i=g(x_i).\n$$

On each segment,

$$\nL_i(x)=\n\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i+\n\frac{x-x_i}{x_{i+1}-x_i}y_{i+1},\n$$

with

$$\nL_i(x)\\\ge f(x)\\\qquad (x\in[x_i,x_{i+1}]).\n$$

Shared vertex heights enforce continuity exactly.

## Solvers

| Solver | Role | Global guarantee |
| --- | --- | --- |
| adaptiveGridDP | baseline breakpoint-grid DP | grid-convergence theorem in exact-oracle model |
| fastGridDP | optimized implementation of the same recurrence | same mathematical target |
| curvatureAdaptive | curvature-guided initialization | heuristic |
| breakpointSearch | direct breakpoint-space exhaustive subdivision | global convergence in exact-oracle model |
| coordinateSearch | cyclic coordinate optimization | coordinatewise descent only |
| envelopeSQP | envelope sensitivity + safeguarded L-BFGS-style search | local numerical method |

`directHeight` is the fixed-breakpoint continuous-height inner solver used by the outer methods.

## Documentation

- [Theory](theory/problem.md) — definitions, theorems, formulas, proofs, pseudocode, complexity, convergence, correctness, and error terms.
- [Implementation](implementation/README.md) — concrete C++/numerical realization, tolerances, data flow, and numerical limitations.

Start with [theory/problem.md](theory/problem.md), then [theory/fixed-breakpoint.md](theory/fixed-breakpoint.md), [theory/dynamic-programming.md](theory/dynamic-programming.md), and [theory/algorithm.md](theory/algorithm.md).

## Numerical correctness boundary

The exact theorems assume exact or certified numerical oracles. The current black-box implementation uses floating-point arithmetic, finite cutting-plane budgets, numerical support searches, numerical integration, and finite outer-search budgets. Therefore a returned value is a numerical result, not automatically a global certificate.

Curvature-based initialization is an asymptotic acceleration heuristic, not a finite-n correctness theorem.

## Build

    cmake -S . -B build
    cmake --build build

## License

MIT
