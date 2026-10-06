# Existence of an optimal spline

Let $f:[a,b]\to\mathbb R$ be continuous and let $n\ge1$.

## Theorem 1 — Existence

There exists a continuous piecewise-affine majorant $g$ with exactly $n$ segments such that

$g(x)\ge f(x) \qquad (x\in[a,b])$

and

$$
\int_a^b (g(x)-f(x))dx
$$

is minimal among all such majorants.

### Proof

We first allow at most $n$ nondegenerate affine pieces. Let $\mathcal S_n$ denote this class, and define

$$
J(g)=\int_a^b(g(x)-f(x))dx.
$$

The feasible class is nonempty: the constant function $g\equiv M$, where

$$
M=\max_{x\in[a,b]}f(x),
$$

is a feasible spline with one affine piece.

The fixed-breakpoint problem has a minimizer by the result proved in `fixed-breakpoint.md`. Thus the only issue is the freedom of the breakpoints.

We use the following standard free-knot existence theorem for spline approximation.

> **Free-knot existence theorem.** For a continuous target on a compact interval, the infimum of a continuous $L^1$-type error functional over a finite-dimensional family of continuous splines of fixed degree and bounded number of free knots is attained, including the one-sided problem obtained by restricting the approximants to lie above the target.

This is the only external existence result used here. A classical reference for existence with free knots is Barrar and Loeb, *Existence of best spline approximations with free knots*, Journal of Mathematical Analysis and Applications 31 (1970), 383–390. The one-sided $L^1$ formulation is part of the standard one-sided spline approximation theory.

Applying this theorem with degree $1$, at most $n$ pieces, target $f$, and the constraint $g\ge f$ gives a minimizer $g^*$ in $\mathcal S_n$.

It remains to recover the formulation with exactly $n$ segments. Suppose $g^*$ has $m\le n$ nondegenerate affine pieces. Each affine piece can be subdivided at arbitrary interior points. Subdivision does not change the function, the majorant constraint, or the value of $J$. Therefore $g^*$ can be represented with exactly $n$ segments.

Hence the original problem has an optimizer.

No uniqueness is asserted.
