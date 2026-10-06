# Existence of an optimal spline majorant

Let

$$
\mathcal S_n
=
\left\{
g\in C[a,b]:
g\text{ is piecewise-affine with at most }n\text{ nondegenerate segments}
\right\},
$$

and define

$$
\mathcal F_n
=
\left\{
g\in\mathcal S_n:g(x)\ge f(x)\text{ for all }x\in[a,b]
\right\},
$$

with objective

$$
J(g)=\int_a^b(g(x)-f(x))\,dx.
$$

## Fixed-breakpoint existence

The free-breakpoint problem is not needed to establish existence when the
breakpoints are fixed. The following result is the existence statement that
is proved here.

### Theorem

Fix

$$
a=x_0<x_1<\cdots<x_m=b,
\qquad m\le n.
$$

Among all continuous piecewise-affine functions whose breakpoints are
contained in

$$
X=\{x_0,\ldots,x_m\},
$$

there exists a feasible function minimizing $J$.

### Proof

For

$$
y=(y_0,\ldots,y_m)\in\mathbb R^{m+1},
$$

let $g_y$ be the unique continuous piecewise-affine function satisfying

$$
g_y(x_i)=y_i,
\qquad i=0,\ldots,m.
$$

The feasible parameter set is

$$
\mathcal C_X
=
\left\{
y\in\mathbb R^{m+1}:
g_y(x)\ge f(x)\text{ for all }x\in[a,b]
\right\}.
$$

It is nonempty because

$$
g_y(x)\equiv M,
\qquad
M=\max_{x\in[a,b]}f(x),
$$

is feasible.

For each fixed $x\in[a,b]$, the map

$$
y\longmapsto g_y(x)
$$

is affine and continuous. Hence $mathcal C_X$ is closed: if
$y^{(k)}\to y$ with $y^{(k)}\in\mathcal C_X$, then
$g_{y^{(k)}}(x)\to g_y(x)$ for every $x$, and therefore
$g_y(x)\ge f(x)$ for every $x$.

On each interval $[x_i,x_{i+1}]$,

$$
\int_{x_i}^{x_{i+1}}g_y(x)\,dx
=
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1}).
$$

Thus

$$
J(g_y)
=
\sum_{i=0}^{m-1}
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
\int_a^b f(x)\,dx.
$$

Equivalently,

$$
J(g_y)
=
\sum_{i=0}^{m}c_i y_i
-
\int_a^b f(x)\,dx,
$$

where

$$
c_0=\frac{x_1-x_0}{2},
\qquad
c_m=\frac{x_m-x_{m-1}}{2},
$$

and

$$
c_i=\frac{x_{i+1}-x_{i-1}}{2}
\qquad
(1\le i\le m-1).
$$

Every coefficient $c_i$ is strictly positive.

Feasibility at the breakpoints gives

$$
y_i=g_y(x_i)\ge f(x_i)
\qquad(i=0,\ldots,m).
$$

Therefore every feasible coordinate is bounded below. If a sequence in
$​\mathcal C_X$ has $J(g_y)\le C$, then the positive coefficients $c_i$ and
the lower bounds on all other coordinates give an upper bound for each
$y_i$. Hence every sublevel set

$$
\left\{
y\in\mathcal C_X:J(g_y)\le C
\right\}
$$

is bounded. It is closed because both $\mathcal C_X$ and $J$ are
continuous. Thus it is compact in $\mathbb R^{m+1}$.

Choose any feasible $y^{(0)}$. Every minimizing sequence eventually lies
in the compact sublevel set

$$
\left\{
y\in\mathcal C_X:J(g_y)\le J(g_{y^{(0)}})\right\}.
$$

By the Weierstrass theorem, $J(g_y)$ attains its minimum on this set.
Therefore a feasible minimizer exists.

$$
\square
$$

## Free breakpoints

The preceding theorem does **not** prove existence for the full free-
breakpoint class $\mathcal F_n$. In that case the knot locations vary, and
a minimizing sequence may contain intervals whose lengths tend to zero.
The fixed-breakpoint compactness argument therefore cannot simply be
extended by adding the knot locations as parameters.

In particular, boundedness of $J$ does not by itself give the required
compactness. Any proof of free-breakpoint existence must control degenerating
knot intervals and show that the limiting object remains a continuous
piecewise-affine majorant with at most $n$ segments.

We therefore do not state a free-breakpoint existence theorem here without
an independently verified theorem or a complete proof covering precisely
this one-sided constrained problem.
