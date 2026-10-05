# Grid refinement and convergence

The convergence statement must refer to the **continuous, coupled problem**, not to the independent one-segment relaxation.

Let
\[
E_n^*
=
\inf
\left\{
\int_a^b(g-f):
g\text{ is a continuous PL majorant with }n\text{ nondegenerate segments}
\right\}.
\]

For a fixed breakpoint grid $G_N$, let $E_{n,N}^*$ denote the optimum of the **same continuous problem restricted to breakpoint locations in $G_N$**, with shared vertex heights optimized jointly.

The old quantity obtained from
\[
\sum_i C_{\mathrm{ind}}(x_i,x_{i+1})
\]
must not be identified with $E_{n,N}^*$.

## Fixed-breakpoint value

For
\[
X=(x_0,\ldots,x_n),
\qquad
a=x_0\le\cdots\le x_n=b,
\]
define $V(X)$ as the optimal value of the shared-height problem
\[
\min_y
\sum_{i=0}^{n-1}
\left[
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
\int_{x_i}^{x_{i+1}}f
\right]
\]
subject to the affine interpolation of $(x_i,y_i)$ and $(x_{i+1},y_{i+1})$ majorizing $f$ on each interval.

For positive-length intervals, feasibility is a semi-infinite linear constraint. If an interval has zero length, its contribution is interpreted as zero.

The continuous problem is therefore
\[
E_n^*=\min_X V(X)
\]
over admissible breakpoint vectors, with the usual interpretation of degenerate breakpoint limits.

## What must be proved for grid convergence

A valid grid-refinement theorem now requires three ingredients.

### 1. Feasibility preservation under breakpoint perturbation

Given a feasible continuous piecewise-linear majorant with breakpoints $X$, one must construct nearby grid breakpoints $X_N$ and corresponding shared heights $y_N$ that remain feasible.

It is not sufficient to round breakpoints while keeping independently optimized segment lines. The heights must be perturbed jointly so that the resulting adjacent segments still meet.

### 2. Cost continuity

One must establish continuity, or an appropriate upper-semicontinuity/approximation property, of the fixed-breakpoint value $V(X)$ under admissible perturbations.

The previous proof based only on continuity of $C_{\mathrm{ind}}(u,v)$ no longer proves this statement, because $V$ contains shared-height coupling.

### 3. Squeeze argument

Once the two properties above are established,
\[
E_n^*\le E_{n,N}^*
\]
because the grid-restricted feasible set is smaller.

If for every continuous optimum (or arbitrarily good feasible approximation) there exist grid-feasible continuous majorants with
\[
E(g_N)\to E_n^*,
\]
then
\[
E_n^*\le E_{n,N}^*\le E(g_N),
\]
and hence
\[
\boxed{E_{n,N}^*\to E_n^*.}
\]

This is the convergence theorem that the new implementation must satisfy.

## Numerical constraint refinement

If the implementation uses sampled support constraints and an exchange procedure, there is a second error source:
\[
\text{finite constraint approximation}
\ne
\text{exact semi-infinite constraint}.
\]

Therefore the proof of mathematical grid convergence must be kept separate from numerical support-search error.

A complete numerical analysis will need explicit assumptions and error bounds for:

- numerical integration;
- support maximization;
- finite constraint sampling;
- LP solution tolerance;
- breakpoint-grid refinement.

Until those bounds are established, the implementation should report numerical convergence empirically rather than claiming a rigorous global error bound.

## Independent relaxation

The independent value
\[
R_n^*
=
\min_X\sum_i C_{\mathrm{ind}}(x_i,x_{i+1})
\]
is a lower bound on the continuous coupled problem:
\[
R_n^*\le E_n^*.
\]

It remains useful as a diagnostic and baseline, but convergence of $R_{n,N}$ proves convergence only to the relaxation, not to the original continuous-cover problem.
