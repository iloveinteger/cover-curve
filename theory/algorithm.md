# Approximation algorithm

The exact continuous problem has two free components: breakpoint locations and vertex heights. The following finite problems approximate both.

## 1. Breakpoint grid

Let

$
G_N=\{z_0<\cdots<z_{m_N}\},\qquad z_0=a,\quad z_{m_N}=b,
$

with mesh

$
\delta_N=\max_j(z_{j+1}-z_j)\to0.
$

Restrict every breakpoint to \(G_N\).

For a fixed ordered breakpoint sequence, the exact shared-height problem is the linear semi-infinite program of fixed-breakpoint.md. Its value is attained.

## 2. Height grid

Let

$
m=\min_{[a,b]}f,\qquad M=\max_{[a,b]}f,\qquad C=(b-a)(M-m),
$

and let

$
\rho_N=\min_j(z_{j+1}-z_j)>0.
$

Every fixed-breakpoint optimum whose breakpoints lie in \(G_N\) has vertex heights in

$
[m,B_N],\qquad B_N=m+\frac{4C}{\rho_N}.
$

### Proof of the height bound

The constant spline \(g\equiv M\) is feasible and has error \(C\). Hence every fixed-breakpoint optimum satisfies \(E_X(y)\le C\).

For its vertex heights,

$
E_X(y)+\int_a^b(f-m)
=
\sum_i c_i(y_i-m),
$

where the fixed-breakpoint coefficients satisfy

$
c_i\ge\frac{\rho_N}{2}.
$

Since \(\int_a^b(f-m)\le C\),

$
\sum_i c_i(y_i-m)\le2C.
$

Also feasibility at each breakpoint gives \(y_i\ge f(x_i)\ge m\). Therefore

$
\frac{\rho_N}{2}(y_i-m)
\le c_i(y_i-m)
\le2C,
$

and hence

$
y_i\le m+\frac{4C}{\rho_N}=B_N.
$

Choose a finite height grid

$
H_N\subset[m,B_N]
$

whose mesh is \(\eta_N\to0\).

The fully discrete feasible set consists of splines whose breakpoints belong to \(G_N\) and whose vertex heights belong to \(H_N\).

## 3. Exact finite dynamic program

A state is

$
(k,j,p),
$

where \(k\) is the number of completed segments, \(z_j\) is the current breakpoint, and \(p\) is its current height.

For a candidate next breakpoint \(z_l\) and next height \(q\), the segment is feasible exactly when

$
p\ge f(z_j)
$

and

$
q\ge T_{z_j,z_l}(p),
$

where

$
T_{u,v}(p)
=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}.
$

Thus

$
D_{k+1}(l,q)
=
\min_{k\le j<l}
\min_{p\in H_N}
\left[
D_k(j,p)+
\frac{z_l-z_j}{2}(p+q)
\right],
$

where the minimization is restricted to feasible transitions.

The initial state is

$
D_0(0,p)=
0 for $p\ge f(a)$, and $+\infty$ for $p<f(a)$
$

After \(n\) segments,

$
E_{n,N,\eta}^*
=
\min_{q\in H_N}D_n(m_N,q)
-
\int_a^b f(x)\,dx.
$

Because the state and transition sets are finite, every finite minimum is attained. Store an attaining predecessor for each finite state and backtrack to recover the breakpoints and heights.

## 4. Correctness

For fixed \(N\) and \(\eta_N\), the state contains exactly the information needed to concatenate feasible segments: the current breakpoint and its shared height.

Every DP transition is feasible if and only if the corresponding segment majorizes \(f\), by the segment-feasibility lemma in dynamic-programming.md.

Every feasible discrete spline determines one path through the DP, and every finite DP path determines one feasible discrete spline. The path cost is exactly its trapezoidal integral \(\int_a^b g\).

Therefore the recurrence returns exactly the optimum of the fully discrete problem.

## 5. Convergence

Let \(E_{n,N}^*\) denote the exact optimum when breakpoints are restricted to \(G_N\) but vertex heights remain continuous.

First,

$
E_n^*\le E_{n,N}^*,
$

because the breakpoint-restricted feasible class is a subset of the original feasible class.

### Theorem — Breakpoint-grid convergence

If \(\delta_N\to0\), then

$
E_{n,N}^*\to E_n^*.
$

### Proof

Let \(g^*\) be an optimizer of the original problem, whose existence is given by existence.md. Choose a representation with exactly \(n\) segments by splitting affine pieces if necessary:

$
a=x_0^*<x_1^*<\cdots<x_n^*=b.
$

For each interior breakpoint choose \(z_i^{(N)}\in G_N\) with

$
z_i^{(N)}\to x_i^*.
$

Because there are only finitely many strictly ordered breakpoints, for all sufficiently large \(N\),

$
a=z_0^{(N)}<z_1^{(N)}<\cdots<z_n^{(N)}=b.
$

Let \(p_N\) be the piecewise-affine interpolant of \(g^*\) through

$
\bigl(z_i^{(N)},g^*(z_i^{(N)})\bigr).
$

Since \(g^*\) is continuous and piecewise affine, it is uniformly continuous, and the knot perturbations tend to zero. Hence

$
\|p_N-g^*\|_\infty\to0.
$

Put

$
\varepsilon_N=\|p_N-g^*\|_\infty,
\qquad
\widetilde g_N=p_N+\varepsilon_N.
$

Then

$
\widetilde g_N\ge g^*\ge f,
$

so \(\widetilde g_N\) is feasible for the breakpoint-restricted problem. Moreover,

$
0\le E(\widetilde g_N)-E(g^*)
\le2(b-a)\varepsilon_N\to0.
$

Therefore

$
E_n^*
\le E_{n,N}^*
\le E(\widetilde g_N)
\to E(g^*)=E_n^*,
$

which proves

$
\boxed{E_{n,N}^*\to E_n^*.}
$

### Height-grid convergence

For each breakpoint-restricted optimizer, round every vertex height upward to the smallest point of \(H_N\) not below it. Feasibility is preserved because every height only increases.

Each height increases by at most \(\eta_N\). Since the trapezoidal coefficients satisfy

$
\sum_i c_i=b-a,
$

the objective increase is at most

$
(b-a)\eta_N.
$

Thus

$
E_{n,N}^*
\le E_{n,N,\eta}^*
\le E_{n,N}^*+(b-a)\eta_N.
$

Combining this with breakpoint-grid convergence gives

$
\boxed{E_{n,N,\eta}^*\to E_n^*}
$

whenever

$
\delta_N\to0,
\qquad
\eta_N\to0.
$

Thus the finite shared-height dynamic program converges to the optimum of the original free-breakpoint problem.

## 6. Numerical realization

The mathematical algorithm assumes exact evaluation of \(f\), \(T\), and the integral. A numerical implementation must approximate these quantities with errors tending to zero as the discretization is refined. Those numerical errors are implementation-level issues and are not part of the convergence theorem.
