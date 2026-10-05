# Optional Candidate-Grid Heuristics

This document describes optional breakpoint-grid heuristics. These heuristics are not part of the mathematical correctness theorem.

## 1. Role

The correctness-oriented solver is defined by the breakpoint grid, height grid, and shared-height DP in `adaptive-grid-dp.md`.

A heuristic may be used to choose a nonuniform candidate breakpoint grid before that DP is run. It must not change the shared-height optimization itself.

## 2. Curvature-based candidate grid

If additional smoothness information is available, one possible heuristic is based on
\[
w(x)=\sqrt{|f''(x)|}.
\]

The implementation may estimate this quantity numerically, form an approximate cumulative density, and place candidate breakpoints at approximately equal density increments.

This is only a candidate-generation heuristic. No claim is made here that this density is optimal for the coupled majorant problem.

## 3. Numerical safeguards

A numerical implementation may use:

- a positive floor for the estimated density;
- an upper cap;
- endpoint-specific finite-difference formulas;
- smoothing of noisy second-derivative estimates.

These are implementation choices, not theoretical constants.

## 4. Interaction with the exact architecture

After a candidate grid is generated, the solver must still:

1. construct the finite height grid;
2. optimize shared vertex heights;
3. enforce segment majorant constraints;
4. reconstruct one continuous piecewise-affine function.

The curvature heuristic must never replace the shared-height DP with independent segment optimization.

## 5. Status

This module is optional and heuristic. It does not establish global optimality, convergence, or a curvature-based asymptotic law. Any future theorem about curvature-adaptive grids must be proved separately before being incorporated into the correctness theory.
