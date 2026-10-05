# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, the target problem is to find a continuous piecewise-linear function $g\ge f$ with exactly $n$ line segments while minimizing

$
E(g)=\int_a^b(g(x)-f(x))\,dx.
$

## Mathematical formulation

Choose breakpoints

$
a=x_0<x_1<\cdots<x_n=b
$

and shared vertex heights $y_i=g(x_i)$. Each segment is

$
L_i(x)
=
\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}.
$

The shared vertex heights enforce continuity automatically. The constraint is

$
L_i(x)\ge f(x)
\qquad
(x\in[x_i,x_{i+1}]).
$

For fixed breakpoints, the problem is a linear semi-infinite program in the shared heights. The exact Bellman formulation uses the current breakpoint height as its state, so continuity is enforced inside the optimization rather than by post-processing.

## Theory

The theory is organized as one logical chain:

1. `theory/problem.md` — precise definition of the free-breakpoint majorant problem.
2. `theory/existence.md` — existence of an optimal spline.
3. `theory/fixed-breakpoint.md` — exact fixed-breakpoint formulation and existence.
4. `theory/dynamic-programming.md` — exact shared-height Bellman formulation.
5. `theory/algorithm.md` — finite breakpoint/height discretization, exact finite DP, and convergence to the original optimum.

The main convergence statement is

$
E_{n,N,\eta}^*\longrightarrow E_n^*
$

when the breakpoint-grid mesh and height-grid mesh both tend to zero.

The free-breakpoint existence theorem uses standard spline approximation results; the fixed-breakpoint and discretization arguments are proved directly in the repository.

## Numerical implementation

The implementation is separate from the mathematical convergence theorem. In particular, numerical evaluation of $f$, the support function

$
T_{u,v}(p)
=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u},
$

and the integral must eventually be controlled so that their numerical errors vanish under refinement.

The implementation should therefore be regarded as an approximation of the finite mathematical DP, not as a substitute for the shared-height formulation.

## Status

The theory has been reorganized around the exact continuous shared-height problem. The old independent one-segment-cost DP is not part of the mathematical solution, because independently optimized segments do not enforce continuity.

Curvature-based breakpoint heuristics are also not part of the current correctness theorem. They may be investigated later as numerical acceleration or grid-design heuristics, but they are not needed for the convergence result above.

## Build

    cmake -S . -B build
    cmake --build build

The numerical library is built as the `cover_curve` target.

## License

MIT License
