# Curvature-adaptive breakpoint grids

## 1. Motivation

The baseline solver uses a uniform breakpoint grid and dynamic programming. For a sufficiently smooth function, the local error of a short affine upper cover contains information about the local curvature. This suggests spending more breakpoint resolution where the curve bends more strongly.

This document derives the first-order grid-density heuristic used by the experimental curvature-adaptive solver.

## 2. Local model

Consider a short interval of length h centered at x, and suppose f is C2. To second order,

f(x+t) = f(x) + f'(x)t + (1/2) f''(x)t^2 + o(h^2).

The affine part is reproduced exactly by an affine cover, so the leading local covering error is determined by the quadratic term.

For a quadratic with constant second derivative q, its secant is the affine upper cover when q >= 0, while the reflected construction applies when q < 0. In either case the leading area error is |q| h^3 / 12. Hence, locally,

C(x-h/2,x+h/2) = |f''(x)| h^3 / 12 + o(h^3).

This is an asymptotic statement, not an exact formula for arbitrary finite intervals.

## 3. Optimal local spacing

Suppose the interval is partitioned into small cells with lengths h_i. The local model gives

E approximately (1/12) sum_i |f''(x_i)| h_i^3.

For a fixed number of cells n, minimizing this approximation subject to sum_i h_i = b-a gives, by a Lagrange multiplier,

3 |f''(x_i)| h_i^2 = lambda.

Therefore h_i is proportional to |f''(x_i)|^(-1/2). Equivalently, the breakpoint density is

rho(x) is proportional to sqrt(|f''(x)|).

In the continuous limit, define w(x)=sqrt(|f''(x)|). Equal increments of W(x)=integral_a^x w(t) dt produce the asymptotically appropriate breakpoint distribution: W(x_i) is approximately i W(b)/n.

## 4. Degenerate curvature

If f''(x)=0 on an interval, the local second-order model predicts zero error there. The true function may still have higher-order curvature, and numerical second derivatives may also be noisy.

The implementation therefore does not use w(x)=sqrt(|f''(x)|) literally. It uses a small positive curvature floor and a bounded density range. This keeps the grid valid and prevents an almost-linear region from receiving an arbitrarily large cell.

The floor and density bounds are numerical safeguards; they are not part of the mathematical asymptotic result.

## 5. Numerical second derivative

The public function type is a generic callable, so the solver cannot assume that an analytic derivative is available. The experimental implementation estimates f''(x) with a symmetric finite difference,

D2 f(x) approximately [f(x+h)-2f(x)+f(x-h)] / h^2,

using one-sided differences near the endpoints.

This introduces an additional numerical approximation. Consequently the curvature solver is a grid-generation heuristic followed by the same finite-grid DP optimization used by the baseline, not a new proof of global optimality.

## 6. Relationship to the baseline

The baseline remains unchanged. The curvature solver changes only the candidate breakpoint grid:

1. estimate curvature magnitude;
2. construct a density proportional to sqrt(|f''|);
3. place N+1 breakpoints at approximately equal cumulative density;
4. run the finite-grid dynamic program;
5. refine N until the objective stabilizes.

Thus every fixed curvature grid still receives a finite-grid DP treatment. The new approximation is entirely in the choice of candidate grid.

## 7. Scope and limitations

The asymptotic derivation assumes sufficient smoothness and short cells. It is most informative when f is locally well approximated by a quadratic.

It is not a universal theorem that a curvature-adaptive grid is better than a uniform grid for every continuous function. Nonsmooth functions are outside the C2 derivation; rapidly changing curvature can make coarse curvature sampling inaccurate; sign changes of f'' can change exact one-segment geometry; higher-order effects matter where f'' is small; and numerical differentiation can amplify noise.

For this reason the curvature solver is an experimental second solver and should be compared against the baseline rather than replacing it.

## 8. Future directions

The derivation suggests deriving higher-order local asymptotics when f'' vanishes, estimating curvature using spline derivatives for sampled data, adapting the grid from actual one-segment cost rather than only f'', combining curvature prediction with a posteriori DP-error indicators, and investigating whether the cost matrix has Monge or related structure for restricted function classes.
