# Envelope-SQP implementation

## Components

The solver is split into two numerical layers.

1. directHeightSolveDetailed
   - solves the fixed-breakpoint semi-infinite LP by cutting planes;
   - returns endpoint heights;
   - retains positive dual multipliers associated with sampled/contact constraints.

2. envelopeSQPSolve
   - treats breakpoints as the outer variables;
   - computes the Lagrangian envelope gradient from the fixed-x primal/dual solution;
   - uses an L-BFGS direction with a feasibility-preserving line search;
   - re-solves the fixed-x LP after each accepted breakpoint step.

The implementation is in:
- math/src/algorithms/direct_height/solver.cpp
- math/src/algorithms/envelope_sqp/solver.cpp

Public interfaces:
- math/include/cover_curve/solvers/direct_height.hpp
- math/include/cover_curve/solvers/envelope_sqp.hpp

## Why the solver is independent of fastGridDP

fastGridDP is useful as a global/coarse search method but its fixed-grid recurrence repeatedly performs expensive support searches. The envelope solver therefore defaults to a uniform breakpoint seed and does not call fastGridDP.

A coarse-DP seed remains available through EnvelopeSQPOptions::includeFastGridSeed, but it is deliberately disabled by default.

## Curvature-density initialization\n\nFor sufficiently fine partitions the proven asymptotic density is\n\\[\\rho(x)\\propto c(x)^{1/3}|f''(x)|^{1/3}.\\]\nThe implementation estimates the second derivative on 129 uniform samples, forms a cumulative density, and inverts it to obtain the second seed. A small density floor prevents zero curvature from producing degenerate cells. This is an initializer, not a finite-n optimality claim.\n\n## Outer iteration

For a current breakpoint vector x:

1. solve the fixed-x LP;
2. recover y and dual contact multipliers;
3. evaluate the envelope gradient;
4. compute an L-BFGS direction;
5. limit the step so breakpoint ordering is maintained;
6. backtrack until the LP objective decreases sufficiently;
7. update the L-BFGS history.

The LP is always re-solved at the accepted breakpoint vector, so the returned result remains a feasible fixed-breakpoint majorant subject to the direct-height separation tolerance.

## Numerical derivative

The continuous-function API only supplies f(x). The endpoint part of the envelope gradient requires f'(x_j), so the implementation currently obtains that term by a centered finite difference.

This does not finite-difference the whole value function. The expensive dependence of the LP optimum on x is handled analytically through the dual multipliers.

A future differentiable-function interface can replace this endpoint finite difference directly.

## Ordering constraint

Interior breakpoints must satisfy
\[
a<x_1<\cdots<x_{n-1}<b.
\]

The line search computes a feasible step limit and additionally checks the candidate vector before solving the LP. If the sufficient-decrease condition is not met, the step is halved.

## Dual contacts

directHeightDetailedResult::contacts contains the positive multipliers of retained cutting-plane constraints.

For a contact z in segment i, the interpolation weights are
\[
w_i=\frac{x_{i+1}-z}{x_{i+1}-x_i},
\qquad
w_{i+1}=\frac{z-x_i}{x_{i+1}-x_i}.
\]

These weights are used in the envelope derivative and in reconstruction of endpoint multipliers.

The contact list is numerical: it is the active set returned by the finite cutting-plane LP at the current tolerance. It is not a proof that every continuous active measure has been represented exactly.

## Validation

tests/envelope_sqp.cpp checks:
- compilation and linkage;
- the exact quadratic family at n=2;
- recovery of the uniform breakpoint 1/2;
- dense majorant feasibility;
- existence of nonzero dual contacts;
- continuous majorant feasibility for sin on [0,2pi].

The solver is also built in the normal C++ CI target.

## Performance interpretation

fastGridDP performs a global grid search and pays for many fixed-breakpoint states. Envelope-SQP instead performs a small number of fixed-breakpoint LP solves.

It is therefore expected to be advantageous when the number of breakpoints is small and a good local solution is sufficient, while fastGridDP remains useful when stronger global-search behavior is required.
