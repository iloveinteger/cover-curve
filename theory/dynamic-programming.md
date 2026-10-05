# Fixed-breakpoint Bellman formulation

Fix

$X=(x_0,\ldots,x_n), \qquad a=x_0<x_1<\cdots<x_n=b,$

and put

$h_i=x_{i+1}-x_i.$

For $u<v$ and $p\in\mathbb R$, define the extended-real-valued function

$T_{u,v}(p) = \sup_{u<x\le v} \frac{(v-u)f(x)-(v-x)p}{x-u}.$

## Lemma 1 — Exact one-segment feasibility

Let $p,q\in\mathbb R$. The affine segment joining $(u,p)$ and $(v,q)$ majorizes $f$ on $[u,v]$ if and only if

$p\ge f(u)$

and

$q\ge T_{u,v}(p).$

### Proof

At $x=u$, feasibility is exactly

$p\ge f(u).$

For $u<x\le v$,

$\frac{v-x}{v-u}p + \frac{x-u}{v-u}q \ge f(x)$

is equivalent to

$q \ge \frac{(v-u)f(x)-(v-x)p}{x-u}.$

Thus feasibility at every $x\in(u,v]$ is equivalent to

$q \ge \sup_{u<x\le v} \frac{(v-u)f(x)-(v-x)p}{x-u} = T_{u,v}(p).$

Combining the endpoint condition with the interior conditions proves the claim. ∎

## Lemma 2 — Properties of the transition map

For fixed $u<v$, $T_{u,v}$ is convex, nonincreasing, and lower semicontinuous as an extended-real-valued function.

### Proof

For each $x\in(u,v]$,

$p \longmapsto \frac{(v-u)f(x)-(v-x)p}{x-u}$

is affine with slope

$-\frac{v-x}{x-u}\le0.$

The supremum of affine functions is convex and lower semicontinuous. Since every member of the family is nonincreasing, its supremum is nonincreasing. ∎

Define

$V_n(p) = \begin{cases} 0,&p\ge f(b),\\ +\infty,&p<f(b). \end{cases}$

For $i=n-1,\ldots,0$, define

$V_i(p)=\begin{cases}\inf_{q\ge T_{x_i,x_{i+1}}(p)}\left[\frac{h_i}{2}(p+q)+V_{i+1}(q)\right],&p\ge f(x_i),\\+\infty,&p<f(x_i).\end{cases}$

## Theorem 3 — Exact Bellman recursion

For each $i$ and each $p\in\mathbb R$, $V_i(p)$ is the infimum of

$\sum_{r=i}^{n-1} \frac{h_r}{2}(y_r+y_{r+1})$

over all $y_i,\ldots,y_n$ satisfying $y_i=p$ and all segment majorant constraints on $[x_r,x_{r+1}]$.

Consequently,

$V(X) = \inf_{p\in\mathbb R}V_0(p) - \int_a^b f(x)\,dx.$

By Theorem 1, this final infimum is attained.

### Proof

The statement is proved by backward induction on $i$.

For $i=n$, there is no remaining segment. The only condition is

$y_n=p\ge f(b),$

so the infimum is $0$ for $p\ge f(b)$ and $+\infty$ otherwise.

Assume the statement holds for $i+1$. Fix $y_i=p$. By Lemma 1, the segment $[x_i,x_{i+1}]$ is feasible exactly when

$p\ge f(x_i)$

and

$y_{i+1}=q\ge T_{x_i,x_{i+1}}(p).$

For a fixed admissible $q$, the first segment contributes

$\frac{h_i}{2}(p+q),$

while the infimum over the remaining segments is $V_{i+1}(q)$ by the induction hypothesis. Taking the infimum over all admissible $q$ gives the stated formula for $V_i(p)$.

At $i=0$, taking the infimum over $y_0$ gives the complete fixed-breakpoint objective after subtracting the constant

$\int_a^b f(x)\,dx.$

Theorem 1 guarantees attainment of the resulting fixed-breakpoint minimum. ∎

## Theorem 4 — Convexity of the value functions

Every $V_i$ is convex as an extended-real-valued function.

### Proof

$V_n$ is the indicator function of the closed convex set

$[f(b),\infty),$

hence is convex.

Assume $V_{i+1}$ is convex. The set

$D_i = \left\{ (p,q): p\ge f(x_i), \quad q\ge T_{x_i,x_{i+1}}(p) \right\}$

is convex because it is the intersection of a half-line constraint with the epigraph of the convex function $T_{x_i,x_{i+1}}$.

The function

$(p,q) \longmapsto \frac{h_i}{2}(p+q)+V_{i+1}(q)$

is convex on $D_i$. Its partial infimum over $q$ is convex. Therefore $V_i$ is convex. ∎

## Corollary 5 — The independent segment-cost DP is not the continuous problem

Let

$D_k(j) = \min_{\substack{0=i_0<\cdots<i_k=j}} \sum_{r=0}^{k-1} C_{\mathrm{ind}}(x_{i_r},x_{i_{r+1}})$

be the scalar dynamic program obtained by independently minimizing the affine majorant on every segment.

In general, $D_n(n)$ is not equal to the fixed-breakpoint value $V(X)$.

### Proof

For a continuous piecewise-affine majorant, the value at every shared breakpoint is a single common variable $y_i$. The exact Bellman recursion therefore carries the current shared height $p$ as a state and imposes

$q\ge T_{x_i,x_{i+1}}(p).$

By contrast, $C_{\mathrm{ind}}(x_i,x_{i+1})$ minimizes over both endpoint heights independently. Adjacent segments in the scalar recursion consequently need not assign the same value to their common breakpoint.

Thus the scalar recursion optimizes over a relaxation that does not enforce continuity at internal breakpoints. The exact recursion is the state-dependent Bellman recursion of Theorem 3. ∎
