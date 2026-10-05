# Numerical Methods

This document describes how the numerical components of Cover Curve are implemented. It is intentionally separate from `theory/`: the theory explains mathematical correctness and the present document explains computational choices.

## Integration

The library uses adaptive Simpson quadrature for integrals of the supplied callable.

For an interval $[u,v]$, Simpson's rule uses

```math
S(u,v)=\frac{v-u}{6}\left(f(u)+4f\left(\frac{u+v}{2}\right)+f(v)\right).
```

An interval is recursively subdivided when the difference between the parent Simpson estimate and the two child estimates is too large. The accepted estimate includes the usual Simpson error correction.

The implementation is designed for ordinary continuous numerical functions rather than symbolic integration.

## Support maximum

For a fixed slope $\beta$, the one-segment problem needs

```math
\max_{x\in[u,v]}(f(x)-\beta x).
```

The implementation first samples the interval, scores subintervals using endpoint/midpoint information, and recursively refines several promising regions. Importantly, regions that are not selected for immediate refinement are retained for later consideration; they are not discarded.

This is a global-search heuristic. For an arbitrary continuous black-box callable, finite evaluations cannot certify that the discovered maximum is the true global maximum unless additional regularity information such as a known Lipschitz bound is available.

## Slope minimization

For a fixed interval, the intercept is eliminated analytically:

```math
\alpha(\beta)=\max_x(f(x)-\beta x).
```

The remaining objective in $\beta$ is convex. The implementation therefore brackets a minimum and applies a one-dimensional golden-section search to the convex objective.

The support maximization is evaluated numerically at each slope candidate, so the computed objective is only an approximation to the exact convex objective.

## Numerical robustness

The implementation validates positive tolerances and grid-size constraints. Relative changes in the adaptive outer loop are normalized by

```math
\max(1,|E_N|,|E_{2N}|).
```

This avoids unstable relative errors when the objective is close to zero.

