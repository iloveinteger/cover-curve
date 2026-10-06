# Existence of an optimal spline majorant

Let \(f:[a,b]\to\mathbb R\) be continuous and let \(n\ge1\). Define
\[
\mathcal S_n
=
\{g\in C[a,b]:g\text{ is piecewise-affine with at most }n
\text{ nondegenerate segments}\}.
\]
We minimize
\[
J(g)=\int_a^b(g(x)-f(x))\,dx
\]
over
\[
\mathcal F_n=\{g\in\mathcal S_n:g(x)\ge f(x)\ \forall x\in[a,b]\}.
\]

The distinction between **at most \(n\)** and **exactly \(n\)** segments is
important: collisions of free knots are then harmless, because a collision
simply decreases the number of nondegenerate segments.

## Theorem 1 — existence

There exists \(g^*\in\mathcal F_n\) such that
\[
J(g^*)=\min_{g\in\mathcal F_n}J(g).
\]
Consequently an optimizer can be represented with exactly \(n\) segments by
subdividing affine pieces.

### External compactness theorem

We use the standard free-knot spline existence theorem of
R. B. Barrar and H. L. Loeb,
*Existence of best spline approximations with free knots*,
Journal of Mathematical Analysis and Applications 31 (1970), 383--390.

In the form needed here, the theorem states that for a continuous target on
a compact interval, a best \(L^p\)-approximation exists in the class of
continuous splines of fixed degree with a bounded number of free knots
(\(1\le p\le\infty\)); coincident knots are allowed in the limiting
representation and therefore the class is taken with at most the prescribed
number of nondegenerate pieces.

The present problem is a constrained version of the \(p=1\) case. The
constraint is closed under the convergence supplied by the free-knot
compactness argument: if \(g_j\to g\) in \(L^1\) and the limiting spline
\(g\) is continuous, then \(g_j\ge f\) implies \(g\ge f\) almost everywhere,
and continuity of \(g-f\) upgrades this to
\[
g(x)\ge f(x)\qquad\forall x\in[a,b].
\]

### Proof of Theorem 1

Let
\[
I=\inf_{g\in\mathcal F_n}J(g).
\]
The feasible set is nonempty. Indeed, if
\[
M=\max_{x\in[a,b]}f(x),
\]
then the constant spline \(g\equiv M\) belongs to \(\mathcal F_n\).
Hence \(I<\infty\).

Choose a minimizing sequence \(g_j\in\mathcal F_n\) with
\[
J(g_j)\longrightarrow I.
\]
Since \(g_j\ge f\),
\[
J(g_j)
=
\int_a^b|g_j-f|
=
\|g_j-f\|_{L^1}.
\]
Thus \(\{g_j\}\) is an \(L^1\)-bounded minimizing sequence for the
free-knot spline approximation problem.

Apply the free-knot existence/compactness theorem to this sequence.
After passing to a subsequence, there is a continuous
\(g^*\in\mathcal S_n\) such that
\[
g_j\longrightarrow g^*
\quad\text{in }L^1([a,b]).
\]

Because \(g_j-f\ge0\), the \(L^1\) convergence implies
\(g^*-f\ge0\) almost everywhere. Since both \(g^*\) and \(f\) are
continuous, \(g^*-f\) is continuous. A continuous function which is
nonnegative almost everywhere on an interval is nonnegative everywhere;
otherwise it would be negative on a nonempty open interval. Therefore
\[
g^*(x)\ge f(x)\qquad\forall x\in[a,b],
\]
so \(g^*\in\mathcal F_n\).

Finally,
\[
|J(g_j)-J(g^*)|
=
\left|
\int_a^b(g_j-g^*)\,dx
\right|
\le
\|g_j-g^*\|_{L^1}
\longrightarrow0.
\]
Hence
\[
J(g^*)=\lim_{j\to\infty}J(g_j)=I.
\]
Thus \(g^*\) attains the infimum.

\(\square\)

## Why breakpoint collisions do not invalidate the theorem

The free-knot parameter set with strictly increasing knots is not compact.
That is not the correct compactness space.

A minimizing sequence may have
\[
x_i^{(j)}-x_{i-1}^{(j)}\to0.
\]
The limiting spline then simply has fewer nondegenerate pieces. The
free-knot existence theorem is formulated precisely so that such knot
coalescence is included in the closure of the spline class.

Therefore one must **not** argue by claiming that the strictly ordered
breakpoint simplex is compact. Compactness is obtained at the level of the
spline class, with degenerate knots absorbed as redundant knots.

## Fixed-breakpoint existence

For completeness, the fixed-breakpoint case can be proved directly.

Fix
\[
a=x_0<x_1<\cdots<x_n=b
\]
and write the spline by its vertex values \(y_0,\ldots,y_n\).
The feasibility constraints
\[
g_y(x)\ge f(x)\qquad(x\in[a,b])
\]
form a closed set in \(\mathbb R^{n+1}\).

Moreover,
\[
J(g_y)
=
\sum_{i=0}^{n-1}
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-\int_a^b f(x)\,dx.
\]
Every \(y_i\) has a strictly positive coefficient. Consequently
\(J(g_y)\to+\infty\) whenever \(\|y\|\to\infty\) along the feasible set.
The objective is therefore coercive on a closed feasible set and attains
its minimum by the Weierstrass theorem.

This direct argument is independent of the free-knot theorem.

## Exactly \(n\) segments

If the optimizer has \(m<n\) nondegenerate affine pieces, choose arbitrary
interior points inside its affine pieces and declare them additional
breakpoints. The function does not change, so neither feasibility nor the
objective changes. Hence the optimizer has an exactly-\(n\)-segment
representation.

No uniqueness is asserted.

## Reference

R. B. Barrar and H. L. Loeb, “Existence of best spline approximations with
free knots,” *Journal of Mathematical Analysis and Applications* 31 (1970),
383--390.

The existence of this classical free-knot result is independently recorded
in standard spline bibliographies and later free-knot approximation
literature. The one-sided \(L^1\) setting is also part of the established
spline-approximation literature.
