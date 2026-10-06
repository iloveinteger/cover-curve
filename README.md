# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, the target problem is to find a continuous piecewise-linear function $g\ge f$ with exactly $n$ line segments while minimizing

$$
E(g)=\int_a^b(g(x)-f(x))\,dx.
$$

## Mathematical formulation

Choose breakpoints

$$
a=x_0<x_1<\cdots<x_n=b
$$

and shared vertex heights $y_i=g(x_i)$. Each segment is

$$
L_i(x)=\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i+\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}.
$$

The shared vertex heights enforce continuity automatically. The constraint is

$$
L_i(x)\ge f(x)\qquad(x\in[x_i,x_{i+1}]).
$$

For fixed breakpoints, the problem is a linear semi-infinite program in the shared heights. The exact Bellman formulation uses the current breakpoint height as its state, so continuity is enforced inside the optimization rather than by post-processing.

## Theory

The theory is organized as one logical chain:

1. `theory/problem.md` — precise definition of the free-breakpoint majorant problem.
2. `theory/existence.md` — existence of an optimal spline.
3. `theory/fixed-breakpoint.md` — exact fixed-breakpoint formulation and existence.
4. `theory/dynamic-programming.md` — exact continuous-height shared-height Bellman formulation.
5. `theory/algorithm.md` — breakpoint-only discretization, continuous-height DP, and convergence to the original optimum.
6. `theory/breakpoint-search.md` — direct non-convex breakpoint-space search, local continuity of the fixed-breakpoint value function, and exhaustive-subdivision convergence.

The breakpoint-grid convergence statement is

$$
E_{n,N}^*\longrightarrow E_n^*
$$

when the breakpoint-grid mesh tends to zero. No height-grid convergence parameter is required by the mathematical formulation.

The direct breakpoint-search theorem is different: it assumes an exact fixed-breakpoint oracle and proves convergence by exhaustive subdivision and continuity at an optimal strict breakpoint representation.

## Numerical implementation

The implementation is separate from the mathematical convergence theorem. In particular, numerical evaluation of $f$, the support function

$$
T_{u,v}(p)=\sup_{u<x\le v}\frac{(v-u)f(x)-(v-x)p}{x-u},
$$

and the integral must be controlled separately.

The current baseline implementations use continuous real-valued heights with numerical support and one-dimensional searches. They are therefore numerical approximations to the continuous-height theory, not global feasibility certificates for arbitrary continuous black-box functions.

The implementation specifications are:

- `implementation/adaptive-grid-dp.md` — continuous-height DP realization.
- `implementation/breakpoint-search.md` — direct adaptive breakpoint search.

## Status

The theory is organized around the exact continuous shared-height problem. The old independent one-segment-cost DP is not part of the mathematical solution, because independently optimized segments do not enforce continuity.

The independently optimized one-segment problem is still useful as a relaxation, but the new breakpoint-search convergence proof does **not** rely on that relaxation becoming exact as a breakpoint box shrinks.

Curvature-based breakpoint heuristics are also not part of the correctness theorem. They may be used as numerical acceleration or grid-design heuristics, but the mathematical convergence result needs only breakpoint meshes with vanishing mesh size.

## Solvers

The library currently contains:

- `adaptiveGridDP` — baseline continuous-height breakpoint-grid DP.
- `fastGridDP` — optimized implementation of the same mathematical recurrence.
- `curvatureAdaptive` — curvature-guided breakpoint-grid heuristic.
- `breakpointSearch` — direct deterministic breakpoint-space search with exhaustive subdivision.

The last solver is intended primarily for small $n$ and independent cross-validation of the DP family.

## Build

    cmake -S . -B build
    cmake --build build

The numerical library is built as the `cover_curve` target.

## License

MIT License
