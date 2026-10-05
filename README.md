# Cover Curve

**Optimal piecewise-linear upper approximation of a curve.**

Given a function $f:[a,b]\to\mathbb R$, approximate it from above by exactly $n$ line segments while minimizing the area between the boundary and the graph of $f$.

## Mathematical formulation

We seek a continuous piecewise-linear function $g$ with exactly $n$ segments such that

$$
g(x)\ge f(x)
\qquad (a\le x\le b)
$$

and minimize

$$
E(g)=\int_a^b (g(x)-f(x))\,dx.
$$

For sufficiently regular $f$, an optimum can be taken to have no vertical segments, so its breakpoints satisfy

$$
a=x_0<x_1<\cdots<x_n=b.
$$

For one interval define

$$
C(u,v)=
\min_{L\ge f\text{ on }[u,v]}
\int_u^v(L(x)-f(x))\,dx.
$$

Then the global problem is

$$
E_n^*=
\min_{a=x_0<\cdots<x_n=b}
\sum_{i=0}^{n-1}C(x_i,x_{i+1}).
$$

## Numerical method

Only the **breakpoint positions** are discretized.

For

$$
G_N=
\left\{
a+\frac{j(b-a)}{N}
:
j=0,\ldots,N
\right\},
$$

the one-segment problem $C(x_i,x_j)$ is still solved over the full continuous interval.

The finite breakpoint problem is then solved exactly by dynamic programming:

$$
F[k][j]
=
\min_{i<j}
\left(
F[k-1][i]+C(x_i,x_j)
\right).
$$

Thus

$$
E_{n,N}=F[n][N].
$$

The grid problems satisfy

$$
E_n^*\le E_{n,N}
$$

and, under standard regularity assumptions,

$$
\boxed{
E_{n,N}\longrightarrow E_n^*
}.
$$

This gives a principled grid-refinement method rather than a greedy search.

## Project status

This repository contains the initial reference implementation and an interactive browser demo.

The numerical implementation currently uses a one-dimensional numerical search for the optimal supporting line on each interval. The breakpoint optimization itself is solved globally by dynamic programming.

## Run the demo

The project is static JavaScript, so `docs/index.html` can be opened directly in a browser or served through GitHub Pages.

For local development, any static HTTP server works. For example:

```bash
python -m http.server 8000 -d docs
```

Then open:

`http://localhost:8000`

## License

MIT License.
