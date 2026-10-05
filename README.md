# Cover Curve

**Optimal piecewise-linear upper approximation of a curve.**

Given a continuous function $f:[a,b]\to\mathbb R$, approximate it from above by exactly $n$ nondegenerate line segments while minimizing the area between the approximation and the graph of $f$.

## Mathematical formulation

We seek a continuous piecewise-linear function $g:[a,b]\to\mathbb R$ with exactly $n$ nondegenerate segments such that

```math
g(x)\ge f(x)
\qquad\text{for all }x\in[a,b].
```

The objective is to minimize

```math
E(g)
=
\int_a^b(g(x)-f(x))\,dx.
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

is convex, since it is the pointwise maximum of affine functions of $\beta$. Hence the one-dimensional optimization problem is convex.

For continuous $f$, the one-segment cost $C(u,v)$ is continuous for $u<v$. It also has the continuous extension

```math
C(u,u)=0.
```

## Global optimization

For breakpoints

```math
a=x_0\le x_1\le\cdots\le x_n=b,
```

define the total cost

```math
J(x_1,\ldots,x_{n-1})
=
\sum_{i=0}^{n-1}C(x_i,x_{i+1}).
```

The closed breakpoint region is compact, and $J$ is continuous on it. Therefore a global minimizer exists.

Allowing coincident breakpoints only introduces zero-length segments with cost $C(u,u)=0$. Such segments can be removed, while any positive-length segment can be subdivided into additional nondegenerate segments without changing the piecewise-linear function or its cost. Thus the same optimal value is obtained for the original problem requiring exactly $n$ nondegenerate segments.

The optimal value is therefore

```math
E_n^*
=
\min_{a=x_0\le x_1\le\cdots\le x_n=b}
\sum_{i=0}^{n-1}C(x_i,x_{i+1}).
```

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

all breakpoint positions are restricted to $G_N$, while each one-segment cost $C(x_i,x_j)$ is still optimized over the full continuous interval $[x_i,x_j]$.

The finite breakpoint problem is solved by dynamic programming.

Let $F[k][j]$ be the minimum cost of covering the interval from $x_0=a$ to $x_j$ using exactly $k$ segments. Initialize

```math
F[0][0]=0,
\qquad
F[0][j]=+\infty\quad(j>0).
```

The recurrence is

```math
F[k][j]
=
\min_{i=k-1,\ldots,j-1}
\left\{
F[k-1][i]+C(x_i,x_j)
\right\}.
```

Thus

```math
E_{n,N}=F[n][N].
```

The predecessor index can be stored to recover the optimal breakpoint positions.

The dynamic program is exact for the supplied one-segment costs $C(x_i,x_j)$. The reference implementation computes these costs numerically using a one-dimensional search, so the practical result is a numerical approximation to the mathematical optimum.

## Grid refinement and convergence

Every grid-feasible breakpoint configuration is also feasible for the continuous problem, so

```math
E_n^*\le E_{n,N}.
```

Conversely, take any feasible continuous breakpoint configuration

```math
a<x_1<\cdots<x_{n-1}<b.
```

Its breakpoints can be approximated arbitrarily closely by grid points while preserving their strict ordering. Since $C$ is continuous, the corresponding total cost converges to the original cost.

Therefore, for every $\varepsilon>0$, a sufficiently fine grid contains a feasible breakpoint configuration whose cost is less than $E_n^*+\varepsilon$. Hence

```math
E_n^*
\le
E_{n,N}
<
E_n^*+\varepsilon
```

for all sufficiently large $N$.

It follows that

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

MIT License
