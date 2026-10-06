# Implementation

This directory documents the actual numerical implementation. Mathematical definitions, exact theorems, proofs, and asymptotic convergence statements belong in [theory/](../theory/problem.md).

## Architecture

| Document | Role | Status |
|---|---|---|
| [adaptive-grid-dp.md](adaptive-grid-dp.md) | continuous-height breakpoint-grid DP | baseline/reference |
| [numerical-methods.md](numerical-methods.md) | integration, transition evaluation, tolerances | shared primitives |
| [support-maximization.md](support-maximization.md) | black-box maximization | shared primitive |
| [interpolation.md](interpolation.md) | sampled-data interpolation | input layer |
| [breakpoint-search.md](breakpoint-search.md) | direct breakpoint-space subdivision | cross-validation |
| [coordinate-search.md](coordinate-search.md) | cyclic coordinate optimization | local solver |
| [envelope-sqp.md](envelope-sqp.md) | envelope sensitivity + safeguarded L-BFGS-style search | local solver |
| [curvature-adaptive.md](curvature-adaptive.md) | curvature-based breakpoint initialization | initialization |
| [slope-minimization.md](slope-minimization.md) | independent one-segment reference | diagnostic only |

## Common data flow

```text
function -> breakpoint proposal -> fixed-breakpoint solve -> numerical objective/feasibility -> outer search -> best candidate
```

The fixed-breakpoint solver is the common inner oracle. Outer methods must preserve shared internal vertex heights; they must not optimize each segment independently.

## Numerical boundary

The theory uses exact quantities such as

$$
T_{u,v}(p)=\sup_{u<x\le v}\frac{(v-u)f(x)-(v-x)p}{x-u}.
$$

The implementation replaces exact operations with floating-point arithmetic, finite sampling/refinement, finite LP iterations, and numerical quadrature. Therefore sampled feasibility is not a global certificate, finite budgets do not imply global optimality, and numerical errors must be separated from mathematical discretization error.

## Required documentation for each solver

Each implementation document should state:

1. interface and state representation;
2. numerical procedure;
3. stopping criteria and tolerances;
4. complexity and main bottlenecks;
5. guarantee/certification boundary.

## GitHub math syntax

Use `$...$` for inline math and `$$...$$` for display math. Do not use `\(...\)`, `\[...\]`, doubled command backslashes, or formulas inside fenced code blocks.