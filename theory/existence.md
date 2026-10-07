# Existence of an optimal piecewise-affine upper envelope

**Theorem.** Let $f\in C[a,b]$, $a<b$, and $n\ge1$. Among continuous
piecewise-affine functions $g\ge f$ with at most $n$ nondegenerate affine
pieces, the functional

$$
I(g)=\int_a^b(g-f)\,dx
$$

attains its infimum.

Set

$$
\mu=\min_{[a,b]}f, \qquad M=\max_{[a,b]}f.
$$

The constant function $g\equiv M$ is admissible, so the infimum is finite.

## 1. Relaxed class

Let

$$
P=(P_0,\ldots,P_n), \qquad P_i=(x_i,y_i), \qquad a=x_0\le\cdots\le x_n=b.
$$

A segment is **regular** if $x_{i-1}<x_i$ and **vertical** if
$x_{i-1}=x_i$. It is feasible if every regular affine interpolant $\ell_i$
satisfies $\ell_i\ge f$, and every vertical segment at $c$ has both endpoint
values at least $f(c)$.

For a regular segment define

$$
I_i(P) = \frac{x_i-x_{i-1}}{2}(y_{i-1}+y_i) -\int_{x_{i-1}}^{x_i}f(x)\,dx,
$$

and set $I_i(P)=0$ for vertical segments. Then

$$
I(P)=\sum_{i=1}^n I_i(P), \qquad I_i(P)\ge0.
$$

Let $m_{\rm rel}$ be the infimum over this relaxed class. A feasible
configuration without vertical segments is an admissible function for the
original problem, so

$$
m_{\rm rel} \le \inf_{g\ge f,\ g\text{ admissible}}I(g).
$$

## 2. Existence of a relaxed minimizer

Take a minimizing sequence $P^k$ with
$$
I(P^k)\to m_{\rm rel},
$$
and, after discarding finitely many terms, assume
$$
I(P^k)\le C.
$$

Since every knot is feasible,
$$
y_i^k\ge f(x_i^k)\ge\mu.
$$

Because $x_i^k\in[a,b]$, after passing to a subsequence we may assume
$$
x_i^k\to\bar x_i\in[a,b]
$$
for every $i$. Passing to a further subsequence, we may also assume
$$
y_i^k\to\bar y_i\in[\mu,+\infty]
$$
for every $i$.

Call $i$ **finite** if $\bar y_i<\infty$ and **divergent** if
$\bar y_i=+\infty$.

For a regular segment, let
$$
w_i^k=x_i^k-x_{i-1}^k.
$$
Since $f\le M$,
$$
I_i(P^k)
\ge
w_i^k\left(\frac{y_{i-1}^k+y_i^k}{2}-M\right).
$$
Moreover $0\le I_i(P^k)\le C$. Hence, if either endpoint height tends to
$+\infty$, then
$$
w_i^k\to0.
$$

There is at least one finite knot. Otherwise every knot height tends to
$+\infty$, so $w_i^k\to0$ for every $i$. Since there are finitely many
segments,
$$
b-a=\sum_{i=1}^n w_i^k\to0,
$$
a contradiction.

List the finite knots as
$$
i_1<\cdots<i_r.
$$

Consider consecutive finite knots $i_j<i_{j+1}$.

If
$$
\bar x_{i_j}<\bar x_{i_{j+1}},
$$
then there is no divergent knot between them. Indeed, if $r$ is a divergent
knot with $i_j<r<i_{j+1}$, then every knot between $i_j$ and $r$ except
possibly $i_j$ is divergent, and every knot between $r$ and $i_{j+1}$ except
possibly $i_{j+1}$ is divergent. Every segment in these two finite chains has
a divergent endpoint, except possibly the two segments adjacent to the finite
endpoints; those two also have a divergent endpoint. Hence all their widths
tend to zero, and therefore
$$
\bar x_{i_j}=\bar x_{i_{j+1}},
$$
a contradiction. Thus
$$
i_{j+1}=i_j+1.
$$

The corresponding affine interpolants converge uniformly to the affine
interpolant joining
$$
(\bar x_{i_j},\bar y_{i_j})
\quad\text{and}\quad
(\bar x_{i_{j+1}},\bar y_{i_{j+1}}),
$$
and the limiting segment is feasible.

If
$$
\bar x_{i_j}=\bar x_{i_{j+1}}=:c,
$$
then every segment between these two finite knots has width tending to zero.
Replace the whole chain by the vertical segment at $c$ joining
$$
(c,\bar y_{i_j})
\quad\text{and}\quad
(c,\bar y_{i_{j+1}}).
$$
Since
$$
\bar y_{i_j}
=
\lim_{k\to\infty}y_{i_j}^k
\ge
\lim_{k\to\infty}f(x_{i_j}^k)
=f(c),
$$
and similarly
$$
\bar y_{i_{j+1}}\ge f(c),
$$
this vertical segment is feasible. Its cost is $0$.

Every segment in the boundary run before $i_1$ has a divergent endpoint,
except when $i_1=0$, so its width tends to zero. Hence
$$
\bar x_{i_1}
=
a+\lim_{k\to\infty}\sum_{i=1}^{i_1}w_i^k
=a.
$$
Similarly,
$$
\bar x_{i_r}=b.
$$
Thus the resulting configuration spans $[a,b]$ and has at most $n$ segments.

For every retained regular segment of positive limiting width, the endpoint
data converge, so the affine interpolants converge uniformly. Since
$\ell_i^k\ge f$ and $f$ is continuous,
$$
\bar\ell_i\ge f.
$$

For every retained vertical segment at $c$, its endpoint values are at least
$f(c)$ by the preceding argument. Thus the limiting configuration $\bar P$
is feasible.

Let $J$ be the set of retained regular segments having positive limiting
width. For each $i\in J$,
$$
I_i(P^k)\to I_i(\bar P).
$$
All other segment costs are nonnegative, and the vertical segments in $\bar
P$ have cost $0$. Therefore
$$
I(\bar P)
=
\sum_{i\in J}\lim_{k\to\infty}I_i(P^k)
\le
\liminf_{k\to\infty}I(P^k)
=
m_{\rm rel}.
$$
Since $\bar P$ is feasible,
$$
I(\bar P)\ge m_{\rm rel}.
$$
Hence
$$
I(\bar P)=m_{\rm rel}.
$$

Therefore the relaxed problem has a minimizer.

## 3. Removing vertical segments

Normalize a relaxed minimizer by deleting coincident zero-length segments,
merging consecutive vertical segments, and deleting vertical segments at $a$
or $b$. It remains feasible and its cost and segment count do not increase.

Suppose an isolated interior vertical segment at $c$ joins $u$ and $v$,
with $d=|v-u|>0$. Reverse the orientation of the vertical segment if
necessary, and assume $v>u$. Let the adjacent affine pieces be $L$ and $R$,
so

$$
L(c)=u, \qquad R(c)=v, \qquad f(c)\le u.
$$

Choose $\eta>0$ small enough that $s=c-\eta$ and $t=c+2\eta$ lie in the
domains of $L$ and $R$, respectively. Replace $L\,|\,\mathrm{vertical}\,|\,R$
by $L\,|\,M\,|\,R$, where $M$ joins $(s,L(s))$ to $(t,R(t))$.

Let $\sigma_L,\sigma_R$ be the slopes of $L$ and $R$. Then

$$
L(s)=u-\sigma_L\eta, \qquad
R(t)=v+2\sigma_R\eta,
$$

and therefore

$$
M(c)
=
\frac23L(s)+\frac13R(t)
=
u+\frac d3+\frac23(\sigma_R-\sigma_L)\eta.
$$

Set
$$
\varepsilon=\frac d6.
$$
By continuity of $f$ at $c$, there exists $\delta>0$ such that
$$
|x-c|<\delta
\quad\Longrightarrow\quad
|f(x)-f(c)|<\varepsilon.
$$

Now choose $\eta>0$ small enough to satisfy all of
$$
2\eta<\delta,
$$
$$
\left|\frac23(\sigma_R-\sigma_L)\eta\right|<\varepsilon,
$$
and
$$
d+(\sigma_L+2\sigma_R)\eta>0.
$$
Such an $\eta$ exists because $d>0$, the slopes are finite, and all three
conditions hold for every sufficiently small positive $\eta$. We may also
require $s$ and $t$ to remain inside the adjacent affine pieces.

The second condition gives
$$
M(c)>u+\frac d3-\frac d6=u+\frac d6.
$$
Since $M$ is increasing by the third condition,
$$
M(x)\ge M(c)>u+\frac d6
\qquad(c\le x\le t).
$$
On the other hand, for $x\in[c,t]$ we have $|x-c|<\delta$, so
$$
f(x)<f(c)+\frac d6\le u+\frac d6.
$$
Hence
$$
M(x)>f(x)
\qquad(c\le x\le t).
$$

On $[s,c]$, $M-L$ is affine, vanishes at $s$, and is positive at $c$.
Therefore $M\ge L\ge f$ on $[s,c]$. Thus the modified configuration is
feasible.

The change in objective can be computed exactly. Since $M-L$ is affine,
vanishes at $s$, and has value
$$
M(c)-L(c)
=
\frac d3+\frac23(\sigma_R-\sigma_L)\eta
$$
at $c$,

$$
\int_s^c(M-L)\,dx
=
\frac{\eta d}{6}
+
\frac{\eta^2}{3}(\sigma_R-\sigma_L).
$$

Similarly, $R-M$ is affine, vanishes at $t$, and has value
$$
R(c)-M(c)
=
\frac{2d}{3}
-
\frac23(\sigma_R-\sigma_L)\eta
$$
at $c$. Hence

$$
\int_c^t(R-M)\,dx
=
\frac{2\eta d}{3}
-
\frac{2\eta^2}{3}(\sigma_R-\sigma_L).
$$

Therefore the exact change in objective is

$$
\begin{aligned}
\Delta I
&=
\int_s^c(M-L)\,dx
-
\int_c^t(R-M)\,dx \\
&=
-\frac{d}{2}\eta
+
(\sigma_R-\sigma_L)\eta^2.
\end{aligned}
$$

The second condition imposed on $\eta$ gives

$$
|\sigma_R-\sigma_L|\eta<\frac d4.
$$

Consequently,

$$
\Delta I
<
-\frac d2\eta+\frac d4\eta
=
-\frac d4\eta<0.
$$

Thus the modified configuration has strictly smaller objective, contradicting
minimality. Therefore a relaxed minimizer has no vertical segments.

## 4. Conclusion

The relaxed minimizer is therefore an admissible continuous
piecewise-affine $g\ge f$ with at most $n$ nondegenerate affine pieces.
By (2),

$$
I(g) = m_{\rm rel} \le \inf_{g\ge f,\ g\text{ admissible}}I(g) \le I(g).
$$

Hence
$$
I(g) = \inf_{g\ge f,\ g\text{ admissible}}I(g),
$$
so the infimum is attained.

$\square$

**Remark.** The relaxed compactification permits vertical jumps. The local
replacement in Step 3 shows that every nontrivial vertical jump strictly
increases the objective, so an optimizer cannot contain one.
