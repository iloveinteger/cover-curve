# Adaptive breakpoint search

## 1. Purpose

The free-breakpoint problem is non-convex because the internal breakpoints are variables. The fixed-breakpoint problem is convex in the shared vertex heights, but minimizing its value over breakpoint locations is not a convex optimization problem.

This document gives a correctness argument for a solver that searches breakpoint space directly and uses the exact fixed-breakpoint shared-height problem as its inner oracle.

The method is deliberately different from the breakpoint-grid dynamic program: it does not enumerate predecessor breakpoints and does not use a height grid.

## 2. Breakpoint value function

For
$$
X=(x_1,\ldots,x_{n-1}),\qquad
a<x_1<\cdots<x_{n-1}<b,
$$
put
$$
x_0=a,\qquad x_n=b
$$
and define
$$
V(X)=\min_y E_X(y),
$$
where the minimum is the fixed-breakpoint shared-height majorant problem from `fixed-breakpoint.md`.

The free-breakpoint optimum is
$$
E_n^*=\inf_{X\in\mathcal X}V(X),
$$
with
$$
\mathcal X=\{a<x_1<\cdots<x_{n-1}<b\}.
$$

An optimizer exists by the free-knot existence theorem used in `existence.md`.

## 3. Local continuity of V

A global Lipschitz estimate for V is **not** assumed. It is also not necessary.

### Theorem 1 — continuity at a strict breakpoint representation

Let X* be a strict breakpoint representation of an optimal spline, and let
$$
a=x_0^*<x_1^*<\cdots<x_n^*=b.
$$
Then V is continuous at X*.

### Proof

Because all intervals of X* have positive length, there is a neighborhood U of X* in which all breakpoint sequences remain strictly ordered and every perturbed breakpoint stays in the corresponding neighborhood of the original one.

Let g* be an optimal spline for X*. It is piecewise affine and therefore Lipschitz on [a,b]; let K be a Lipschitz constant.

For a sequence X_r -> X*, construct p_r by interpolating g* at the perturbed breakpoints. The perturbed segments can cross an old knot only inside an interval whose length tends to zero. The Lipschitz property gives
$$
\|p_r-g^*\|_\infty\le C K\|X_r-X^*\|_\infty
$$
for a fixed local constant C. Therefore
$$
\widetilde p_r=p_r+C K\|X_r-X^*\|_\infty
$$
is a feasible majorant for X_r and
$$
E(\widetilde p_r)\to E(g^*)=V(X^*).
$$
Hence
$$
\limsup_{r\to\infty}V(X_r)\le V(X^*).
$$

For the reverse inequality, take optimal splines g_r for X_r. In a sufficiently small neighborhood U the breakpoint spacings are bounded below by a positive number. The vertex values are bounded on every sublevel set because the fixed-breakpoint objective has strictly positive coefficients. Thus the corresponding piecewise-affine functions have uniformly bounded heights and slopes. A subsequence converges uniformly to a feasible spline for X*. The objective is continuous under uniform convergence, so
$$
V(X^*)\le\liminf_{r\to\infty}V(X_r).
$$
Combining the two inequalities proves
$$
V(X_r)\to V(X^*).
$$
∎

The argument is local on purpose: breakpoint collisions need not be treated as ordinary points of the strict parameter domain. An optimum that uses fewer than n distinct affine pieces can be represented with n strict breakpoints by splitting affine pieces, so an optimal strict representation is always available.

## 4. Exhaustive subdivision

A search node is a hyperrectangle
$$
B=\prod_{i=1}^{n-1}[\ell_i,r_i].
$$

The node is allowed to contain unordered breakpoint vectors. Such vectors are simply infeasible and are never passed to the fixed-breakpoint oracle.

A node is bisected along a longest coordinate:
$$
m_i=(\ell_i+r_i)/2.
$$

Both children are retained. Therefore the union of nodes after every subdivision still covers the entire ambient box [a,b]^{n-1}, and in particular covers every strict breakpoint vector.

After d complete levels of bisection,
$$
\max_i(r_i-\ell_i)\le\frac{b-a}{2^d}.
$$

Thus the subdivision is exhaustive.

## 5. The search theorem

### Theorem 2 — global convergence of midpoint breakpoint search

Assume the fixed-breakpoint oracle returns V(X) exactly. Let U_d be the smallest value among all feasible midpoint evaluations performed through subdivision depth d.

Then
$$
U_d\ge E_n^*
$$
for every d and
$$
\boxed{U_d\longrightarrow E_n^*.}
$$

### Proof

Every evaluated midpoint is a feasible strict breakpoint sequence, hence its value is at least E_n*. Thus U_d >= E_n*.

Let X* be an optimal strict breakpoint representation. For every neighborhood of X*, exhaustive bisection eventually produces a node whose midpoint lies in that neighborhood. By Theorem 1,
$$
V(X)\to V(X^*)=E_n^*
$$
as X -> X*. Hence for every epsilon > 0 there is a sufficiently deep node whose midpoint X satisfies
$$
V(X)<E_n^*+\epsilon.
$$
Therefore
$$
\limsup_d U_d\le E_n^*+\epsilon.
$$
Since epsilon is arbitrary and U_d >= E_n*, the result follows.
∎

This is a deterministic global-convergence theorem. It does **not** require convexity of V.

## 6. Why the previous lower-bound proof is not used

For an interval [u,v], let C(u,v) be the independently optimized one-segment majorant cost. Then
$$
\sum_i C(x_i,x_{i+1})\le V(X)
$$
is a valid relaxation.

However, this relaxation does not by itself prove
$$
\inf_{X\in B}\sum_i C(x_i,x_{i+1})
\longrightarrow V(X^*)
$$
as the breakpoint box B shrinks. Independent segment optima can have incompatible vertex heights.

Therefore the independent one-segment cost is a valid lower bound, but it is **not** used as a convergence certificate in this solver.

This distinction is important: a branch-and-bound pruning rule requires a lower bound that is both valid and sufficiently consistent with the target value. General deterministic global optimization theory likewise treats the quality and convergence of lower bounds as a separate requirement.

## 7. Numerical solver

The actual implementation is an adaptive midpoint search:

1. evaluate a feasible point in the current node;
2. update the incumbent upper bound;
3. bisect the longest coordinate;
4. evaluate child nodes;
5. continue until a depth/evaluation budget is reached.

The inner evaluation uses the existing continuous-height shared-height solver. Thus continuity of the resulting spline is enforced by the inner optimization, not by joining independently optimized segments.

The returned result is always a feasible candidate **to the accuracy of the numerical inner solver**. It is not a finite-time global certificate for a black-box continuous function.

## 8. Complexity

Let d=n-1 be the breakpoint dimension.

A full binary subdivision tree through depth D contains
$$
O(2^D)
$$
nodes, with constants exponential in the breakpoint dimension. Every evaluated node requires one fixed-breakpoint continuous-height solve.

Thus this solver is not intended to replace the DP for large n. Its purpose is to provide a structurally different deterministic global search that can cross-check the non-convex breakpoint optimization.

## 9. Relation to the breakpoint-grid DP

The two methods have different outer search spaces.

- Breakpoint DP: discretizes breakpoint locations first and solves the resulting discrete dynamic program exactly in the continuous-height oracle model.
- Adaptive breakpoint search: directly searches the continuous breakpoint domain and repeatedly calls the fixed-breakpoint shared-height solver.

Agreement between them is therefore a useful implementation cross-check, especially on small n.

## 10. Numerical caveat

The theorem above is an oracle theorem. The C++ implementation approximates the transition supremum, integration, and continuous one-dimensional minimizations. Consequently numerical convergence and feasibility must be tested independently.

A finite numerical gap between two solvers is evidence, not a mathematical proof of global optimality.
