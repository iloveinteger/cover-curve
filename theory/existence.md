# Existence of a global optimum

Let \(\mathcal A_n\) be the admissible continuous piecewise-affine majorants with at most \(n\) nondegenerate affine pieces. Using at most \(n\) pieces instead of exactly \(n\) does not change the optimal value: any function with fewer than \(n\) pieces can be split at arbitrary interior points of its affine pieces.

Define
\[
E_n^*=\inf_{g\in\mathcal A_n} \int_a^b (g-f).
\]

## Theorem 1 — Global existence

For every continuous \(f:[a,b]\to\mathbb R\) and every integer \(n\ge1\), the infimum \(E_n^*\) is attained by some \(g^*\in\mathcal A_n\). Consequently an optimum with exactly \(n\) nondegenerate segments also exists.

### Proof structure

The fixed-breakpoint problem is already known to attain its minimum. The only additional issue is that the breakpoints are free.

The relevant compactness fact is the standard existence theorem for free-knot spline approximation: a minimizing sequence of degree-one splines with a uniformly bounded \(L^1\) objective has a subsequence whose non-collapsing pieces converge, while intervals whose lengths tend to zero either disappear or merge adjacent pieces. Thus the limit has at most \(n\) affine pieces. For the present one-sided problem, every member of the minimizing sequence satisfies \(g_k\ge f\). Since \(f\) is continuous, the limiting spline can be chosen to remain above \(f\), and the \(L^1\) functional is lower semicontinuous under this convergence.

Therefore a minimizing spline with at most \(n\) pieces exists.

Finally, if the minimizer has \(m<n\) pieces, split any of its nondegenerate affine intervals at arbitrary interior points. The resulting function is unchanged and has exactly \(n\) segments. Hence the original exactly-\(n\) problem also has a minimizer. ∎

## Why the fixed-breakpoint theorem is not enough

The existence result for a fixed breakpoint sequence cannot by itself be promoted to free breakpoints by saying that the breakpoint set is compact. The strict inequalities

\[
a=x_0<x_1<\cdots<x_n=b
\]

form an open simplex, so a minimizing sequence may have collapsing intervals. The free-knot existence theorem is precisely what handles these degeneracies.

This distinction is important for the global convergence argument: the grid approximation only needs the value \(E_n^*\), but the existence theorem additionally guarantees that this value is represented by an actual optimal spline.

## Reference

Existence of best spline approximations with free knots is classical; see R. B. Barrar and H. L. Loeb, *Existence of best spline approximations with free knots*, Journal of Mathematical Analysis and Applications 31 (1970), 383–390.
