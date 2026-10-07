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

Represent a configuration by

```math
P=(P_0,\ldots,P_n),\qquad P_i=(x_i,y_i),\qquad
a=x_0\le\cdots\le x_n=b.
```

A segment is regular if $x_{i-1}<x_i$ and vertical if $x_{i-1}=x_i$.
Zero-length segments allow configurations with fewer than $n$ effective
segments. The configuration is feasible if every regular affine
interpolant $\ell_i$ satisfies $\ell_i\ge f$, while a vertical segment at
$c$ has both endpoint values at least $f(c)$.

For a regular segment define

```math
I_i(P)=
\frac{x_i-x_{i-1}}2(y_{i-1}+y_i)
-\int_{x_{i-1}}^{x_i}f(x)\,dx,
```

and set $I_i(P)=0$ for vertical segments. Then $I(P)=\sum_i I_i(P)$ and
$I_i(P)\ge0$. Let $m_{\rm rel}$ be the infimum over this relaxed class.
Since every feasible configuration without vertical segments is admissible
for the original problem,

```math
m_{\rm rel}\le
\inf_{g\ge f,\ g\text{ admissible}}I(g).
```

## 2. Existence of a relaxed minimizer

Choose a minimizing sequence $P^k$ with $I(P^k)\to m_{\rm rel}$ and
$I(P^k)\le C$. Since every knot is feasible,

```math
y_i^k\ge f(x_i^k)\ge\mu.
```

After passing to a subsequence,

```math
x_i^k\to\bar x_i\in[a,b],\qquad
y_i^k\to\bar y_i\in[\mu,+\infty].
```

Call a knot finite if $\bar y_i<\infty$ and divergent otherwise. For a
regular segment put

```math
w_i^k=x_i^k-x_{i-1}^k.
```

Since $f\le M$,

```math
I_i(P^k)\ge
w_i^k\left(\frac{y_{i-1}^k+y_i^k}{2}-M\right).
```

Thus any segment having a divergent endpoint has $w_i^k\to0$.

At least one knot is finite; otherwise every $w_i^k\to0$, contradicting

```math
b-a=\sum_{i=1}^n w_i^k>0.
```

Let the finite knots be $i_1<\cdots<i_r$. Every knot before $i_1$ is
divergent, hence

```math
\bar x_{i_1}=a.
```

Similarly, $\bar x_{i_r}=b$, so $r\ge2$.

Consider consecutive finite knots $i_j<i_{j+1}$. If
$\bar x_{i_j}<\bar x_{i_{j+1}}$, no divergent knot can lie between them:
such a knot would force every intervening width to tend to $0$. Hence
$i_{j+1}=i_j+1$, and the corresponding affine segments converge uniformly
to the affine segment joining their limiting endpoints. Since each segment
dominates $f$, the limit does too.

If instead

```math
\bar x_{i_j}=\bar x_{i_{j+1}}=:c,
```

replace all intervening segments by the vertical segment joining
$(c,\bar y_{i_j})$ and $(c,\bar y_{i_{j+1}})$. Continuity of $f$ gives

```math
\bar y_{i_j}\ge f(c),\qquad
\bar y_{i_{j+1}}\ge f(c),
```

so this segment is feasible and has zero cost.

The resulting configuration is feasible, spans $[a,b]$, and has $r-1\le n$
segments. For every retained regular segment its cost converges to the
corresponding limit cost; all discarded costs are nonnegative and all new
vertical costs are zero. Therefore

```math
I(\bar P)\le
\liminf_{k\to\infty}I(P^k)=m_{\rm rel}.
```

By definition of $m_{\rm rel}$, $I(\bar P)\ge m_{\rm rel}$. Hence

```math
I(\bar P)=m_{\rm rel}.
```

Thus a relaxed minimizer exists.

## 3. Removing vertical segments

Merge consecutive vertical segments and delete vertical segments at $a$ or
$b$. These operations preserve feasibility and do not increase cost or
segment count. Delete also any vertical segment whose endpoint values
coincide.

Suppose an interior vertical segment at $c$ has distinct endpoint values.
After reflection in $x=c$ if necessary, write

```math
L(c)=u<R(c)=v,qquad d=v-u>0,qquad f(c)\le u,
```

where $L,R$ are the adjacent regular pieces. Let their widths be
$w_L,w_R>0$. Choose $\eta>0$ such that

```math
\eta<\min\left\{w_L,\frac{w_R}{2}\right\},
```

and set $s=c-\eta$, $t=c+2\eta$. Replace the vertical segment and the
corresponding portions of $L,R$ by the affine segment $M$ joining
$(s,L(s))$ to $(t,R(t))$.

Let $\sigma_L,\sigma_R$ be the slopes of $L,R$. Then

```math
L(s)=u-\sigma_L\eta,\qquad
R(t)=v+2\sigma_R\eta,
```

and

```math
M(c)=u+\frac d3+
\frac23(\sigma_R-\sigma_L)\eta.
```

Set $\varepsilon=d/6$. By continuity of $f$, choose $\delta>0$ such that
$|x-c|<\delta$ implies $|f(x)-f(c)|<\varepsilon$. Choose $\eta$ smaller
if necessary so that

```math
2\eta<\delta,\qquad
\left|\frac23(\sigma_R-\sigma_L)\eta\right|<\varepsilon,
\qquad
d+(\sigma_L+2\sigma_R)\eta>0.
```

Then $M(c)>u+d/6$, and the last inequality makes $M$ increasing. Hence
$M>f$ on $[c,t]$. On $[s,c]$, $M-L$ is affine, vanishes at $s$, and is
positive at $c$, so $M\ge L\ge f$. The replacement is therefore feasible.

Its change in objective is

```math
\Delta I
=
-\frac d2\eta+
(\sigma_R-\sigma_L)\eta^2.
```

The second smallness condition gives
$|\sigma_R-\sigma_L|\eta<d/4$, hence

```math
\Delta I<-\frac d4\eta<0,
```

contradicting minimality. Thus no nontrivial interior vertical segment
exists.

## 4. Conclusion

The relaxed minimizer is therefore an admissible continuous
piecewise-affine function with at most $n$ nondegenerate affine pieces.
Consequently,

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
