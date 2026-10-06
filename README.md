# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a continuous curve.**

Given a continuous function $f:[a,b]\to\mathbb R$ and a segment budget $n\ge1$, the problem is to find a continuous piecewise-affine majorant $g\ge f$ with at most $n$ segments that minimizes

$$
E(g)=\int_a^b (g(x)-f(x))\,dx.
$$

The project separates the **exact mathematical problem** from the **finite numerical implementation**. The theory documents prove the statements that can be proved under the stated assumptions; implementation documents describe the approximations actually made by the C++ solvers.

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

The same $y_i$ is used by the two segments adjacent to an interior breakpoint, so continuity is built into the representation.

For fixed breakpoints, this is a convex linear semi-infinite program in the vertex heights. Free breakpoints make the outer problem nonconvex.

## What is proved

### Fixed breakpoints

For every strict breakpoint sequence, the feasible height set is nonempty, closed and convex, and the fixed-breakpoint optimum is attained.

The fixed-breakpoint problem has an exact continuous-height Bellman representation. The transition

$$
T_{u,v}(p)=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}
$$

satisfies

$$
q\ge T_{u,v}(p)
$$

exactly when the segment joining $(u,p)$ and $(v,q)$ is feasible, together with $p\ge f(u)$.

The vertex heights are continuous real variables. A finite height grid is **not** part of the mathematical formulation.

### Breakpoint-grid convergence

For a breakpoint grid $G_N$ with mesh

$$
\delta_N=\max_j(z_{j+1}-z_j),
$$

the exact grid-restricted optimum satisfies

$$
E_n^*\le E_{n,N}^*
$$

and, for an optimal spline with Lipschitz constant $K$,

$$
0\le E_{n,N}^*-E_n^*
\le
2(b-a)K\delta_N.
$$

Consequently,

$$
E_{n,N}^*\to E_n^*
\qquad
(\delta_N\to0).
$$

This is a theorem for the **exact continuous-height breakpoint-grid problem**. It is not a claim that a finite numerical run is globally optimal.

### Direct breakpoint-space search

The direct breakpoint-search theory proves global convergence under an exact fixed-breakpoint oracle and exhaustive subdivision of breakpoint space. The proof uses continuity of the fixed-breakpoint value function at a strict optimal breakpoint representation.

It does not use independently optimized one-segment costs as a branch-and-bound certificate, because those segment optima can have incompatible shared vertex heights.

### Coordinate search

Exact cyclic coordinate minimization is monotone decreasing and, under compactness, continuity and exact coordinate minimization assumptions, accumulation points are coordinatewise minima. This is a **local nonconvex result**, not a global-optimality theorem.

### Envelope/L-BFGS method

The envelope method is a local numerical method. Its sensitivity formula is justified only at differentiability points where the required primal-dual envelope/KKT assumptions hold. Active-set changes can make the value function nonsmooth.

The implementation therefore treats the computed sensitivity as a search direction and uses safeguarded line search rather than claiming a globally valid gradient.

### Curvature model

For constant-sign quadratic local models, the one-cell leading errors are

$$
\frac{q h^3}{12}
\quad(q>0),
\qquad
\frac{|q|h^3}{24}
\quad(q<0).
$$

This gives the cube-root curvature density

$$
w(x)\propto c(x)^{1/3}|f''(x)|^{1/3}.
$$

It is used for initialization. It is an asymptotic heuristic, **not** a finite-$n$ optimality theorem.

## Exact benchmark references

The theory includes exact finite-$n$ results for the quadratic benchmark functions on $[0,1]$:

$$
f(x)=x^2
\quad\Longrightarrow\quad
E_n^*=\frac{1}{6n^2},
$$

and

$$
f(x)=-x^2
\quad\Longrightarrow\quad
E_n^*=\frac{1}{12n^2}.
$$

These provide useful regression and convergence references for the numerical solvers.

The theory also records asymptotic curvature-density reference formulas for smoother examples such as $x^4$ and $\sin x$. Those formulas are benchmark references, not finite-run error certificates.

## Solvers

| Solver | Purpose | Mathematical status |
|---|---|---|
| adaptiveGridDP | breakpoint-grid dynamic programming with continuous heights | exact target on a fixed grid; grid-convergence theorem |
| fastGridDP | optimized implementation of the same target | same mathematical target; numerical approximation |
| directHeight | fixed-breakpoint continuous-height cutting-plane solver | exact/conditional theory; numerical separation in practice |
| breakpointSearch | direct subdivision of breakpoint space | global convergence in exact-oracle model |
| coordinateSearch | cyclic breakpoint optimization | local coordinate descent |
| envelopeSQP | envelope sensitivity + safeguarded L-BFGS-style search | local numerical method |
| curvatureAdaptive | curvature-guided breakpoint initialization | heuristic/asymptotic acceleration |
| slope-minimization | independent one-segment reference calculation | diagnostic only |

<code>directHeight</code> is the shared fixed-breakpoint inner solver for the outer local methods. Independent one-segment optimization must not be substituted for this inner problem when $n>1$, because internal vertex heights have to be shared.

## Numerical correctness boundary

The exact theory assumes exact or certified oracles where required. The current implementation uses finite-precision arithmetic and numerical procedures including:

- numerical integration;
- finite cutting-plane iterations;
- numerical support/separation searches;
- finite continuous-height searches;
- finite breakpoint-search budgets;
- finite coordinate/L-BFGS iterations;
- finite-difference derivatives for the envelope method.

For a black-box continuous function, finite sampling alone cannot certify a global supremum on an unsampled interval. Therefore sampled feasibility is numerical evidence unless an independent certified separation bound is available.

The numerical error should be separated from the mathematical breakpoint error. Conceptually,

$$
|\widehat E-E_n^*|
\le
|\widehat E-E_{n,N}^*|
+
|E_{n,N}^*-E_n^*|.
$$

The second term has the proved $O(\delta_N)$ bound above under the stated assumptions. The first term contains implementation-specific numerical and optimization errors and is not automatically bounded by the mathematical convergence theorem.

## Documentation

### Theory

Start with [Problem](theory/problem.md), which fixes the notation and formal optimization problem.

Then read:

1. [Existence](theory/existence.md) — existence of optimal splines and the literature input used for free knots.
2. [Fixed-breakpoint](theory/fixed-breakpoint.md) — height formulation and linear semi-infinite programming.
3. [Dynamic programming](theory/dynamic-programming.md) — exact continuous-height Bellman formulation.
4. [Algorithm](theory/algorithm.md) — breakpoint-grid DP, correctness, convergence and error bound.
5. [Direct height](theory/direct-height.md) — cutting-plane/separation formulation.
6. [Breakpoint search](theory/breakpoint-search.md) — exhaustive breakpoint-space convergence.
7. [Coordinate search](theory/coordinate-search.md) — coordinatewise descent and limit-point result.
8. [Envelope-SQP](theory/envelope-sqp.md) — sensitivity derivation, local optimization and benchmark theorems.

The theory deliberately distinguishes proved results from numerical heuristics and conditional certificates.

### Implementation

[Implementation documentation](implementation/README.md) describes the actual C++ architecture, state representation, numerical procedures, tolerances, complexity and guarantee boundaries.

The implementation documentation includes separate descriptions of:

- continuous-height DP;
- numerical integration and transition evaluation;
- support maximization;
- breakpoint subdivision;
- coordinate search;
- envelope/L-BFGS-style optimization;
- curvature initialization;
- interpolation;
- one-segment slope minimization.

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Repository structure

~~~text
theory/          exact mathematical formulation and proofs
implementation/ concrete numerical algorithms and their limitations
math/            mathematical/solver support code
tests/           regression and numerical tests
web/             web-facing interface
~~~

## Markdown math convention

All repository documentation uses GitHub-compatible Markdown math:

- inline: $f(x)$
- display:

$$
f(x)=\frac{1}{2}x^2.
$$

Use only `$...# Cover Curve

**Numerical library for optimal continuous piecewise-linear upper approximation of a continuous curve.**

Given a continuous function $f:[a,b]\to\mathbb R$ and a segment budget $n\ge1$, the problem is to find a continuous piecewise-affine majorant $g\ge f$ with at most $n$ segments that minimizes

$$
E(g)=\int_a^b (g(x)-f(x))\,dx.
$$

The project separates the **exact mathematical problem** from the **finite numerical implementation**. The theory documents prove the statements that can be proved under the stated assumptions; implementation documents describe the approximations actually made by the C++ solvers.

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

The same $y_i$ is used by the two segments adjacent to an interior breakpoint, so continuity is built into the representation.

For fixed breakpoints, this is a convex linear semi-infinite program in the vertex heights. Free breakpoints make the outer problem nonconvex.

## What is proved

### Fixed breakpoints

For every strict breakpoint sequence, the feasible height set is nonempty, closed and convex, and the fixed-breakpoint optimum is attained.

The fixed-breakpoint problem has an exact continuous-height Bellman representation. The transition

$$
T_{u,v}(p)=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}
$$

satisfies

$$
q\ge T_{u,v}(p)
$$

exactly when the segment joining $(u,p)$ and $(v,q)$ is feasible, together with $p\ge f(u)$.

The vertex heights are continuous real variables. A finite height grid is **not** part of the mathematical formulation.

### Breakpoint-grid convergence

For a breakpoint grid $G_N$ with mesh

$$
\delta_N=\max_j(z_{j+1}-z_j),
$$

the exact grid-restricted optimum satisfies

$$
E_n^*\le E_{n,N}^*
$$

and, for an optimal spline with Lipschitz constant $K$,

$$
0\le E_{n,N}^*-E_n^*
\le
2(b-a)K\delta_N.
$$

Consequently,

$$
E_{n,N}^*\to E_n^*
\qquad
(\delta_N\to0).
$$

This is a theorem for the **exact continuous-height breakpoint-grid problem**. It is not a claim that a finite numerical run is globally optimal.

### Direct breakpoint-space search

The direct breakpoint-search theory proves global convergence under an exact fixed-breakpoint oracle and exhaustive subdivision of breakpoint space. The proof uses continuity of the fixed-breakpoint value function at a strict optimal breakpoint representation.

It does not use independently optimized one-segment costs as a branch-and-bound certificate, because those segment optima can have incompatible shared vertex heights.

### Coordinate search

Exact cyclic coordinate minimization is monotone decreasing and, under compactness, continuity and exact coordinate minimization assumptions, accumulation points are coordinatewise minima. This is a **local nonconvex result**, not a global-optimality theorem.

### Envelope/L-BFGS method

The envelope method is a local numerical method. Its sensitivity formula is justified only at differentiability points where the required primal-dual envelope/KKT assumptions hold. Active-set changes can make the value function nonsmooth.

The implementation therefore treats the computed sensitivity as a search direction and uses safeguarded line search rather than claiming a globally valid gradient.

### Curvature model

For constant-sign quadratic local models, the one-cell leading errors are

$$
\frac{q h^3}{12}
\quad(q>0),
\qquad
\frac{|q|h^3}{24}
\quad(q<0).
$$

This gives the cube-root curvature density

$$
w(x)\propto c(x)^{1/3}|f''(x)|^{1/3}.
$$

It is used for initialization. It is an asymptotic heuristic, **not** a finite-$n$ optimality theorem.

## Exact benchmark references

The theory includes exact finite-$n$ results for the quadratic benchmark functions on $[0,1]$:

$$
f(x)=x^2
\quad\Longrightarrow\quad
E_n^*=\frac{1}{6n^2},
$$

and

$$
f(x)=-x^2
\quad\Longrightarrow\quad
E_n^*=\frac{1}{12n^2}.
$$

These provide useful regression and convergence references for the numerical solvers.

The theory also records asymptotic curvature-density reference formulas for smoother examples such as $x^4$ and $\sin x$. Those formulas are benchmark references, not finite-run error certificates.

## Solvers

| Solver | Purpose | Mathematical status |
|---|---|---|
| adaptiveGridDP | breakpoint-grid dynamic programming with continuous heights | exact target on a fixed grid; grid-convergence theorem |
| fastGridDP | optimized implementation of the same target | same mathematical target; numerical approximation |
| directHeight | fixed-breakpoint continuous-height cutting-plane solver | exact/conditional theory; numerical separation in practice |
| breakpointSearch | direct subdivision of breakpoint space | global convergence in exact-oracle model |
| coordinateSearch | cyclic breakpoint optimization | local coordinate descent |
| envelopeSQP | envelope sensitivity + safeguarded L-BFGS-style search | local numerical method |
| curvatureAdaptive | curvature-guided breakpoint initialization | heuristic/asymptotic acceleration |
| slope-minimization | independent one-segment reference calculation | diagnostic only |

<code>directHeight</code> is the shared fixed-breakpoint inner solver for the outer local methods. Independent one-segment optimization must not be substituted for this inner problem when $n>1$, because internal vertex heights have to be shared.

## Numerical correctness boundary

The exact theory assumes exact or certified oracles where required. The current implementation uses finite-precision arithmetic and numerical procedures including:

- numerical integration;
- finite cutting-plane iterations;
- numerical support/separation searches;
- finite continuous-height searches;
- finite breakpoint-search budgets;
- finite coordinate/L-BFGS iterations;
- finite-difference derivatives for the envelope method.

For a black-box continuous function, finite sampling alone cannot certify a global supremum on an unsampled interval. Therefore sampled feasibility is numerical evidence unless an independent certified separation bound is available.

The numerical error should be separated from the mathematical breakpoint error. Conceptually,

$$
|\widehat E-E_n^*|
\le
|\widehat E-E_{n,N}^*|
+
|E_{n,N}^*-E_n^*|.
$$

The second term has the proved $O(\delta_N)$ bound above under the stated assumptions. The first term contains implementation-specific numerical and optimization errors and is not automatically bounded by the mathematical convergence theorem.

## Documentation

### Theory

Start with [Problem](theory/problem.md), which fixes the notation and formal optimization problem.

Then read:

1. [Existence](theory/existence.md) — existence of optimal splines and the literature input used for free knots.
2. [Fixed-breakpoint](theory/fixed-breakpoint.md) — height formulation and linear semi-infinite programming.
3. [Dynamic programming](theory/dynamic-programming.md) — exact continuous-height Bellman formulation.
4. [Algorithm](theory/algorithm.md) — breakpoint-grid DP, correctness, convergence and error bound.
5. [Direct height](theory/direct-height.md) — cutting-plane/separation formulation.
6. [Breakpoint search](theory/breakpoint-search.md) — exhaustive breakpoint-space convergence.
7. [Coordinate search](theory/coordinate-search.md) — coordinatewise descent and limit-point result.
8. [Envelope-SQP](theory/envelope-sqp.md) — sensitivity derivation, local optimization and benchmark theorems.

The theory deliberately distinguishes proved results from numerical heuristics and conditional certificates.

### Implementation

[Implementation documentation](implementation/README.md) describes the actual C++ architecture, state representation, numerical procedures, tolerances, complexity and guarantee boundaries.

The implementation documentation includes separate descriptions of:

- continuous-height DP;
- numerical integration and transition evaluation;
- support maximization;
- breakpoint subdivision;
- coordinate search;
- envelope/L-BFGS-style optimization;
- curvature initialization;
- interpolation;
- one-segment slope minimization.

## Build

~~~bash
cmake -S . -B build
cmake --build build
~~~

## Repository structure

~~~text
theory/          exact mathematical formulation and proofs
implementation/ concrete numerical algorithms and their limitations
math/            mathematical/solver support code
tests/           regression and numerical tests
web/             web-facing interface
~~~

## Markdown math convention

All repository documentation uses GitHub-compatible Markdown math:

- inline: $f(x)$
- display:

$$
f(x)=\frac{1}{2}x^2.
$$

 for inline math and `$...$` for display math. Keep display equations outside fenced code blocks.

## License

MIT
