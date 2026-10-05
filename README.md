# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, the target problem is to find a continuous piecewise-linear function $g\ge f$ with exactly $n$ nondegenerate line segments while minimizing
\[ E(g)=\int_a^b(g(x)-f(x))\,dx. \]

## Mathematical formulation

The breakpoints are $a=x_0<x_1<\cdots<x_n=b$, with shared vertex heights $y_i=g(x_i)$. Each segment is
\[ L_i(x)=y_i+\frac{y_{i+1}-y_i}{x_{i+1}-x_i}(x-x_i). \]
This representation enforces continuity automatically: $L_i(x_i)=y_i=L_{i-1}(x_i)$.

Every segment must satisfy $L_i(x)\ge f(x)$ throughout its interval. For fixed breakpoints, minimizing the total area over the shared heights is a linear semi-infinite program.

## Independent one-segment relaxation

The repository also contains the `oneSegmentCost()` routine. It computes
\[ C_{\mathrm{ind}}(u,v)=\min_{L\text{ affine},\ L\ge f}\int_u^v(L-f). \]
This is the exact independent one-segment relaxation. It is useful as a reference and lower bound, but summing these costs does not enforce continuity between neighboring segments.

The old scalar DP based on $\sum_i C_{\mathrm{ind}}(x_i,x_{i+1})$ therefore is not an exact solver for the continuous target problem.

## Theory

The theory documents distinguish the continuous shared-height formulation, the independent relaxation, finite-grid optimization with continuity, convergence of the coupled problem, and curvature-based candidate-grid heuristics.

See the [theory documentation](theory/).

## Numerical implementation

The numerical components include adaptive integration, support maximization, slope minimization for the independent relaxation, breakpoint candidate generation, and the shared-height coupled optimization under development.

Finite support sampling and numerical optimization are separate numerical approximations from the mathematical grid-discretization problem.

## Curvature-adaptive grids

The experimental curvature strategy uses $\rho(x)\propto\sqrt{|f''(x)|}$ as a candidate-grid heuristic derived from a local quadratic model.

This density is not currently claimed to be the exact optimal allocation for the coupled continuous problem. A rigorous curvature theorem must be derived from the shared-height formulation itself.

## Status

The repository is being migrated from the old independent-segment DP to the mathematically correct continuous formulation. The `fix/continuous-segments` branch contains the transition work; post-processing that merely lifts breakpoint heights is not considered a final solution.

## Build

    cmake -S . -B build
    cmake --build build

The numerical library is built as the `cover_curve` target.

## License

MIT License