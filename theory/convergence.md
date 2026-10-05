# Grid refinement and convergence

Let $E_n^*$ be the optimum when breakpoint positions are continuous, and let $E_{n,N}$ be the exact optimum when all breakpoints must lie on the grid

```math
G_N=\left\{a+\frac{j(b-a)}N:j=0,\ldots,N\right\}.
```

Since every grid-feasible solution is also continuously feasible,

```math
E_n^*\le E_{n,N}.
```

We first prove the continuity of the one-segment cost.

## Continuity of the one-segment cost

Recall that

```math
C(u,v)
=
\min_{\beta\in\mathbb R}
\left[
(v-u)\max_{x\in[u,v]}(f(x)-\beta x)
+
\beta\frac{v^2-u^2}{2}
-
\int_u^v f(x)\,dx
\right].
```

For $u<v$, write

```math
m=\frac{u+v}{2},
\qquad
h=\frac{v-u}{2}.
```

Then $u=m-h$ and $v=m+h$. Since

```math
\max_{x\in[u,v]}(f(x)-\beta x)
=
-\beta m+
\max_{-1\le s\le1}(f(m+hs)-\beta hs),
```

the terms involving $\beta m$ cancel, giving

```math
C(m-h,m+h)
=
\min_{\beta\in\mathbb R}A(m,h,\beta),
```

where

```math
A(m,h,\beta)
=
2h\max_{-1\le s\le1}
\left(f(m+hs)-\beta hs\right)
-
\int_{m-h}^{m+h}f(x)\,dx.
```

Since $f$ is continuous on the compact interval $[a,b]$, it is bounded and uniformly continuous.

The function

```math
(m,h,\beta,s)\mapsto f(m+hs)-\beta hs
```

is continuous. Because $[-1,1]$ is compact, the maximum over $s$ is a continuous function of $(m,h,\beta)$. The integral term is also continuous. Therefore $A$ is continuous.

It remains to show that the minimization over $\beta\in\mathbb R$ can be restricted to a bounded interval locally.

Let

```math
M=\max_{x\in[a,b]}|f(x)|.
```

The maximum in $A$ is at least its value at $s=1$ and at $s=-1$. Hence

```math
\max_{-1\le s\le1}
\left(f(m+hs)-\beta hs\right)
\ge h|\beta|-M.
```

Therefore

```math
A(m,h,\beta)
\ge
2h(h|\beta|-M)
-
\int_{m-h}^{m+h}f(x)\,dx.
```

Fix $(m_0,h_0)$ with $h_0>0$. In a sufficiently small neighborhood of $(m_0,h_0)$, there exists $h_*>0$ such that

```math
h\ge h_*.
```

Since

```math
\left|\int_{m-h}^{m+h}f(x)\,dx\right|
\le 2hM
\le 2(b-a)M,
```

we obtain, throughout this neighborhood,

```math
A(m,h,\beta)
\ge
2h_*^2|\beta|-4(b-a)M.
```

Thus

```math
A(m,h,\beta)\to+\infty
\qquad\text{as }|\beta|\to\infty,
```

uniformly in the neighborhood.

Consequently, there exists $B>0$ such that every point in this neighborhood has at least one minimizing slope satisfying

```math
|\beta|\le B.
```

Hence, locally,

```math
C(m-h,m+h)
=
\min_{|\beta|\le B}A(m,h,\beta).
```

The function $A$ is continuous and $[-B,B]$ is compact. Therefore the minimum-value function is continuous. Hence $C(u,v)$ is continuous for

```math
a\le u<v\le b.
```

We will also use the continuous extension

```math
C(u,u)=0.
```

This extension is continuous at $u=v$. Indeed, if $u<v$ and $\ell=v-u$, choose the constant function

```math
L(x)=\max_{t\in[u,v]}f(t).
```

It is a feasible majorant, so

```math
0\le C(u,v)
\le
\ell\left(\max_{t\in[u,v]}f(t)-\min_{t\in[u,v]}f(t)\right).
```

By uniform continuity of $f$, the right-hand side tends to $0$ as $v-u\to0$. Therefore

```math
C(u,v)\to0=C(u,u).
```

Thus $C$ extends continuously to the closed triangle

```math
D=\{(u,v):a\le u\le v\le b\}.
```

## Continuity of the total cost

For a breakpoint vector

```math
X=(x_1,\ldots,x_{n-1}),
```

define

```math
x_0=a,
\qquad
x_n=b,
```

and

```math
J(X)=\sum_{i=0}^{n-1}C(x_i,x_{i+1}).
```

On the closed breakpoint region

```math
\overline D_n
=
\{(x_1,\ldots,x_{n-1}):a\le x_1\le\cdots\le x_{n-1}\le b\},
```

the function $J$ is continuous because every term $C(x_i,x_{i+1})$ is continuous.

The actual feasible breakpoint region is its interior

```math
D_n
=
\{(x_1,\ldots,x_{n-1}):a<x_1<\cdots<x_{n-1}<b\}.
```

A point on the boundary of $\overline D_n$ may contain repeated breakpoints. The corresponding zero-length segments contribute $C(u,u)=0$ and can simply be removed.

Conversely, if fewer than $n$ positive-length segments remain, any one of the positive-length segments can be split into additional nondegenerate segments without changing the piecewise-linear function. Repeating this operation gives exactly $n$ nondegenerate segments with the same cost.

Therefore minimizing $J$ on $\overline D_n$ is equivalent, in terms of the optimal value, to the original problem with exactly $n$ nondegenerate segments. Since $\overline D_n$ is compact and $J$ is continuous, a minimizer exists.

Let

```math
X^*=(x_1^*,\ldots,x_{n-1}^*)
```

be an optimal breakpoint vector for the continuous problem. Then

```math
J(X^*)=E_n^*.
```

## Approximation by grid breakpoints

For each sufficiently large $N$, choose grid points

```math
X_N'=(x_{1,N}',\ldots,x_{n-1,N}')
```

such that

```math
x_{i,N}'\in G_N
```

and

```math
x_{i,N}'\to x_i^*
```

for every $i$.

Because $X^*$ has strictly ordered breakpoints, for all sufficiently large $N$ these grid points can be chosen so that

```math
a<x_{1,N}'<\cdots<x_{n-1,N}'<b.
```

Thus $X_N'$ is a feasible grid breakpoint vector.

By continuity of $J$,

```math
J(X_N')\to J(X^*)=E_n^*.
```

## Convergence of the grid optimum

The grid DP minimizes over all grid-feasible breakpoint vectors. Therefore

```math
E_n^*\le E_{n,N}\le J(X_N').
```

Since

```math
J(X_N')\to E_n^*,
```

the squeeze theorem gives

```math
\boxed{E_{n,N}\to E_n^*.}
```

Thus the exact optimum of the finite-grid problem converges to the optimum of the original continuous problem.

## Convergence of breakpoint locations

Let $X_N$ be a grid-optimal breakpoint vector.

Because every $X_N$ lies in the compact set $\overline D_n$, every subsequence has a further convergent subsequence. Let

```math
X_{N_r}\to\widehat X.
```

By continuity of $J$ on $\overline D_n$,

```math
J(\widehat X)
=
\lim_{r\to\infty}J(X_{N_r}).
```

Since each $X_{N_r}$ is grid-optimal,

```math
J(X_{N_r})=E_{n,N_r}.
```

Therefore

```math
J(\widehat X)
=
\lim_{r\to\infty}E_{n,N_r}
=
E_n^*.
```

Hence $\widehat X$ is a minimizer of $J$ on $\overline D_n$.

If $\widehat X$ contains repeated breakpoints, remove the corresponding zero-length segments and split other positive-length segments as necessary. This produces an exactly $n$-segment feasible solution with the same cost $E_n^*$. Thus every accumulation point of grid-optimal breakpoint vectors represents a global optimum of the original continuous problem.

In particular, suppose that the continuous problem has a unique optimal breakpoint vector $X^*$.

Then every accumulation point of the sequence $X_N$ must represent that same unique optimum. Hence every accumulation point is $X^*$, and therefore

```math
\boxed{X_N\to X^*.}
```

If the continuous optimum is not unique, every accumulation point of grid-optimal breakpoint vectors represents a continuous global optimum.

Thus refining the breakpoint grid gives a sequence of finite-grid problems whose exact optimal values converge to the optimum of the original continuous problem.
