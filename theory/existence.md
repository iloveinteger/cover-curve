# Existence of an optimal spline majorant

We consider the optimization problem stated in [`problem.md`](../problem.md).

Let
$$
\mathcal S_n
=
\left\{
g\in C[a,b]:
g\text{ is piecewise-affine with at most }n
\text{ nondegenerate segments}
\right\},
$$

and define
$$
J(g)=\int_a^b(g-f),
\qquad
\mathcal F_n=\{g\in\mathcal S_n:g\ge f\text{ on }[a,b]\}.
$$

## Free-breakpoint problem

The full free-breakpoint existence statement is
$$
\exists\,g^*\in\mathcal F_n
\quad\text{such that}\quad
J(g^*)=\inf_{g\in\mathcal F_n}J(g).
$$

This statement is **not proved in this document**. In particular, we do not
invoke a general free-knot approximation theorem unless its hypotheses and
conclusion are verified to match the present one-sided problem exactly.

For a feasible $g$,
$$
g-f\ge0,
$$
so
$$
J(g)=\int_a^b|g-f|=\|g-f\|_{L^1}.
$$

Thus the problem is a one-sided $L^1$ approximation problem. However, an
existence theorem for ordinary best $L^1$ approximation by free-knot
splines does not by itself imply existence in the constrained majorant
class.

The following references establish relevant free-knot and one-sided spline
approximation results, but they are **not used here as a direct proof of the
combined free-knot majorant existence statement**:

- R. B. Barrar and H. L. Loeb, “Existence of best spline approximations
  with free knots,” *Journal of Mathematical Analysis and Applications*
  31 (1970), 383--390,
  DOI: 10.1016/0022-2476(70)90032-6.
- Z. Ziegler, “One-sided $L^1$-approximation by splines of an arbitrary
  degree,” in *Approximations with Special Emphasis on Spline Functions*,
  Academic Press, 1969, pp. 405--413.

A bounded objective also cannot be converted directly into $L^1$
compactness: bounded subsets of $L^1$ need not be relatively compact in
$L^1$. Consequently, a minimizing-sequence proof for free breakpoints needs
an additional compactness argument that preserves the spline class and the
majorant constraint.

## Theorem — fixed breakpoints

Fix
$$
a=x_0<x_1<\cdots<x_m=b,
\qquad m\le n.
$$

Among continuous piecewise-affine functions whose breakpoints are contained
in this fixed set, there exists a feasible minimizer.

### Proof

The feasible set is nonempty: the constant function
$$
g(x)\equiv \max_{x\in[a,b]}f(x)
$$
belongs to the class and satisfies $g\ge f$.

Write
$$
y_i=g(x_i),\qquad i=0,\ldots,m.
$$

The spline is uniquely determined by the vector
$y=(y_0,\ldots,y_m)$. The feasible set
$$
\mathcal C_X
=
\left\{
y\in\mathbb R^{m+1}:g_y(x)\ge f(x)
\text{ for every }x\in[a,b]
\right\}
$$
is closed, because for each $x$ the value $g_y(x)$ is a continuous affine
function of $y$.

On $[x_i,x_{i+1}]$,
$$
\int_{x_i}^{x_{i+1}}g_y(x)\,dx
=
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1}).
$$

Therefore
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
c_0y_0+c_my_m+\sum_{i=1}^{m-1}c_i y_i
-
\int_a^b f,
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
\qquad(1\le i\le m-1).
$$

All coefficients are strictly positive.

Feasibility implies
$$
y_i=g_y(x_i)\ge f(x_i)
$$
for every $i$. Thus no coordinate can tend to $-\infty$ along the
feasible set. If any coordinate tends to $+\infty$, the positivity of its
coefficient forces $J(g_y)\to+\infty$. Hence every sublevel set
$$
\{y\in\mathcal C_X:J(g_y)\le C\}
$$
is bounded. It is also closed, and therefore compact.

A minimizing sequence eventually lies in one such compact sublevel set.
By the Weierstrass theorem, $J$ attains its minimum on $\mathcal C_X$.
$$
\square
$$
