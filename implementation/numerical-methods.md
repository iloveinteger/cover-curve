# Numerical Methods

This document specifies numerical primitives used by the shared-height solver. It deliberately separates numerical approximation from the mathematical optimization defined in `theory/`.

## 1. Integration

The implementation needs numerical values of
\[
I(u,v)=\int_u^v f(x)\,dx.
\]

Adaptive Simpson quadrature is the current integration method. For an interval ([u,v]),
\[
S(u,v)=\frac{v-u}{6}
\left(
f(u)+4f\left(\frac{u+v}{2}\right)+f(v)
\right).
\]

The implementation recursively subdivides an interval when the estimated quadrature error exceeds the requested tolerance. The accepted estimate uses the standard Simpson correction.

Integration error is independent of breakpoint and height discretization error and should therefore be reported separately.

## 2. Transition evaluation

The shared-height DP needs
\[
T_{u,v}(p)
=
\sup_{u<x\le v}
\left[
p+\frac{v-u}{x-u}(f(x)-p)
\right].
\]

This is not the same numerical problem as the old independent one-segment slope minimization.

For sampled constraint points (S\subset(u,v]), evaluate
\[
T_S(u,v;p)
=
\max_{x\in S}
\left[
p+\frac{v-u}{x-u}(f(x)-p)
\right].
\]

The sampled maximum is a lower approximation to the exact supremum. It must not be silently treated as a proof of feasibility.

## 3. Support-search primitive

The existing support-search machinery for
\[
\max_{x\in[u,v]}(f(x)-\beta x)
\]
may remain as a reusable numerical primitive.

It is useful for independent one-segment diagnostics and for future alternative transition evaluators, but it is not the global DP objective.

For arbitrary continuous black-box input, finite sampling cannot certify a global maximum without additional information such as a modulus of continuity or a Lipschitz bound.

## 4. Numerical tolerances

The implementation should keep separate tolerances for:

- integration;
- transition/support search;
- breakpoint refinement;
- height-grid refinement;
- feasibility checks.

A single tolerance should not be used to represent all numerical errors.

For objective comparisons, a scale such as
\[
\max(1,|E_N|,|E_{2N}|)
\]
can be used to avoid unstable relative errors when the objective is close to zero.

## 5. Feasibility checks

A candidate transition is accepted only if its numerical constraint evaluation indicates
\[
q\ge T_{u,v}(p)
\]
within the selected numerical policy.

If the numerical method provides only sampled constraints, the result is a numerical candidate rather than a certified majorant. The implementation should expose this distinction instead of reporting sampled feasibility as mathematical feasibility.

## 6. Numerical error versus mathematical convergence

The theorem in `theory/algorithm.md` concerns the exact finite DP followed by
\[
\delta_N\to0,qquad \eta_N\to0.
\]

It does not automatically cover fixed numerical tolerances. A numerical implementation intended to approximate the theorem must also make the evaluation errors tend to zero, or provide a separate error bound.

