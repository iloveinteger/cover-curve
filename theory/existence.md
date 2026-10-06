# Existence of an optimal spline

Let $\\\mathcal A_n$ be the set of continuous piecewise-affine majorants of $f$ with at most $n$ nondegenerate affine pieces.

## Theorem 1 — Existence

For every continuous $f:[a,b]\to\mathbb R$ and every $n\ge1$, the minimum

$$
E_n^*=\min_{g\in\\\mathcal A_n}\int_a^b(g-f)
$$

is attained.

Consequently the problem stated with exactly $n$ segments also has an optimizer, because any affine piece may be split at an arbitrary interior point without changing the function.

### Proof status

For fixed breakpoints, existence is proved directly in `fixed-breakpoint.md`.

For free breakpoints, the knot set is not compact because knot intervals may collapse. We invoke the standard existence theory for best spline approximation with free knots, together with the established theory of one-sided $L^1$ spline approximation.

The free-knot existence reference is:

- R. B. Barrar and H. L. Loeb, “Existence of best spline approximations with free knots,” *Journal of Mathematical Analysis and Applications* 31 (1970), 383–390, DOI 10.1016/0022-247X(70)90032-6.

For the one-sided $L^1$ setting, see:

- A. Pinkus, “One-Sided $L^1$-Approximation by Splines with Fixed Knots,” *Journal of Approximation Theory* 18 (1976), 130–135.
- C. A. Micchelli and A. Pinkus, “Moment Theory for Weak Chebyshev Systems with Applications to Monosplines, Quadrature Formulae and Best One-Sided $L^1$-Approximation by Spline Functions with Fixed Knots,” *SIAM Journal on Mathematical Analysis* 8 (1977), 206–230.

These references are used for the free-knot/one-sided existence input. The fixed-breakpoint existence proof and all discretization and convergence arguments needed by this project are proved directly in the other theory documents.

The constraint $g\\\ge f$ is closed under uniform convergence, and the functional $g\mapsto\int_a^b(g-f)$ is continuous in the uniform norm. Thus the standard free-knot one-sided existence theorem applies.

No uniqueness is assumed or needed.
