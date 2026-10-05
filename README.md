# Cover Curve

**Numerical library for optimal piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, the problem is to find a continuous piecewise-linear function $g\ge f$ with exactly $n$ nondegenerate line segments while minimizing

```math
E(g)=\int_a^b(g(x)-f(x))\,dx.
```

The repository currently contains the C++ numerical implementation and mathematical documentation. A browser interface will be added separately.

## Mathematical formulation

We seek a continuous piecewise-linear function $g:[a,b]\to\mathbb R$ with exactly $n$ nondegenerate segments such that

```math
g(x)\ge f(x)
\qquad\text{for all }x\in[a,b].
```

The breakpoints are

```math
a=x_0<x_1<\cdots<x_n=b.
```

On each interval $[x_i,x_{i+1}]$, $g$ is affine and lies above $f$.

No differentiability of $f$ is required for the mathematical problem; continuity on $[a,b]$ is sufficient.

## One-segment cost

For an interval $[u,v]$, define

```math
C(u,v)
=
\min_{\substack{L\text{ affine}\\
L(x)\ge f(x)\ \forall x\in[u,v]}}
\int_u^v(L(x)-f(x))\,dx.
```

Writing $L(x)=\alpha+\beta x$, the smallest feasible intercept for a fixed slope $\beta$ is

```math
\alpha(\beta)=\max_{x\in[u,v]}(f(x)-\beta x).
```

Therefore

```math
C(u,v)
=
\min_{\beta\in\mathbb R}\left[
(v-u)\max_{x\in[u,v]}(f(x)-\beta x)
+\beta\frac{v^2-u^2}{2}
-\int_u^v f(x)\,dx
\right].
```

The support function $\beta\mapsto\max_x(f(x)-\beta x)$ is convex, so the resulting one-dimensional slope optimization is convex.

## Global optimization

For breakpoints $a=x_0\le x_1\le\cdots\le x_n=b$, define

```math
J(x_1,\ldots,x_{n-1})=\sum_{i=0}^{n-1}C(x_i,x_{i+1}).
```

The continuous extension $C(u,u)=0$ makes the relaxed breakpoint domain compact, and the objective is continuous. This gives existence of an optimum. Zero-length segments can be removed or positive-length segments subdivided without changing the represented piecewise-linear function or its cost, so the optimal value agrees with the formulation using exactly $n$ nondegenerate segments.

## Numerical method

Only the **breakpoint positions** are discretized.

For the uniform grid

```math
G_N=\left\{a+\frac{j(b-a)}{N}\;\middle|\;j=0,\ldots,N\right\},
```

breakpoints are restricted to $G_N$, while each one-segment cost is still optimized over the continuous interval between the selected grid points.

The finite problem is solved by dynamic programming:

```math
F[0][0]=0,\qquad F[0][j]=+\infty\quad(j>0),
```

and

```math
F[k][j]=\min_{i=k-1,\ldots,j-1}\{F[k-1][i]+C(x_i,x_j)\}.
```

The predecessor indices recover the selected breakpoints.

The implementation uses:

- adaptive Simpson integration;
- adaptive support maximization for $\max_x(f(x)-\beta x)$;
- convex one-dimensional slope minimization;
- dynamic programming over the breakpoint grid;
- adaptive grid refinement $N\to2N$ until the computed objective stabilizes.

The support maximization is a numerical global-search heuristic for a general continuous black-box function; finite sampling alone cannot certify a global maximum without additional assumptions on $f$.

## Interpolation

For sampled data, the library provides:

- piecewise-linear interpolation;
- natural cubic spline interpolation.

The core solver also accepts a generic callable representing $f(x)$ directly.

## Build

The project uses CMake and requires C++20.

```text
cmake -S . -B build
cmake --build build
```

The numerical library is built as the `cover_curve` target.

## Project structure

```text
cover-curve/
├── CMakeLists.txt
├── math/
│   ├── include/cover_curve/
│   │   ├── cover_curve.hpp
│   │   ├── interpolation.hpp
│   │   ├── types.hpp
│   │   └── solvers/
│   │       └── adaptive_grid_dp.hpp
│   └── src/
│       ├── algorithms/
│       │   └── adaptive_grid_dp/
│       ├── interpolation/
│       └── numerical/
└── theory/
    ├── convergence.md
    ├── dynamic-programming.md
    ├── one-segment-cost.md
    └── problem.md
```

## Status

The mathematical formulation, numerical components, C++ library structure, and adaptive grid-DP solver are currently implemented. Automated tests and the browser service are the next development stages.

## License

MIT License