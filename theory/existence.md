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

(1)

Let $m_{\rm rel}$ be the infimum over this relaxed class. A feasible
configuration without vertical segments is an admissible function for the
original problem, so

$$
m_{\rm rel} \le \inf_{\substack{g\ge f\\g\text{ admissible}}}I(g).
$$

(2)

## 2. Existence of a relaxed minimizer

Take a minimizing sequence $P^k$ with $I(P^k)\le C$. Since every knot is
feasible,

$$
y_i^k\ge f(x_i^k)\ge\mu.
$$

After passing to a subsequence,

$$
x_i^k\to\bar x_i\in[a,b], \qquad y_i^k\to\bar y_i\in[\mu,+\infty].
$$

Call $i$ **finite** if $\bar y_i<\infty$.

For a regular segment, with $w_i^k=x_i^k-x_{i-1}^k$,

$$
I_i(P^k) \ge w_i^k \left( \frac{y_{i-1}^k+y_i^k}{2}-M \right).
$$

Hence, if either endpoint tends to $+\infty$,

$$
w_i^k\to0.
$$

(3)

Since

$$
\sum_i w_i^k=b-a>0,
$$

at least one finite knot exists.

List the finite knots as $i_1<\cdots<i_r$. Between consecutive finite knots,
retain the limiting regular segment when their limiting coordinates differ; if
their coordinates coincide, replace the intervening zero-width chain by a
vertical segment. Infinite runs at the two boundaries have zero total width
and are discarded. This gives a finite relaxed configuration $\bar P$ with
at most $n$ segments.

For every retained regular segment of positive limiting width, the endpoint
data converge and hence the affine interpolants converge uniformly. Passing
to the limit in $\ell_i^k\ge f$ gives $\bar\ell_i\ge f$. For every retained
vertical segment at $c$,

$$
\bar y_i = \lim_k y_i^k \ge \lim_k f(x_i^k) = f(c).
$$

Thus $\bar P$ is feasible.

For each retained regular segment,

$$
I_i(P^k)\to I_i(\bar P),
$$

while every discarded segment has nonnegative cost. Therefore

$$
I(\bar P) \le \liminf_{k\to\infty}I(P^k) = m_{\rm rel}.
$$

By feasibility, $I(\bar P)\ge m_{\rm rel}$, so

$$
I(\bar P)=m_{\rm rel}.
$$

Thus the relaxed problem has a minimizer.

## 3. Removing vertical segments

Normalize a relaxed minimizer by deleting coincident zero-length segments,
merging consecutive vertical segments, and deleting vertical segments at
$a$ or $b$. It remains feasible and its cost and segment count do not
increase.

Suppose an isolated interior vertical segment at $c$ joins $u$ and $v$,
with $d=|v-u|>0$. Reflecting the horizontal coordinate if necessary, assume
$v>u$. Let the adjacent affine pieces be $L$ and $R$, so

$$
L(c)=u, \qquad R(c)=v, \qquad f(c)\le u.
$$

For small $\eta>0$, set

$$
s=c-\eta, \qquad t=c+2\eta,
$$

and replace $L\,|\,\mathrm{vertical}\,|\,R$ by $L\,|\,M\,|\,R$, where $M$
joins $(s,L(s))$ to $(t,R(t))$.

If $\sigma_L,\sigma_R$ are the adjacent slopes, then

$$
L(s)=u-\sigma_L\eta, \qquad R(t)=v+2\sigma_R\eta,
$$

and therefore

$$
M(c) = \frac23L(s)+\frac13R(t) = u+\frac d3+O(\eta).
$$

(4)

For sufficiently small $\eta$, $M\ge L\ge f$ on $[s,c]$. Also $M$ is
increasing because its endpoint difference is $d+O(\eta)>0$. By continuity
of $f$ at $c$,

$$
f(x)\le f(c)+o(1)\le u+o(1) \qquad (x\in[c,t]),
$$

while

$$
M(x)\ge u+\frac d3+o(1).
$$

Hence $M\ge f$ on $[c,t]$ for sufficiently small $\eta$.

The change in objective is

$$
\int_s^c(M-L)\,dx-\int_c^t(R-M)\,dx
= -\frac{\eta d}{2}+O(\eta^2)<0.
$$

This contradicts minimality. Therefore a relaxed minimizer has no vertical
segments.

## 4. Conclusion

The relaxed minimizer is therefore an admissible continuous
piecewise-affine $g\ge f$ with at most $n$ nondegenerate affine pieces.
By (2),

$$
I(g) = m_{\rm rel} \le \inf_{\substack{g\ge f\\g\text{ admissible}}}I(g) \le I(g).
$$

Hence

$$
I(g) = \inf_{\substack{g\ge f\\g\text{ admissible}}}I(g),
$$

so the infimum is attained.

$\square$

**Remark.** The relaxed compactification permits vertical jumps. The local
replacement in Step 3 shows that every nontrivial vertical jump strictly
increases the objective, so an optimizer cannot contain one.
