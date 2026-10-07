# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a continuous curve.**

Given a continuous function $f:[a,b]\to\mathbb R$ and a segment budget $n\ge1$, the problem is to find a continuous piecewise-affine majorant $g\ge f$ with at most $n$ segments that minimizes

$$
E(g)=\int_a^b (g(x)-f(x))\,dx.
$$

## Mathematical problem

Let

$$
a=x_0<x_1<\cdots<x_n=b,
\qquad
y_i=g(x_i).
$$

On each segment,

$$
L_i(x)=
\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i+
\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}.
$$

The constraints are

$$
L_i(x)\ge f(x)
\qquad
(x\in[x_i,x_{i+1}]).
$$

The same $y_i$ is used by adjacent segments, so continuity is built into the representation.

For fixed breakpoints, this is a convex linear semi-infinite program in the vertex heights. Free breakpoints make the outer problem nonconvex.

## Mathematical results

The theory separates exact mathematical statements from numerical approximations.

- **Existence:** an optimal continuous piecewise-affine majorant exists.
- **Fixed breakpoints:** the height problem is a linear semi-infinite program, with an attained optimum.
- **Dynamic programming:** the fixed-breakpoint problem and breakpoint-grid problem admit exact continuous-height Bellman formulations.
- **Breakpoint-grid convergence:** the exact grid-restricted optimum converges to the unrestricted optimum as the breakpoint mesh tends to zero, with the proved bound
  $$
  0\le E_{n,N}^*-E_n^*
  \le2(b-a)K\delta_N
  $$
  for an optimal spline with Lipschitz constant $K$.
- **Direct breakpoint search:** exhaustive subdivision converges globally in the exact-oracle model.
- **Coordinate search:** exact cyclic coordinate minimization is monotone and has coordinatewise-minimal accumulation points under the stated assumptions.
- **Envelope optimization:** the sensitivity formula is derived under the required differentiability and primal-dual envelope/KKT hypotheses. The practical method is a safeguarded L-BFGS-style local method.
- **Curvature model:** the cube-root curvature density is justified as an asymptotic initialization principle, not as a finite-$n$ optimality theorem.
- **Quadratic benchmarks:**
  $$
  f(x)=x^2\quad\Longrightarrow\quad E_n^*=\frac1{6n^2},
  $$
  $$
  f(x)=-x^2\quad\Longrightarrow\quad E_n^*=\frac1{12n^2}.
  $$

## Theory

The theory documents follow the mathematical structure of the project:

1. [Problem](theory/problem.md) — formal optimization problem.
2. [Existence](theory/existence.md) — existence of an optimal spline.
3. [Fixed-breakpoint](theory/fixed-breakpoint.md) — fixed-breakpoint height formulation.
4. [Dynamic programming](theory/dynamic-programming.md) — exact continuous-height Bellman formulation.
5. [Breakpoint-discretized DP](theory/breakpoint-grid/) — algorithm, analysis, error and complexity.
6. [Direct height](theory/direct-height/) — algorithm, analysis, error and complexity.
7. [Breakpoint search](theory/breakpoint-search/) — algorithm, analysis, error and complexity.
8. [Coordinate search](theory/coordinate-search/) — algorithm, analysis, error and complexity.
9. [Envelope optimization](theory/envelope-sqp/) — algorithm, analysis, error and complexity.

Each algorithm has its own directory containing four documents:

- `algorithm.md` — mathematical definition and pseudocode;
- `analysis.md` — coverage, assumptions, correctness and convergence;
- `error.md` — discretization, numerical and optimization error;
- `complexity.md` — time, space and oracle complexity.

Pure mathematical foundations remain directly under `theory/`.

The C++ source under math/ is the implementation itself. There is no separate implementation-documentation layer.

## Solvers

| Solver | Purpose | Mathematical status |
|---|---|---|
| adaptiveGridDP | breakpoint-grid dynamic programming with continuous heights | exact target on a fixed grid; grid-convergence theorem |
| fastGridDP | optimized implementation of the same target | numerical implementation of the same target |
| directHeight | fixed-breakpoint continuous-height cutting-plane solver | exact/conditional theory; numerical separation in practice |
| breakpointSearch | direct subdivision of breakpoint space | global convergence in the exact-oracle model |
| coordinateSearch | cyclic breakpoint optimization | local coordinate descent |
| envelopeSQP | envelope sensitivity + safeguarded L-BFGS-style search | local numerical method |
| curvatureAdaptive | curvature-guided breakpoint initialization | heuristic/asymptotic acceleration |
| slope-minimization | independent one-segment reference calculation | diagnostic only |

directHeight is the shared fixed-breakpoint inner solver for the outer local methods. Independent one-segment optimization is not a substitute for this shared-height problem when $n>1$.

## Numerical boundary

The mathematical theory uses exact or certified oracles where required. The implementation uses finite-precision arithmetic and numerical procedures such as numerical integration, finite cutting-plane iterations, numerical separation, finite optimization budgets, and finite-difference sensitivities.

For a black-box continuous function, finite sampling alone cannot certify a global supremum on an unsampled interval. Numerical feasibility is therefore evidence unless a certified separation bound is available.

For a numerical output $\widehat E$,

$$
|\widehat E-E_n^*|
\le
|\widehat E-V(X_{\mathrm{out}})|
+
|V(X_{\mathrm{out}})-E_n^*|.
$$

The first term is the numerical/inner error at the returned breakpoints. The second is the outer nonconvex optimization gap. The breakpoint-grid theorem gives an additional explicit discretization bound when a grid-based method is used.

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Repository structure

~~~text
theory/    mathematical definitions, algorithms, proofs and analysis
math/      C++ implementation
tests/     regression and numerical tests
web/       web-facing interface
~~~

## Markdown math convention

Use GitHub-compatible Markdown math:

- inline: $f(x)$
- display:

$$
f(x)=\frac12x^2.
$$

Keep display equations outside fenced code blocks.

## License

MIT
