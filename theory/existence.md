# Existence of an optimal spline

Let $\mathcal A_n$ be the set of continuous piecewise-affine majorants of $f$ with at most $n$ nondegenerate affine pieces.

## Theorem 1 — Existence

For every continuous $f:[a,b]\to\mathbb R$ and every $n\ge1$, the minimum

$E_n^*=\min_{g\in\mathcal A_n}\int_a^b(g-f)$

is attained.

Consequently the problem stated with exactly $n$ segments also has an optimizer, because an affine piece may be split at arbitrary interior points without changing the function.

### Justification

For a fixed breakpoint sequence, existence follows from `fixed-breakpoint.md`.

For free breakpoints, the knot set is not compact because knot intervals may collapse. The required compactness result is the standard existence theorem for best spline approximation with free knots, together with the one-sided $L^1$ formulation: a minimizing sequence of splines with at most $n$ pieces has a subsequence converging to a spline with at most $n$ pieces, after zero-length pieces are removed. The one-sided constraint $g\ge f$ is preserved under uniform convergence, and the integral functional is continuous under uniform convergence.

Thus a minimizer exists in $\mathcal A_n$.

This is a standard free-knot existence result; see Barrar and Loeb, *Existence of best spline approximations with free knots*, Journal of Mathematical Analysis and Applications 31 (1970), 383–390, and the literature on one-sided $L^1$ spline approximation. The one-sided fixed-knot existence statement is explicit in Pinkus, *One-Sided $L^1$ Approximation by Splines with Fixed Knots*, Journal of Approximation Theory 18 (1976), 130–135.

No uniqueness is assumed or needed.
