# Curvature-adaptive solver implementation

## Solver role

The curvature-adaptive solver is a separate experimental solver. It does not modify the baseline adaptive_grid_dp algorithm.

Its pipeline is: estimate |f''|, convert it to a bounded density, integrate the density on a sampling mesh, invert the cumulative density to obtain candidate breakpoints, run finite-grid DP, and refine the grid until the objective stabilizes.

## Grid density

The theoretical target is w(x)=sqrt(|f''(x)|). The implementation samples this quantity at equally spaced points. A positive floor and an upper cap prevent zero-density intervals and extreme clustering.

Breakpoint x_i is obtained by approximately equal increments of the cumulative trapezoidal integral of w.

## Numerical second derivative

Interior samples use a symmetric finite difference:

D2 f(x) = [f(x+h)-2f(x)+f(x-h)]/h^2.

Endpoint samples use a second-order one-sided difference. The step is tied to the curvature sampling resolution.

## DP

The curvature grid is passed to a finite-grid DP with the same recurrence as the baseline:

F[k][j] = min_i (F[k-1][i] + C(x_i,x_j)).

The same one-segment cost routine is reused. Segment costs are computed once for each candidate pair and reused across DP layers.

## Refinement

Defaults are tolerance 1e-6, initial grid 32, maximum grid 1024, and 257 curvature samples. The stopping test is the same relative objective change used by the baseline.

## Numerical status

This solver is a heuristic grid-selection method followed by finite-grid DP. It does not claim global optimality for the continuous problem beyond the usual finite-grid formulation. The baseline remains the reference solver for comparison.
