# Breakpoint branch-and-bound from finite sampled relaxations

This solver is a second global-search formulation. It is deliberately not based on concavity, convexity, curvature, Monge structure, or a local optimum assumption for the free-breakpoint objective.

It combines two observations with a numerical oracle: fixed breakpoints leave a continuous shared-height problem, while dropping continuity and replacing the semi-infinite segment constraint by finitely many sampled constraints gives a rigorous lower relaxation.

## 1. Finite-sample segment relaxation

Fix u<v and sample points u=t_0<...<t_m=v. Let P be the set of affine functions L(x)=sx+c satisfying L(t_r)>=f(t_r) for all r. Let R_sample be the minimum integral excess over P, and R_true the corresponding one-segment optimum with L(x)>=f(x) for every x in [u,v].

### Theorem 1 — Sampled constraints give a lower bound

R_sample <= R_true.

### Proof

Every true majorizing affine function satisfies the sampled constraints. Thus the true feasible set is a subset of the sampled feasible set. Minimizing the same objective over the larger set cannot increase the optimum. No derivative or curvature assumption is used. ∎

## 2. Computing the finite relaxation

For L(x)=sx+c, the sampled constraints imply c >= max_r(f(t_r)-s t_r). After eliminating c, the sampled line integral is

Phi(s) = ((v^2-u^2)/2)s + (v-u) max_r(f(t_r)-s t_r).

This is a convex piecewise-linear function of s.

### Theorem 2 — Pairwise slope enumeration is exact

The minimum of the finite sampled LP is attained at a slope where two sampled constraints intersect, or on a flat minimizing piece. Hence evaluating every pairwise slope

s_rs = (f(t_s)-f(t_r))/(t_s-t_r)

together with s=0 obtains the finite LP optimum.

### Proof

A convex piecewise-linear function attains its minimum at a breakpoint or on a flat interval. Every breakpoint occurs when two affine functions f(t_r)-s t_r and f(t_s)-s t_s coincide. If the minimum is flat, any point in the flat interval has the same value; s=0 also attains that value whenever the active point is the midpoint. ∎

## 3. Dropping continuity

For a breakpoint sequence X=(x_0,...,x_n), let R(X) be the sum of independently optimized one-segment errors.

### Theorem 3 — Independent segment relaxation

R(X) <= V(X), where V(X) is the optimum with shared vertex heights.

### Proof

The continuous problem couples adjacent affine pieces through their shared endpoint heights. Removing those coupling constraints enlarges the feasible set. Therefore its optimum cannot increase. ∎

Combining Theorems 1 and 3 gives a valid sampled lower bound for every breakpoint sequence.

## 4. Branch-and-bound node bound

Suppose a search node has selected breakpoints x_0,...,x_r and accumulated sampled independent cost P. Let B_k(i) be the minimum relaxed sampled cost of any k-segment continuation from grid index i to the right endpoint, with continuity dropped.

Then every completion has objective at least P+B_k(i).

### Proof

Each future sampled independent segment cost is a lower bound for the corresponding true segment cost. Summing those bounds gives a lower bound for every completion. Minimizing the sum over all future breakpoint choices can only make the bound smaller, so B_k(i) remains valid for every particular completion. ∎

Thus a node may be pruned whenever P+B_k(i) is no smaller than a known feasible upper bound U.

## 5. Grid-global correctness

Let G_N={z_0,...,z_N}. The search considers every ordered sequence 0=i_0<i_1<...<i_n=N.

### Theorem 4 — Grid-global correctness

With exact finite-sample lower bounds and an exact fixed-breakpoint solver, branch-and-bound returns the breakpoint-grid optimum E_n,N^*.

### Proof

A pruned node has a lower bound at least as large as the incumbent, so no completion can improve the incumbent. Every unpruned leaf is a valid breakpoint sequence and its fixed-breakpoint problem is solved. Therefore the best surviving leaf is the minimum over all grid breakpoint sequences. ∎

The C++ implementation uses floating-point numerical oracles, so this is an oracle-model theorem rather than a machine-checkable certificate.

## 6. Convergence to free breakpoints

The existing breakpoint-discretization theorem gives E_n,N^* -> E_n^* as the grid mesh tends to zero. Therefore a sequence of grid-global branch-and-bound solves with mesh tending to zero converges to the unrestricted optimum, subject to the numerical-oracle error separation in theory/algorithm.md.

## 7. Why no curvature theorem is needed

The free-breakpoint objective may have different local curvature on different regions and need not be globally convex or concave. The branch-and-bound algorithm never infers global shape from local curvature.

## 8. Complexity

A grid with N+1 points has binomial(N-1,n-1) breakpoint sequences. With a fixed small sample count, constructing all sampled segment relaxations costs O(N^2) up to the constant from pairwise slopes. The branch search can still be exponential in n, but its practical cost depends on pruning strength.

This is intentionally a different trade-off from the O(nN^2) oracle-level breakpoint DP.

## 9. Numerical status

The finite sampled lower bound is mathematically safe, but the current implementation evaluates f and the integral in floating point. Therefore it does not claim a machine-checkable global certificate for arbitrary black-box continuous functions. The support search used by the fixed-breakpoint solver is also numerical, and grid-refinement stopping is empirical.

The important distinction is that the algorithmic relaxation itself is proved rather than relying on an unproved curvature or local-search assumption.
