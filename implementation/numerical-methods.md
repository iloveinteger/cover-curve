# Numerical Methods

This document specifies numerical primitives used by the shared-height solver. It deliberately separates numerical approximation from the mathematical optimization defined in `theory/`.

## 1. Integration

The implementation needs numerical values of
$$
I(u,v)=\int_u^v f(x)\,dx.
$$

Adaptive Simpson quadrature is the current integration method. Integration error is independent of breakpoint discretization and should be reported separately.

## 2. Transition evaluation

The shared-height DP needs
$$
T_{u,v}(p)=\sup_{u<x\le v}\left[p+\frac{v-u}{x-u}(f(x)-p)\right].
$$

For sampled constraint points $S\subset(u,v]$, evaluate
$$
T_S(u,v;p)=\max_{x\in S}\left[p+\frac{v-u}{x-u}(f(x)-p)\right].
$$

The sampled maximum is a lower approximation to the exact supremum. The current baseline samples every breakpoint and every cell midpoint. It must not be treated as a proof of feasibility.

## 3. Support-search primitive

The existing support-search machinery for
$$
\max_{x\in[u,v]}(f(x)-\beta x)
$$
may remain as a reusable numerical primitive. It is useful for diagnostics and future transition evaluators, but it does not by itself make finite sampled constraints a global feasibility certificate.

For arbitrary continuous black-box input, finite sampling cannot certify a global maximum without additional information such as a modulus of continuity or a Lipschitz bound.

## 4. Numerical tolerances

The implementation should keep separate tolerances for:

- integration;
- transition/support search;
- breakpoint refinement;
- continuous-height optimization;
- feasibility checks.

A single tolerance should not represent all numerical errors.

## 5. Feasibility checks

A candidate transition is accepted only according to the selected numerical policy for
$$
q\ge T_{u,v}(p).
$$

If the method provides only sampled constraints, the result is a numerical candidate rather than a certified majorant. The implementation should expose this distinction instead of reporting sampled feasibility as mathematical feasibility.

## 6. Numerical error versus mathematical convergence

The theorem in `theory/algorithm.md` concerns the exact breakpoint-grid problem followed by
$$
\delta_N\to0.
$$

It does not automatically cover fixed numerical tolerances or the current finite-height baseline. A final height-grid-free implementation must also control transition, optimization, and integration errors, or provide separate error bounds.
