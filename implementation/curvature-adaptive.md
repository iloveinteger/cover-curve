# Curvature-density initialization

This module is an initialization heuristic for free-breakpoint optimization. It is not a substitute for the fixed-breakpoint solver and does not provide a finite-$n$ optimality certificate.

## 1. Theory

For constant-sign curvature, the local $L^1$ majorant error is
$$
c(x)|f''(x)|h^3+o(h^3),
$$
with
$$
c(x)=1/12\quad(f''>0),\qquad
c(x)=1/24\quad(f''<0).
$$

Balancing this leading term over $n$ cells gives
$$
\boxed{\rho(x)\propto c(x)^{1/3}|f''(x)|^{1/3}}.
$$

The implementation therefore uses the cube-root curvature density. The older $|f''|^{1/2}$ description is obsolete.

## 2. Construction

For $M$ uniformly spaced samples:

1. estimate $f''$ with the centered three-point difference;
2. compute
$$
   d_i=\max(d_{\min},(c_i|f''_i|)^{1/3});
$$
3. integrate $d_i$ by the trapezoidal rule;
4. place $n-1$ interior knots at equal cumulative-density quantiles.

A positive floor is used only to avoid a degenerate numerical CDF at zero curvature.

## 3. Complexity

The seed construction costs $O(M)$ sampled function evaluations and $O(M+n)$ arithmetic work.

It is normally negligible compared with one direct-height solve.

## 4. Limitations

- second-derivative estimation can be noisy for black-box functions;
- the density is an asymptotic model;
- the seed does not prove finite-$n$ global optimality;
- active-set transitions can make the outer value function nonsmooth.

The uniform seed is retained as a robustness baseline.
