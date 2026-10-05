# Cover Curve

**Numerical library for optimal piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, the problem is to find a continuous piecewise-linear function $g\ge f$ with exactly $n$ nondegenerate line segments while minimizing

```math
E(g)=\int_a^b(g(x)-f(x))\,dx.
```

The repository contains the C++ numerical implementation and separate documentation for the mathematical theory and implementation details. A browser interface will be added separately.

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

For the one-segment problem on $[u,v]$,

```math
C(u,v)
=
\min_{\substack{L\text{ affine}\\
L(x)\ge f(x)\ \forall x\in[u,v]}}
\int_u^v(L(x)-f(x))\,dx.
```

Writing $L(x)=\alpha+\beta x$, the smallest feasible intercept for a fixed slope is

```math
\alpha(\beta)=\max_{x\in[u,v]}(f(x)-\beta x).
```

Thus

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

For global optimization, the breakpoint objective is

```math
J(x_1,\ldots,x_{n-1})
=
\sum_{i=0}^{n-1}C(x_i,x_{i+1}).
```

A uniform breakpoint grid reduces this continuous problem to a finite dynamic program, and the grid is refined adaptively.

For the detailed mathematical derivations and convergence discussion, see the [theory documentation](theory/).

## Numerical implementation

The solver uses:

- adaptive numerical integration;
- numerical support maximization;
- one-dimensional convex slope minimization;
- dynamic programming over breakpoint grids;
- adaptive breakpoint-grid refinement.

The implementation details are documented separately so that the README remains focused on the library's purpose and public behavior.

See:

- [Numerical methods](implementation/numerical-methods.md)
- [Support maximization](implementation/support-maximization.md)
- [Slope minimization](implementation/slope-minimization.md)
- [Adaptive-grid DP](implementation/adaptive-grid-dp.md)
- [Interpolation](implementation/interpolation.md)

The support maximization is a numerical global-search heuristic for a general continuous black-box function. Finite sampling alone cannot certify a global maximum without additional assumptions on $f$.

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
├── theory/
│   ├── convergence.md
│   ├── dynamic-programming.md
│   ├── one-segment-cost.md
│   └── problem.md
└── implementation/
    ├── adaptive-grid-dp.md
    ├── interpolation.md
    ├── numerical-methods.md
    ├── slope-minimization.md
    └── support-maximization.md
```

## Status

The mathematical formulation, numerical components, C++ library structure, and adaptive grid-DP solver are currently implemented. Automated tests and the browser service are the next development stages.

## License

MIT License
