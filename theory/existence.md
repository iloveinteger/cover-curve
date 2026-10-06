# Existence of an optimal spline majorant

Let $f:[a,b]\to\mathbb R$ be continuous and let $n\ge1$. Define
\[
\mathcal S_n
=
\{g\in C[a,b]:
g\text{ is piecewise-affine with at most }n
\text{ nondegenerate segments}\}.
\]
Set
\[
J(g)=\int_a^b(g-f),
\qquad
\mathcal F_n=\{g\in\mathcal S_n:g\ge f\text{ on }[a,b]\}.
\]

The use of **at most** $n$ segments is essential when free breakpoints are
allowed: a sequence of breakpoints may coalesce, and the limiting spline
may then have fewer nondegenerate segments.

## Theorem 1 — existence

There exists $g^*\in\mathcal F_n$ such that
\[
J(g^*)=\min_{g\in\mathcal F_n}J(g).
\]

For fixed breakpoints this follows by an elementary finite-dimensional
compactness argument, given below. For free breakpoints, the existence
statement is a standard free-knot one-sided $L^1$ spline-approximation
result; it is used here as an external theorem rather than being inferred
from a false $L^1$ compactness claim.

### Free-knot existence theorem used here

A continuous target on a compact interval admits a best one-sided
$L^1$ approximation from the class of continuous piecewise-polynomial
splines of fixed degree with a bounded number of free knots. The class is
understood with coalescing knots allowed, equivalently with at most the
prescribed number of nondegenerate pieces.

For degree one, this gives exactly the existence statement above, because
for every feasible $g$,
\[
g-f\ge0
\]
and hence
\[
J(g)=\int_a^b|g-f|
=\|g-f\|_{L^1}.
\]

This is part of the classical free-knot spline approximation theory. The
free-knot existence result of Barrar and Loeb is the relevant compactness
result, while the one-sided $L^1$ existence theory supplies the
restricted-range formulation.

References:

- R. B. Barrar and H. L. Loeb, “Existence of best spline approximations
  with free knots,” *Journal of Mathematical Analysis and Applications*
  31 (1970), 383--390,
  DOI: 10.1016/0022-247X(70)90032-6.
- N. Richter-Dyn, “On Best Nonlinear Approximation in Sign-Monotone Norms
  and in Norms Induced by Inner Products,” *SIAM Journal on Numerical
  Analysis* 16 (1979), 612--622,
  DOI: 10.1137/0716046.
- Z. Ziegler, “One-sided $L^1$-approximation by splines of an arbitrary
  degree,” in *Approximations with Special Emphasis on Spline Functions*,
  Academic Press, 1969, pp. 405--413.

The cited results are used only for existence. No uniqueness statement is
needed here.

### Why the naive $L^1$ compactness argument is invalid

It is not enough to take a minimizing sequence and say that it is
$L^1$-bounded, hence has an $L^1$-convergent subsequence. A bounded sequence
in $L^1$ need not be relatively compact in $L^1$; narrow, high spikes give
a standard counterexample. Therefore the free-knot existence theorem must
supply the required compactness/attainment result. It cannot be replaced
by the assertion
\[
\{J(g_j)\}\text{ bounded}
\quad\Longrightarrow\quad
\{g_j\}\text{ has an }L^1\text{-convergent subsequence}.
\]

If $g_j\to g$ in $L^1$ and the limit is known to belong to the spline
class, then the majorant constraint is closed: from $g_j\ge f$ we obtain
$g\ge f$ almost everywhere, and continuity of $g-f$ then gives
\[
g(x)\ge f(x)\qquad\forall x\in[a,b].
\]
The issue is therefore the compactness and preservation of the free-knot
spline class, not the passage of the inequality itself.

## Theorem 2 — existence for fixed breakpoints

Fix
\[
a=x_0<x_1<\cdots<x_m=b,
\qquad m\le n.
\]
Among continuous piecewise-affine functions whose breakpoints are contained
in this fixed set, there exists a feasible minimizer.

### Proof

Write
\[
y_i=g(x_i),\qquad i=0,\ldots,m.
\]
The spline is uniquely determined by the vector
$y=(y_0,\ldots,y_m)$. The feasible set
\[
\mathcal C_X
=
\{y\in\mathbb R^{m+1}:g_y(x)\ge f(x)
\text{ for every }x\in[a,b]\}
\]
is closed, because for each $x$ the value $g_y(x)$ is a continuous affine
function of $y$.

On $[x_i,x_{i+1}]$,
\[
\int_{x_i}^{x_{i+1}}g_y(x)\,dx
=
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1}).
\]
Therefore
\[
J(g_y)
=
\sum_{i=0}^{m-1}
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
\int_a^b f(x)\,dx.
\]
Equivalently,
\[
J(g_y)
=
c_0y_0+c_my_m+\sum_{i=1}^{m-1}c_i y_i
-
\int_a^b f,
\]
where
\[
c_0=\frac{x_1-x_0}{2},\qquad
c_m=\frac{x_m-x_{m-1}}{2},
\]
and
\[
c_i=\frac{x_{i+1}-x_{i-1}}{2}
\qquad(1\le i\le m-1).
\]
All coefficients are strictly positive.

Feasibility implies
\[
y_i=g_y(x_i)\ge f(x_i)
\]
for every $i$. Thus no coordinate can tend to $-infty$ along the
feasible set. If any coordinate tends to $+infty$, the positivity of its
coefficient forces $J(g_y)\to+\infty$. Hence every sublevel set
\[
\{y\in\mathcal C_X:J(g_y)\le C\}
\]
is bounded. It is also closed, and therefore compact.

A minimizing sequence eventually lies in one such compact sublevel set.
By the Weierstrass theorem, $J$ attains its minimum on $\mathcal C_X$.
\[
\square
\]

## Breakpoint collisions

For free breakpoints, the strictly ordered parameter set
\[
a<x_1<\cdots<x_{m-1}<b
\]
is not compact. It is therefore incorrect to prove free-knot existence by
claiming that this open simplex is compact.

If
\[
x_i^{(j)}-x_{i-1}^{(j)}\to0,
\]
the two neighboring affine pieces may merge in the limit. The resulting
function simply has fewer nondegenerate segments. This is why the problem
is formulated with **at most $n$** segments.

No claim is made that an optimizer must use exactly $n$ effective segments.

## Uniqueness

No uniqueness is asserted. In particular, different breakpoint
representations can describe the same affine function, and free-knot
one-sided $L^1$ problems need not have a unique optimizer under the
assumptions used here.
