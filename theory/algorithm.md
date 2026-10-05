# Approximation algorithm

The exact continuous problem has two free components: breakpoint locations and vertex heights. The following finite problems approximate both.

## 1. Breakpoint grid

Let

G_N={z_0<...<z_{m_N}}, z_0=a, z_{m_N}=b,

with mesh

delta_N=max_j(z_{j+1}-z_j) -> 0.

Restrict every breakpoint to G_N.

For a fixed ordered breakpoint sequence, the exact shared-height problem is the linear semi-infinite program of fixed-breakpoint.md. Its value is attained.

## 2. Height grid

Let

m=min f, M=max f, C=(b-a)(M-m),

and let

rho_N=min_j(z_{j+1}-z_j).

Every fixed-breakpoint optimum on G_N has vertex heights in

[m,B_N],

where

B_N=m+4C/rho_N.

Choose a finite height grid H_N subset [m,B_N] with mesh eta_N -> 0.

The fully discrete feasible set consists of splines whose breakpoints belong to G_N and whose vertex heights belong to H_N.

## 3. Exact finite dynamic program

A state is

(k,j,p),

where k is the number of completed segments, z_j is the current breakpoint, and p is its current height.

For a candidate next breakpoint z_l and next height q, the segment is feasible exactly when

p >= f(z_j)

and

q >= T_{z_j,z_l}(p),

where

T_{u,v}(p)
=
sup_{u<x<=v}
[(v-u)f(x)-(v-x)p]/(x-u).

Thus the finite recurrence is

D_{k+1}(l,q)
=
min_{k<=j<l}
min_{p in H_N}
[D_k(j,p)+(z_l-z_j)(p+q)/2],

with the minimization restricted to feasible pairs satisfying the two inequalities above.

The initial state is

D_0(0,p)=0 for p>=f(a), and +infinity otherwise.

After n segments,

E_{n,N,eta}^*
=
min_{q in H_N} D_n(m_N,q)
-
integral_a^b f(x) dx.

Store an attaining predecessor for each finite state and backtrack to recover the breakpoints and heights.

## 4. Correctness

For fixed N and eta_N, the state contains exactly the information needed to concatenate feasible segments: the current breakpoint and its shared height.

Every DP transition is feasible if and only if the corresponding segment majorizes f. Every feasible discrete spline determines one path through the DP, and every finite DP path determines one feasible discrete spline. Therefore the recurrence returns exactly the optimum of the fully discrete problem.

## 5. Convergence

Let E_n^* be the optimum of the original problem. Then

E_n^* <= E_{n,N,eta}^*
<= E_{n,N}^* + (b-a)eta_N,

where E_{n,N}^* is the exact optimum with breakpoints restricted to G_N and continuous heights.

If delta_N -> 0 and eta_N -> 0, then

E_{n,N}^* -> E_n^*

and consequently

E_{n,N,eta}^* -> E_n^*.

Thus the finite algorithm is convergent to the original optimum.

## 6. Numerical realization

The mathematical algorithm requires exact evaluation of f, T, and the integral. A numerical implementation must approximate these quantities with errors tending to zero as the discretization is refined. Numerical tolerances are therefore implementation parameters; they are not part of the convergence theorem.
