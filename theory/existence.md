# Existence of an optimal piecewise-affine upper envelope

**Theorem.** Let $f\in C[a,b]$, $a<b$, and $n\ge1$. Among continuous
piecewise-affine functions $g\ge f$ with at most $n$ nondegenerate affine
pieces, the functional

```math
I(g)=\int_a^b(g-f)\,dx
```

attains its infimum.

Set

```math
\mu=\min_{[a,b]}f,\qquad M=\max_{[a,b]}f.
```

The constant function $g\equiv M$ is admissible, so the infimum is finite.

## 1. Relaxed class

Let

```math
P=(P_0,\ldots,P_n),\qquad P_i=(x_i,y_i),\qquad
a=x_0\le\cdots\le x_n=b.
```

A segment is **regular** if $x_{i-1}<x_i$ and **vertical** if
$x_{i-1}=x_i$. A configuration is feasible if every regular affine
interpolant $\ell_i$ satisfies $\ell_i\ge f$, and every vertical segment
at $c$ has both endpoint values at least $f(c)$.

For a regular segment define

```math
I_i(P)
=
\frac{x_i-x_{i-1}}2(y_{i-1}+y_i)
-
\int_{x_{i-1}}^{x_i}f(x)\,dx,
```

and set $I_i(P)=0$ for vertical segments. Then

```math
I(P)=\sum_{i=1}^n I_i(P),\qquad I_i(P)\ge0.
```

Let $m_{\rm rel}$ be the infimum over the relaxed class. Every feasible
configuration without vertical segments is admissible for the original
problem, hence

```math
m_{\rm rel}\le
\inf_{g\ge f,\ g\text{ admissible}}I(g).
```

## 2. Existence of a relaxed minimizer

Choose a minimizing sequence $P^k$ such that

```math
I(P^k)\to m_{\rm rel},
```

and, after discarding finitely many terms, $I(P^k)\le C$.

Since every knot is feasible,

```math
y_i^k\ge f(x_i^k)\ge\mu.
```

After passing to a subsequence,

```math
x_i^k\to\bar x_i\in[a,b],
\qquad
y_i^k\to\bar y_i\in[\mu,+\infty]
```

for every $i$.

Call $i$ **finite** if $\bar y_i<\infty$ and **divergent** if
$\bar y_i=+\infty$.

For a regular segment put

```math
w_i^k=x_i^k-x_{i-1}^k.
```

Since $f\le M$,

```math
I_i(P^k)
\ge
w_i^k
\left(
\frac{y_{i-1}^k+y_i^k}{2}-M
\right).
```

Thus, if either endpoint of the segment is divergent,

```math
w_i^k\to0.
```

There is at least one finite knot. Otherwise every segment has a divergent
endpoint, so $w_i^k\to0$ for every $i$, and therefore

```math
b-a=\sum_{i=1}^n w_i^k\to0,
```

a contradiction.

List the finite knots as

```math
i_1<\cdots<i_r.
```

Then $i_1>0$ implies every knot $0,\ldots,i_1-1$ is divergent, so every
segment $1,\ldots,i_1$ has a divergent endpoint. Hence

```math
\bar x_{i_1}
=
a+\lim_{k\to\infty}\sum_{i=1}^{i_1}w_i^k
=a.
```

Similarly,

```math
\bar x_{i_r}=b.
```

Fix consecutive finite knots $i_j<i_{j+1}$. If

```math
\bar x_{i_j}<\bar x_{i_{j+1}},
```

there can be no divergent knot between them. Indeed, if
$i_j<q<i_{j+1}$ were divergent, then every segment from $i_j$ through
$i_{j+1}$ would have a divergent endpoint, so all their widths would tend to
zero, giving

```math
\bar x_{i_j}=\bar x_{i_{j+1}},
```

a contradiction. Hence $i_{j+1}=i_j+1$. The corresponding affine
interpolants converge uniformly to the affine interpolant joining
$(\bar x_{i_j},\bar y_{i_j})$ and $(\bar x_{i_{j+1}},\bar y_{i_{j+1}})$,
which is at least $f$.

If instead

```math
\bar x_{i_j}=\bar x_{i_{j+1}}=:c,
```

replace all segments between these two finite knots by one vertical segment
at $c$ joining

```math
(c,\bar y_{i_j})\quad\text{and}\quad(c,\bar y_{i_{j+1}}).
```

Both endpoint values are at least $f(c)$, since

```math
\bar y_{i_j}
=
\lim_{k\to\infty}y_{i_j}^k
\ge
\lim_{k\to\infty}f(x_{i_j}^k)
=f(c),
```

and similarly for $i_{j+1}$. Its cost is $0$.

The resulting configuration consists of the $r$ finite knots joined
consecutively, with either one regular segment or one vertical segment
between each pair. Thus it has $r-1\le n$ segments and spans $[a,b]$.

Let $J$ be the indices of the regular segments of the original sequence
whose limiting endpoints are consecutive finite knots with distinct
limiting $x$-coordinates. For each $i\in J$,

```math
I_i(P^k)\to I_i(\bar P).
```

Every other original segment has nonnegative cost, while every vertical
segment of $\bar P$ has cost $0$. Hence

```math
I(\bar P)
=
\sum_{i\in J}\lim_{k\to\infty}I_i(P^k)
\le
\liminf_{k\to\infty}I(P^k)
=
m_{\rm rel}.
```

Since $\bar P$ is feasible,

```math
I(\bar P)\ge m_{\rm rel}.
```

Therefore

```math
I(\bar P)=m_{\rm rel}.
```

## 3. Removing vertical segments

Normalize a relaxed minimizer by merging consecutive vertical segments and
deleting vertical segments at $a$ or $b$. These operations preserve
feasibility and do not increase the cost or segment count. A vertical
segment whose endpoint values coincide is deleted as well.

Suppose an interior vertical segment at $c$ has distinct endpoint values.
By reflecting the construction in $x=c$ if necessary, write

```math
L(c)=u<R(c)=v,
\qquad d=v-u>0,
\qquad f(c)\le u,
```

where $L$ and $R$ are the adjacent affine pieces.

Choose $\eta>0$ and set

```math
s=c-\eta,\qquad t=c+2\eta.
```

Replace the two adjacent pieces and the vertical segment by $L$, followed
by the affine segment $M$ joining $(s,L(s))$ to $(t,R(t))$, followed by $R$.

Let $\sigma_L,\sigma_R$ be the slopes of $L,R$. Then

```math
L(s)=u-\sigma_L\eta,
\qquad
R(t)=v+2\sigma_R\eta,
```

and

```math
M(c)
=
\frac23L(s)+\frac13R(t)
=
u+\frac d3+\frac23(\sigma_R-\sigma_L)\eta.
```

Set $\varepsilon=d/6$. Choose $\eta>0$ small enough that

```math
2\eta<\delta,
\qquad
\left|\frac23(\sigma_R-\sigma_L)\eta\right|<\varepsilon,
\qquad
d+(\sigma_L+2\sigma_R)\eta>0,
```

where $\delta>0$ satisfies

```math
|x-c|<\delta\Longrightarrow |f(x)-f(c)|<\varepsilon.
```

Then

```math
M(c)>u+\frac d6,
```

and the third condition makes $M$ increasing. Hence, for $c\le x\le t$,

```math
M(x)>u+\frac d6\ge f(c)+\frac d6>f(x).
```

On $[s,c]$, $M-L$ is affine, vanishes at $s$, and is positive at $c$;
therefore $M\ge L\ge f$. Thus the replacement is feasible.

Moreover,

```math
\int_s^c(M-L)\,dx
=
\frac{\eta d}{6}
+
\frac{\eta^2}{3}(\sigma_R-\sigma_L),
```

and

```math
\int_c^t(R-M)\,dx
=
\frac{2\eta d}{3}
-
\frac{2\eta^2}{3}(\sigma_R-\sigma_L).
```

Thus

```math
\Delta I
=
-\frac d2\eta
+
(\sigma_R-\sigma_L)\eta^2.
```

The second smallness condition gives

```math
|\sigma_R-\sigma_L|\eta<\frac d4,
```

so

```math
\Delta I
<
-\frac d4\eta<0,
```

contradicting minimality. Hence a relaxed minimizer has no nontrivial
interior vertical segments.

## 4. Conclusion

The relaxed minimizer is therefore an admissible continuous
piecewise-affine $g\ge f$ with at most $n$ nondegenerate affine pieces.
Hence

```math
I(g)=m_{\rm rel}
\le
\inf_{g\ge f,\ g\text{ admissible}}I(g)
\le I(g),
```

so

```math
I(g)=\inf_{g\ge f,\ g\text{ admissible}}I(g).
```

Therefore the infimum is attained.

$\square$
