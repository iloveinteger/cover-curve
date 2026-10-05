# Shared-Height Dynamic Programming

This document specifies the implementation target corresponding to the mathematical algorithm in `theory/algorithm.md`. It is a design specification; it does not claim that the current C++ solver already implements this algorithm.

## 1. Discrete problem

Choose a breakpoint grid
\[
G_N=\{z_0<\cdots<z_m\},\qquad z_0=a,\ z_m=b,
\]
and a finite height grid
\[
H_N\subset[m_f,B_N],
\]
where
\[
m_f=\min_{x\in[a,b]}f(x),\qquad
B_N=m_f+\frac{4C}{\rho_N},
\]
\[
C=(b-a)(M_f-m_f),\qquad
\rho_N=\min_j(z_{j+1}-z_j).
\]

The implementation target is the finite shared-height problem from the theory. Breakpoints are restricted to (G_N), vertex heights are restricted to (H_N), and adjacent segments share the same height at their common breakpoint.

The height-grid mesh is
\[
\eta_N=\max\{q'-q:q,q'\in H_N\text{ consecutive}\}.
\]

The refinement requirements are
\[
\delta_N=\max_j(z_{j+1}-z_j)\to0,
\qquad
\eta_N\to0.
\]

## 2. State

A DP state is
\[
(k,j,p),
\]
where:

- (k) is the number of completed segments;
- (z_j) is the current breakpoint;
- (p\in H_N) is the current vertex height.

Let
\[
D_k(j,p)
\]
be the minimum trapezoidal integral of the already constructed (k) segments among all feasible paths ending at ((z_j,p)). An unreachable state has value (+\infty).

The height (p) is part of the state. It must not be eliminated by assigning an independent cost to the interval.

## 3. Segment transition

For (u<v) and left height (p), define
\[
T_{u,v}(p)
=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}.
\]

A transition
\[
(z_j,p)\to(z_l,q)
\]
with (j<l) is feasible exactly when
\[
p\ge f(z_j),
\qquad
q\ge T_{z_j,z_l}(p).
\]

In the finite implementation, the exact (T) is replaced by a numerical upper approximation or by a progressively refined finite set of pointwise constraints. The approximation must be conservative for a majorant solver: an underestimated (T) can produce an infeasible segment.

## 4. DP recurrence

Initialize
\[
D_0(0,p)=
\begin{cases}
0,&p\ge f(a),\\
+\infty,&p<f(a).
\end{cases}
\]

For (k=0,\ldots,n-1),
\[
D_{k+1}(l,q)
=
\min_{k\le j<l}
\min_{p\in H_N}
\left[
D_k(j,p)
+
\frac{z_l-z_j}{2}(p+q)
\right],
\]
over transitions satisfying the feasibility condition above.

The final discrete objective is
\[
E_{n,N,\eta}^*
=
\min_{q\in H_N}D_n(m,q)
-
\int_a^b f(x)\,dx.
\]

A predecessor record must store the previous breakpoint index and previous height-grid index.

## 5. Why the shared height is mandatory

The two segments adjacent to (z_j) must use the same value (p=g(z_j)). Therefore the DP cannot use a scalar cost
\[
C(u,v)
\]
computed independently for each interval.

The following architecture is not an implementation of the target problem:

1. solve every interval independently;
2. assign one scalar cost to each interval;
3. optimize those scalar costs with a breakpoint DP;
4. modify the resulting segments afterward to enforce continuity.

The continuity constraint must be present in the optimization state itself.

## 6. Computing the transition constraint

The exact transition quantity can equivalently be written as
\[
T_{u,v}(p)
=
p+(v-u)
\sup_{u<x\le v}
\frac{f(x)-p}{x-u}.
\]

This form is useful for numerical evaluation. The ratio can become arbitrarily large near (u) when (p=f(u)) and (f) has sufficiently steep local growth. The implementation must therefore not assume that the supremum is finite merely because (f) is continuous.

For a finite constraint set (S\subset(u,v]), use
\[
T_S(u,v;p)
=
\max_{x\in S}
\left[
p+\frac{v-u}{x-u}(f(x)-p)
\right].
\]

If (S) is enlarged, this lower approximation is monotone nondecreasing. It is suitable for detecting violated constraints, but it is not by itself a certified upper bound on the true (T).

A correctness-oriented implementation should therefore distinguish:

- **constraint discovery:** sampled points used to find likely active constraints;
- **feasibility certification:** an upper bound or additional regularity assumption sufficient to rule out violations between samples.

For an arbitrary continuous black-box (f), finite point samples alone cannot certify the exact supremum.

## 7. Breakpoint and height refinement

For each refinement level:

1. construct (G_N);
2. compute (m_f,M_f,C,ho_N,B_N);
3. construct (H_N\subset[m_f,B_N]);
4. solve the finite shared-height DP;
5. reconstruct the spline from predecessor states;
6. record the objective and diagnostic information.

The mathematical convergence result requires
\[
\delta_N\to0,qquad \eta_N\to0.
\]

A practical sequence such as (N\mapsto2N) is only one possible refinement policy. The implementation must not treat a fixed finite grid as the exact continuous optimum.

## 8. Numerical error separation

The discretization error and numerical evaluation error are separate.

The theory assumes exact evaluation of:

- (f(x));
- the transition supremum (T_{u,v}(p));
- (int_a^b f(x)\,dx).

The implementation uses numerical approximations to these quantities. Their tolerances must be tracked separately from (delta_N) and (eta_N).

A convergence report should therefore distinguish at least:

- breakpoint-grid level;
- height-grid mesh;
- support/transition-search tolerance;
- integration tolerance;
- resulting objective.

## 9. Complexity

No complexity bound from the old independent scalar DP is inherited by this solver.

For a finite height grid, the direct recurrence has state space of order
\[
O(nm|H_N|)
\]
and a naive transition evaluation can be substantially larger because it considers previous breakpoints and heights.

Any optimized complexity claim must be derived from the actual shared-height implementation after that implementation exists.

## 10. Required implementation invariant

At every point where a candidate spline is constructed, its vertex heights are a single shared sequence
\[
(y_0,\ldots,y_n).
\]

There must be no post-hoc continuity repair that changes the optimized objective without re-solving the coupled problem.

