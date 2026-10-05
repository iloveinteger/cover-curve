# Cover Curve

**Optimal piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, approximate it from above by exactly $n$ nondegenerate line segments while minimizing the area between the approximation and the graph of $f$.

## Mathematical formulation

We seek a continuous piecewise-linear function $g:[a,b]\to\mathbb R$ with exactly $n$ nondegenerate segments such that

```math
g(x)\ge f(x)
\qquad\text{for all }x\in[a,b],
```

and minimize

```math
E(g)
=
\int_a^b (g(x)-f(x))\,dx.
```

The function $g$ is determined by breakpoints

```math
a=x_0<x_1<\cdots<x_n=b.
```

On each interval $[x_i,x_{i+1}]$, $g$ is affine and satisfies

```math
g(x)\ge f(x)
\qquad\text{for all }x\in[x_i,x_{i+1}].
```

No differentiability of $f$ is required; continuity on $[a,b]$ is sufficient.

## One-segment cost

For an interval $[u,v]$, define

```math
C(u,v)
=
\min_{\substack{L\text{ affine}\\
L(x)\ge f(x)\ \forall x\in[u,v]}}
\int_u^v(L(x)-f(x))\,dx.
```

Writing

```math
L(x)=\alpha+\beta x,
```

the smallest feasible intercept for a fixed slope $\beta$ is

```math
\alpha(\beta)
=
\max_{x\in[u,v]}(f(x)-\beta x).
```

Therefore

```math
C(u,v)
=
\min_{\beta\in\mathbb R}
\left[
(v-u)\max_{x\in[u,v]}(f(x)-\beta x)
+
\beta\frac{v^2-u^2}{2}
-
\int_u^v f(x)\,dx
\right].
```

The function

```math
\beta\mapsto\max_{x\in[u,v]}(f(x)-\beta x)
```

is convex, so the one-dimensional optimization problem is convex.

For continuous $f$, the one-segment cost $C(u,v)$ is continuous for $u\le v$ when we define

```math
C(u,u)=0.
```

Consequently, the total cost as a function of the breakpoint positions is continuous.

## Global optimization

The continuous breakpoint problem is

```math
E_n^*
=
\min
\sum_{i=0}^{n-1}C(x_i,x_{i+1}),
\qquad
a=x_0\le x_1\le\cdots\le x_n=b.
```

Zero-length intervals have cost $C(u,u)=0$. They may be removed, and positive-length intervals may be subdivided as necessary, so the same optimal value is obtained by requiring exactly $n$ nondegenerate segments.

The resulting optimization problem has a global minimizer because the closed breakpoint region is compact and the total cost is continuous.

## Numerical method

Only the **breakpoint positions** are discretized.

For

```math
G_N
=
\left\{
a+\frac{j(b-a)}{N}
\;\middle|\;
j=0,\ldots,N
\right\},
```

the one-segment problem $C(x_i,x_j)$ is still solved over the full continuous interval $[x_i,x_j]$.

The finite breakpoint problem is then solved by dynamic programming:

```math
F[k][j]
=
\min_{i<j}
\left(F[k-1][i]+C(x_i,x_j)\right),
```

with the usual feasibility restriction that enough grid points remain for the remaining segments.

Thus the optimal value of the finite-grid problem is

```math
E_{n,N}=F[n][N].
```

The dynamic program is exact for the given one-segment cost values $C(x_i,x_j). In the reference implementation, these costs are computed numerically, so the practical result is a numerical approximation to the mathematical optimum.

## Grid refinement and convergence

Because every grid-feasible breakpoint configuration is also feasible for the continuous problem,

```math
E_n^*\le E_{n,N}.
```

For every continuous feasible breakpoint configuration, its breakpoints can be approximated arbitrarily closely by grid points while preserving their strict ordering. Since the total cost is continuous, the corresponding grid cost converges to the original cost.

Therefore, for every continuous $f$,

```math
\boxed{E_{n,N}\longrightarrow E_n^*}
\qquad\text{as }N\to\infty.
```

Thus refining the breakpoint grid gives a convergent sequence of finite dynamic-programming problems for the original continuous optimization problem.

## Project status

This repository contains the initial reference implementation and an interactive browser demo.

The numerical implementation currently uses a robust one-dimensional search for the optimal supporting line on each interval. The breakpoint optimization is then performed globally by dynamic programming.

## Run the demo

The project is static JavaScript, so `docs/index.html` can be opened directly in a browser or served through GitHub Pages.

For local development, any static HTTP server works, for example:

```text
python -m http.server 8000 -d docs
```

Then open:

`http://localhost:8000`

## License

MIT License.
