# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\\to\\mathbb R$, find a continuous piecewise-linear majorant $g\\ge f$ with $n$ segments minimizing

$$
E(g)=\\int_a^b (g(x)-f(x))\\,dx.
$$

## Problem

Choose shared breakpoints and vertex heights

$$
a=x_0<x_1<\\cdots<x_n=b,
\\qquad y_i=g(x_i).
$$

On each segment,

$$
L_i(x)=
\\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i+
\\frac{x-x_i}{x_{i+1}-x_i}y_{i+1},
$$

with

$$
L_i(x)\\ge f(x)\\qquad (x\\in[x_i,x_{i+1}]).
$$

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

- [Theory](theory/README.md) — definitions, theorems, formulas, proofs, pseudocode, complexity, convergence, correctness, and error terms.
- [Implementation](implementation/README.md) — concrete C++/numerical realization, tolerances, data flow, and numerical limitations.

Start with theory/problem.md, then theory/fixed-breakpoint.md, theory/dynamic-programming.md, and theory/algorithm.md.

## Numerical correctness boundary

The exact theorems assume exact or certified numerical oracles. The current black-box implementation uses floating-point arithmetic, finite cutting-plane budgets, numerical support searches, numerical integration, and finite outer-search budgets. Therefore a returned value is a numerical result, not automatically a global certificate.

Curvature-based initialization is an asymptotic acceleration heuristic, not a finite-n correctness theorem.

## Build

    cmake -S . -B build
    cmake --build build

## License

MIT
