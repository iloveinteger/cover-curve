# Exact dynamic programming for continuous vertex heights

Fix
$$
a=x_0<x_1<\cdots<x_n=b
$$
and let
$$
h_i=x_{i+1}-x_i.
$$

For $u<v$ and $p\in\mathbb R$, define
$$
T_{u,v}(p)=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}.
$$

## Lemma 1 — Segment feasibility

The affine function joining $(u,p)$ and $(v,q)$ satisfies $L\ge f$ on $[u,v]$ if and only if
$$
p\ge f(u)
$$
and
$$
q\ge T_{u,v}(p).
$$

### Proof

For $u<x\le v$,
$$
\frac{v-x}{v-u}p+
\frac{x-u}{v-u}q\ge f(x)
$$
is equivalent to
$$
q\ge
\frac{(v-u)f(x)-(v-x)p}{x-u}.
$$
Taking the supremum over $x\in(u,v]$ gives the result. The endpoint $u$ is exactly the separate condition $p\ge f(u)$. ∎

## Lemma 2 — Transition-map properties

For fixed $u<v$, $T_{u,v}$ is convex, nonincreasing, and lower semicontinuous as an extended-real-valued function.

### Proof

For each $x\in(u,v]$, the defining expression is affine in $p$ with nonpositive slope. A supremum of affine functions is convex and lower semicontinuous, and a supremum of nonincreasing functions is nonincreasing. ∎

## Definition — Continuous-height value functions

Set
$$
V_n(p)=
\begin{cases}
0,&p\ge f(b),\
+\infty,&p<f(b).
\end{cases}
$$

For $i=n-1,\ldots,0$, define
$$
V_i(p)=+\infty
$$
when $p<f(x_i)$, and otherwise
$$
V_i(p)=
\inf_{q\ge T_{x_i,x_{i+1}}(p)}
\left[
\frac{h_i}{2}(p+q)+V_{i+1}(q)
\right].
$$

These are continuous-height value functions: $p$ and $q$ range over real numbers, not a finite height set.

## Theorem 3 — Exact Bellman principle

For every $i$ and $p$, $V_i(p)$ equals the infimum of
$$
\sum_{r=i}^{n-1}\frac{h_r}{2}(y_r+y_{r+1})
$$
over all feasible continuations with $y_i=p$.

Consequently,
$$
V(X)=\inf_{p\in\mathbb R}V_0(p)-\int_a^b f(x)\,dx.
$$

For the fixed-breakpoint problem, the infimum is attained.

### Proof

Induct backward on $i$. The terminal statement is the definition of $V_n$. For the induction step, Lemma 1 gives the exact feasible range of $q=y_{i+1}$. Once $q$ is fixed, the first segment contributes $h_i(p+q)/2$, and the remaining problem is exactly the state $(i+1,q)$. Taking the infimum over feasible $q$ gives the recurrence. At $i=0$, optimizing $p$ gives the fixed-breakpoint optimum. Attainment follows from fixed-breakpoint existence. ∎

## Corollary 4 — Convexity

Every $V_i$ is convex as an extended-real-valued function.

### Proof

$V_n$ is the indicator of the closed convex set $[f(b),\infty)$. Suppose $V_{i+1}$ is convex. The function
$$
(p,q)\mapsto
\frac{h_i}{2}(p+q)+V_{i+1}(q)
$$
is jointly convex, and the feasible set
$$
\{(p,q):p\ge f(x_i),\ q\ge T_{x_i,x_{i+1}}(p)\}
$$
is convex because $T$ is convex. Partial minimization over $q$ preserves convexity. ∎

## Important distinction for free-breakpoint dynamic programming

The convexity statement above applies to a **fixed ordered breakpoint sequence**.

When breakpoint locations are selected from a finite grid, the value at a later grid point is obtained by taking a minimum over possible predecessor breakpoints. A pointwise minimum of convex functions need not be convex.

Therefore this document does **not** justify replacing the free-breakpoint problem by one generic convex one-dimensional search.

The exact breakpoint-grid recurrence in theory/algorithm.md retains the minimum over predecessor breakpoints.

## Continuous heights do not require a height grid

For fixed breakpoints, the exact problem is a finite-dimensional convex optimization problem in the vertex heights with semi-infinite linear constraints. The continuous-height Bellman recurrence is an equivalent dynamic-programming representation.

Thus a height grid is a numerical approximation device, not a mathematical necessity.
