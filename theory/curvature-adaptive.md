# Curvature-adaptive breakpoint grids

## 1. Motivation

The baseline solver uses a uniform breakpoint grid and dynamic programming. For a sufficiently smooth function, the local error of a short affine upper cover contains information about the local curvature. This suggests spending more breakpoint resolution where the curve bends more strongly.

This document derives the first-order grid-density heuristic used by the experimental curvature-adaptive solver.

## 2. Local model

Consider a short interval of length $h$ centered at $x$, and suppose $fin C^2$. To second order,

```math
f(x+t)=f(x)+f'(x)t+rac12 f''(x)t^2+o(h^2).
```

The affine part is reproduced exactly by an affine cover, so the leading local covering error is determined by the quadratic term.

For a quadratic with constant second derivative $q$, the least-area affine upper cover is obtained from its secant/tangent geometry. In either sign of $q$, the leading error is

```math
Cleft(x-rac h2,x+rac h2ight)
=
rac{|q|}{12}h^3+o(h^3).
```

Hence, locally,

```math
Cleft(x-rac h2,x+rac h2ight)
=
rac{|f''(x)|}{12}h^3+o(h^3).
```

This is an asymptotic statement, not an exact formula for arbitrary finite intervals.

## 3. Optimal local spacing

Suppose the interval is partitioned into small cells with lengths $h_i$. The local model gives

```math
Eapproxrac1{12}sum_i |f''(x_i)|h_i^3.
```

For a fixed number of cells $n$, minimize this approximation subject to

```math
sum_i h_i=b-a.
```

The Lagrange multiplier condition is

```math
3|f''(x_i)|h_i^2=lambda.
```

Therefore

```math
h_ipropto |f''(x_i)|^{-1/2}.
```

Equivalently, the breakpoint density is

```math
ho(x)proptosqrt{|f''(x)|}.
```

Define

```math
w(x)=sqrt{|f''(x)|},
qquad
W(x)=int_a^x w(t),dt.
```

Then the asymptotic grid is characterized by approximately equal increments of $W$:

```math
W(x_i)approxrac{i}{n}W(b),
qquad i=0,ldots,n.
```

## 4. Degenerate curvature

If $f''(x)=0$ on an interval, the second-order model predicts zero leading error there. The true function may still have higher-order curvature, and numerical second derivatives may also be noisy.

The implementation therefore does not use $w(x)=sqrt{|f''(x)|}$ literally. It uses a positive curvature floor and a bounded density range. These are numerical safeguards, not part of the asymptotic theorem.

## 5. Numerical second derivative

The public function type is a generic callable, so the solver cannot assume that an analytic derivative is available. The experimental implementation estimates

```math
f''(x)approx
rac{f(x+h)-2f(x)+f(x-h)}{h^2},
```

with one-sided differences near the endpoints.

This introduces another numerical approximation. Consequently, the curvature solver should be viewed as a grid-generation heuristic followed by finite-grid DP, not as a new proof of global optimality.

## 6. Relationship to the baseline

The baseline remains unchanged. The curvature solver changes only the candidate breakpoint grid:

1. estimate $|f''|$;
2. construct a density proportional to $sqrt{|f''|}$;
3. place $N+1$ breakpoints at approximately equal cumulative density;
4. run the finite-grid dynamic program;
5. refine $N$ until the objective stabilizes.

Thus every fixed curvature grid still receives the same finite-grid optimization structure. The new approximation is entirely in the choice of candidate grid.

## 7. Scope and limitations

The asymptotic derivation assumes sufficient smoothness and short cells. It is most informative when $f$ is locally well approximated by a quadratic.

It is not a universal theorem that a curvature-adaptive grid is better than a uniform grid for every continuous function. In particular:

- nonsmooth functions are outside the $C^2$ derivation;
- rapidly changing curvature can make coarse curvature sampling inaccurate;
- sign changes of $f''$ can change exact one-segment geometry;
- higher-order effects matter where $f''$ is small;
- numerical differentiation can amplify noise.

For this reason the curvature solver is an experimental second solver and should be compared against the baseline rather than replacing it.

## 8. Future directions

The derivation suggests:

- deriving higher-order local asymptotics when $f''$ vanishes;
- using analytic or spline derivatives when available;
- adapting the grid from the actual one-segment cost rather than only from $f''$;
- combining curvature prediction with a posteriori DP-error indicators;
- investigating Monge or related structure in the cost matrix for restricted function classes.
