# Grid refinement and convergence

Let $E_n^*$ be the optimum when breakpoint positions are continuous, and let $E_{n,N}$ be the exact optimum when all breakpoints must lie on the grid

```math
G_N=\left\{a+\frac{j(b-a)}N:j=0,\ldots,N\right\}.
```

Since every grid-feasible solution is also continuously feasible,

```math
E_n^*\le E_{n,N}.
```

We first prove that the one-segment cost $C(u,v)$ is continuous.

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

Write

```math
m=\frac{u+v}{2},
\qquad
h=\frac{v-u}{2}.
```

Then $u=m-h$ and $v=m+h$. After the change of variables $x=m+hs$,

```math
C(m-h,m+h)
=
\min_{\beta\in\mathbb R} A(m,h,\beta),
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

is continuous. Since $s\in[-1,1]$ is compact, its maximum over $s$ is continuous in $(m,h,\beta)$. The integral term is also continuous. Therefore $A(m,h,\beta)$ is continuous.

It remains to show that the minimization over $\beta\in\mathbb R$ can be restricted to a bounded interval locally.

Since $f$ is bounded, there exists $M>0$ such that

```math
|f(x)|\le M
\qquad\text{for all }x\in[a,b].
```

The maximum in $A$ is at least the larger of its values at $s=1$ and $s=-1$. Hence

```math
\max_{-1\le s\le1}
\left(f(m+hs)-\beta hs\right)
\ge h|\beta|-M.
```

Fix any $(m_0,h_0)$ with $h_0>0$. In a sufficiently small neighborhood of $(m_0,h_0)$, there exists $h_*>0$ such that $h\ge h_*$. Therefore

```math
\max_{-1\le s\le1}
\left(f(m+hs)-\beta hs\right)
\ge h_*|\beta|-M.
```

Thus

```math
A(m,h,\beta)\to+\infty
\qquad\text{as }|\beta|\to\infty,
```

uniformly in that neighborhood.

Consequently, there exists $B>0$ such that a minimizing slope can always be chosen with

```math
|\beta|\le B
```

throughout the neighborhood.

Therefore, locally,

```math
C(m-h,m+h)
=
\min_{|\beta|\le B}A(m,h,\beta).
```

Since $A$ is continuous and $[-B,B]$ is compact, the minimum-value function is continuous. Hence

```math
C(u,v)
```

is continuous for $a\le u<v\le b$.

## Continuity of the total cost

For a breakpoint vector

```math
X=(x_1,\ldots,x_{n-1}),
```

define

```math
J(X)=\sum_{i=0}^{n-1}C(x_i,x_{i+1}),
```

where

```math
a=x_0<x_1<\cdots<x_{n-1}<x_n=b.
```

Since $C$ is continuous, $J$ is continuous on the feasible breakpoint region.

Let

```math
X^*=(x_1^*,\ldots,x_{n-1}^*)
```

be a continuous global optimum. Then

```math
J(X^*)=E_n^*.
```

For each sufficiently large $N$, choose grid points

```math
X_N'=(x_{1,N}',\ldots,x_{n-1,N}')
```

such that $x_{i,N}'\in G_N$ and

```math
x_{i,N}'\to x_i^*
```

for every $i$.

Because the inequalities defining $X^*$ are strict, these grid points can be chosen so that

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

## Convergence of breakpoint locations

Suppose the continuous optimum $X^*$ is unique.

Let $X_N$ be a sequence of grid-optimal breakpoint vectors. Since the breakpoint region is bounded, every subsequence of $X_N$ has an accumulation point. Let $\widehat X$ be one such accumulation point.

By continuity of $J$,

```math
J(\widehat X)
=
\lim_{r\to\infty}J(X_{N_r})
=
\lim_{r\to\infty}E_{n,N_r}
=
E_n^*.
```

Thus $\widehat X$ is a continuous global optimum. By uniqueness,

```math
\widehat X=X^*.
```

Hence every accumulation point is $X^*$, and therefore

```math
\boxed{X_N\to X^*.}
```

If the continuous optimum is not unique, every accumulation point of grid-optimal breakpoint vectors is a continuous global optimum.

Thus refining the breakpoint grid gives a sequence of finite-grid problems whose exact optimal values converge to the optimum of the original continuous problem.
