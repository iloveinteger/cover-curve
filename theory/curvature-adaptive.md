# Curvature-adaptive breakpoint grids

## 1. Role of curvature

The target problem is the continuous piecewise-linear **majorant with shared breakpoint heights**. Any curvature-based grid rule must therefore be interpreted as a strategy for selecting candidate breakpoint locations for this coupled problem.

It is not a proof that independent one-segment costs determine the global optimum.

## 2. Local quadratic model

For a sufficiently smooth function, on a short interval of length $h$ centered at $x$,
\[
f(x+t)
=
f(x)+f'(x)t+\frac12f''(x)t^2+o(h^2).
\]

The affine part is represented exactly by an affine segment. For the local independent majorant, the leading area error is proportional to
\[
|f''(x)|h^3.
\]

Consequently the familiar local model has the form
\[
E_{\mathrm{local}}
\approx
K|f''(x)|h^3
\]
for an appropriate convention-dependent constant $K$.

For the old independent one-segment calculation, the quadratic model gives $K=1/12$ under the centered-interval convention used previously.

This local calculation is **not by itself a theorem for the global continuous problem**, because neighboring segments share endpoint heights.

## 3. Density heuristic

If small cells have lengths $h_i$ and the local model
\[
E\approx K\sum_i q_i h_i^3,
\qquad
q_i\approx|f''(x_i)|,
\]
is used as an asymptotic surrogate, minimizing subject to
\[
\sum_i h_i=b-a
\]
gives
\[
3Kq_i h_i^2=\lambda,
\]
hence
\[
h_i\propto q_i^{-1/2}.
\]

Therefore the corresponding heuristic breakpoint density is
\[
\rho(x)\propto\sqrt{|f''(x)|}.
\]

Define
\[
w(x)=\sqrt{|f''(x)|},
\qquad
W(x)=\int_a^xw(t)\,dt.
\]
Equal increments of $W$ give the candidate asymptotic grid
\[
W(x_i)\approx\frac{i}{n}W(b).
\]

## 4. Status after introducing continuity

The density rule above remains a **heuristic candidate-grid rule**. It must not be described as the exact optimal allocation for the coupled problem until a local asymptotic analysis of the shared-height formulation establishes that result.

In particular, the implication
\[
\rho(x)\propto\sqrt{|f''(x)|}
\quad\Longrightarrow\quad
\text{globally optimal continuous breakpoint allocation}
\]
is currently unjustified.

The implementation may continue to use curvature to propose candidate grids, but the final optimization on those grids must solve the same shared-height continuous-cover problem as the baseline.

## 5. Numerical curvature estimate

For a black-box callable, the implementation may estimate
\[
f''(x)
\approx
\frac{f(x+h)-2f(x)+f(x-h)}{h^2}
\]
with suitable one-sided formulas near the endpoints.

A positive floor and density cap are numerical safeguards. They are not part of the mathematical theory.

## 6. Separation of errors

The curvature solver has at least two distinct approximations:
\[
\text{curvature estimate}
\to
\text{candidate grid}
\]
and
\[
\text{finite candidate optimization}
\to
\text{continuous optimum}.
\]

They must not be conflated. In particular, a curvature grid may improve efficiency without changing the mathematical target.

## 7. Future theorem

A rigorous curvature theorem should start from the **shared-height local problem**, derive its leading error coefficient, and then optimize the resulting density functional. Only after that derivation can the exponent and weighting function be claimed for the actual target problem.
