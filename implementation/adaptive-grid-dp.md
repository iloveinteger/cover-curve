# Shared-Height Dynamic Programming

This document separates the exact mathematical target from the current numerical implementation.

## 1. Mathematical target

The mathematical algorithm discretizes **breakpoint locations only**:
\[
G_N=\{z_0<\cdots<z_m\}.
\]

Vertex heights remain continuous real variables. There is no mathematical height grid and no height-mesh convergence parameter.

For a fixed breakpoint sequence, segment feasibility is determined by
\[
q\ge T_{u,v}(p),
\]
and the exact breakpoint-grid DP is the continuous-height recurrence in theory/algorithm.md.

## 2. Current implementation status

The current C++ implementation still uses a finite height grid.

That implementation is therefore **not** an implementation of the exact continuous-height recurrence. It is a numerical baseline that approximates the continuous-height problem by restricting vertex heights to a finite set.

It must not be used as evidence for a theorem requiring continuous heights.

In particular, the old refinement
\[
N\mapsto2N,\qquad H\mapsto2H-1
\]
does not guarantee that the height mesh tends to zero, because the admissible height range depends on \(\rho_N\) and can grow with \(N\).

## 3. What a height-grid-free implementation must solve

For a grid point \(z_j\), segment count \(k\), and current height \(q\), the exact recurrence is
\[
F_{k+1}(j,q)=
\min_{i<j}
\inf_{\substack{p\ge f(z_i)\\q\ge T_{z_i,z_j}(p)}}
\left[
F_k(i,p)+\frac{z_j-z_i}{2}(p+q)
\right].
\]

A replacement implementation has to maintain continuous-height value information.

There are two distinct numerical tasks:

1. approximate the transition functions \(T_{u,v}\);
2. approximate the resulting continuous value functions without introducing a fixed height lattice.

A simple local golden-section search is not sufficient for a global correctness claim, because the minimum over predecessor breakpoints is not generally convex.

## 4. Practical numerical direction

A practical height-grid-free solver can use adaptive continuous optimization over the bounded interval
\[
[m_f,B_N],
\]
where
\[
m_f=\min f,
\qquad
B_N=m_f+\frac{4(b-a)(M_f-m_f)}{\rho_N}.
\]

The interval bound is mathematically valid for the global optimum of the breakpoint-restricted problem. The numerical optimizer itself may still be heuristic.

Possible implementation techniques include:

- adaptive one-dimensional evaluation of value functions;
- lower/upper envelopes of locally represented convex pieces;
- branch-and-bound over the continuous height variable;
- exact finite LP solves when the transition constraints are represented by a finite certified constraint set.

The implementation must explicitly label which technique is used and must not claim exactness unless its approximation and global-search errors are controlled.

## 5. Transition constraints

The exact transition is
\[
T_{u,v}(p)=
\sup_{u<x\le v}
\left[p+\frac{v-u}{x-u}(f(x)-p)\right].
\]

Sampling finitely many \(x\)-values gives a lower approximation to this supremum. Such sampling can discover likely active constraints, but it cannot certify \(L\ge f\) between samples for an arbitrary continuous black-box \(f\).

Therefore sampled constraints are suitable for candidate generation; certification requires an upper bound on the unsampled supremum or additional regularity information about \(f\).

## 6. Invariants

Any returned candidate spline must satisfy:

- one shared height at every common breakpoint;
- ordered breakpoints;
- all reported segments cover exactly \([a,b]\);
- no post-hoc continuity repair;
- objective computed from the reconstructed continuous spline, not from independently optimized intervals.

Until the continuous-height solver is implemented and verified, the finite-height implementation remains a baseline rather than the final algorithm.
