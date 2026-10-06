# One-Segment Slope Minimization

This module is a numerical primitive and reference calculation, not the global optimization algorithm.

## 1. Independent one-segment problem

For a single interval ([u,v]), an affine line
$$
L(x)=\alpha+\beta x
$$
is feasible when
$$
\alpha\\\ge\max_{x\in[u,v]}(f(x)-\beta x).
$$

For fixed (\eta), the smallest feasible intercept is
$$
\alpha(\beta)=\max_{x\in[u,v]}(f(x)-\beta x).
$$

Thus the independent one-segment objective is
$$
\Phi(\beta)
=
(v-u)\alpha(\beta)
+
\beta\frac{v^2-u^2}{2}
-
\int_u^v f(x)\,dx.
$$

The support term is convex in (\eta), so the exact scalar objective is convex.

## 2. Role in the current architecture

This calculation can be used for:

- (n=1) validation;
- regression tests;
- numerical diagnostics;
- benchmarking the support-search primitive.

It must **not** be used as the transition cost in the (n>1) shared-height DP.

The reason is that the independent problem chooses both endpoint heights implicitly for each segment. The target problem must use one common height at every internal breakpoint.

## 3. Numerical method

The current numerical route is:

1. evaluate the support maximum numerically;
2. obtain a finite slope bracket;
3. minimize the scalar objective inside the bracket;
4. reconstruct the line.

Because the support maximum is numerical, the returned value is a numerical approximation rather than an exact certificate.

## 4. Required invariant

No result from this module may be interpreted as
$$
C(u,v)
$$
for a shared-height DP unless the left and right endpoint heights are explicitly retained as part of the state.

