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
\mu=\min_{[a,b]}f,
\qquad
M=\max_{[a,b]}f.
$$

The constant function $g\equiv M$ is admissible, so the infimum is finite
and nonnegative.

## 1. Relaxed class

A configuration is a list

$$
P=(P_0,\ldots,P_n),
\qquad
P_i=(x_i,y_i),
$$

with

$$
a=x_0\le x_1\le\cdots\le x_n=b.
$$

Segment $i$ is **regular** if $x_{i-1}<x_i$ and **vertical** if
$x_{i-1}=x_i$. A configuration is feasible if

- on every regular segment, its affine interpolant $\ell_i$ satisfies
  $\ell_i\ge f$ on $[x_{i-1},x_i]$;
- on every vertical segment at $c$, both endpoint values satisfy
  $y_{i-1},y_i\ge f(c)$.

For a regular segment, put

$$
I_i(P)
=
\frac{x_i-x_{i-1}}{2}(y_{i-1}+y_i)
-
\int_{x_{i-1}}^{x_i}f(x)\,dx,
$$

and put $I_i(P)=0$ for a vertical segment. Since the regular intervals cover
$[a,b]$ up to a finite set,

$$
I(P)=\sum_{i=1}^n I_i(P).
$$

Feasibility gives

$$
I_i(P)=\int_{x_{i-1}}^{x_i}(\ell_i-f)\,dx\ge0
$$

for every regular segment, and hence

$$
0\le I_i(P)\le I(P).
\tag{1}
$$

A feasible configuration with no vertical segments represents an admissible
continuous piecewise-affine function with at most $n$ nondegenerate pieces,
with the same cost. Therefore, if $m_{\rm rel}$ denotes the infimum over
feasible configurations,

$$
m_{\rm rel}
\le
\inf_{\substack{g\ge f\\g\text{ admissible}}}I(g).
\tag{2}
$$

The reverse inequality will follow once a relaxed minimizer without vertical
segments is constructed.

## 2. Existence of a relaxed minimizer

Take a minimizing sequence $P^k$ satisfying

$$
I(P^k)\to m_{\rm rel},
\qquad
I(P^k)\le C
$$

for some finite $C$. Since every knot lies on a feasible segment or vertical
segment,

$$
y_i^k\ge f(x_i^k)\ge\mu.
$$

By compactness of $[a,b]$, after passing to a subsequence,

$$
x_i^k\to\bar x_i\in[a,b],
$$

for every $i$. Since the $y_i^k$ are bounded below, pass to a further
subsequence so that

$$
y_i^k\to\bar y_i\in[\mu,+\infty]
$$

for every $i$.

Call knot $i$ **finite** if $\bar y_i<\infty$ and **infinite** otherwise.

### (a) Infinite knots force vanishing widths

For a regular segment $i$, feasibility and
$\int_{x_{i-1}}^{x_i}f\le M(x_i-x_{i-1})$ give, using (1),

$$
C
\ge I_i(P^k)
\ge
w_i^k
\left(
\frac{y_{i-1}^k+y_i^k}{2}-M
\right),
$$

where

$$
w_i^k=x_i^k-x_{i-1}^k.
$$

Because $y_{i-1}^k,y_i^k\ge\mu$, if either endpoint value tends to
$+\infty$, the bracket tends to $+\infty$. Hence

$$
w_i^k\to0.
\tag{3}
$$

Thus every segment incident to an infinite knot has vanishing width.

### (b) At least one finite knot survives

Since

$$
\sum_{i=1}^n w_i^k=b-a>0,
$$

some segment has limiting width strictly positive. By (3), neither endpoint
of such a segment can be infinite. Therefore at least one finite knot exists.

### (c) Construction of the limit configuration

List the finite knots in their original order:

$$
i_1<\cdots<i_r.
$$

Consider two consecutive finite knots $i_j<i_{j+1}$.

If $i_{j+1}=i_j+1$, keep the limiting segment between them. If
$i_{j+1}>i_j+1$, all knots strictly between them are infinite. Every segment
incident to one of these infinite knots has width tending to zero by (3).
In particular,

$$
\bar x_{i_j}=\bar x_{i_{j+1}}=:c.
$$

Replace the whole stretch from $i_j$ to $i_{j+1}$ by one vertical segment at
$c$ joining the finite endpoint values $\bar y_{i_j}$ and
$\bar y_{i_{j+1}}$.

If a maximal run of infinite knots occurs at the left boundary, let $i_1$
be the first finite knot. The segment immediately preceding $i_1$, together
with all segments in the boundary run, has width tending to zero by (3).
Hence $\bar x_{i_1}=a$. Delete that run and start the limit configuration at
$(a,\bar y_{i_1})$. The right boundary is handled symmetrically: if $i_r$
is the last finite knot, then $\bar x_{i_r}=b$, and the boundary run after it
is deleted.

If two consecutive finite knots have the same limiting coordinate, their
limiting segment is vertical and is kept as such. Thus the resulting limit
configuration $\bar P$ is well-defined and has at most $r-1\le n$ segments.
If necessary, repeated points can be inserted to represent it with exactly
$n+1$ points.

### (d) Feasibility of the limit configuration

Consider first a kept regular segment whose limiting width is positive. Its
endpoint coordinates and values converge to finite limits, so its affine
interpolants converge uniformly to the limiting affine function. For every
point in the interior of the limiting interval, that point belongs to the
corresponding approximating interval for all sufficiently large $k$.
Therefore the limiting affine function is at least $f$ on the interior, and
hence on the closed interval by continuity.

Every vertical segment in $\bar P$ has finite endpoint values and both
endpoints converge to the same coordinate $c$. For either endpoint,

$$
y_i^k\ge f(x_i^k),
$$

so continuity of $f$ gives

$$
\bar y_i\ge f(c).
$$

Thus every vertical segment satisfies the required feasibility condition.

Hence $\bar P$ is feasible.

### (e) Cost of the limit

For every kept regular segment, continuity of the endpoint formula and of the
integral with respect to its endpoints gives

$$
I_i(P^k)\to I_i(\bar P).
$$

If a segment has limiting width zero, its cost also tends to zero, since its
endpoint values are finite whenever that segment is retained. Every discarded
segment has nonnegative cost by (1), and every vertical segment has cost zero.
Therefore

$$
I(\bar P)
=
\sum_{\text{kept }i}\lim_{k\to\infty}I_i(P^k)
\le
\liminf_{k\to\infty}I(P^k)
=
m_{\rm rel}.
$$

Since $\bar P$ is feasible,

$$
I(\bar P)\ge m_{\rm rel}.
$$

Consequently,

$$
I(\bar P)=m_{\rm rel},
$$

so the relaxed problem has a minimizer.

## 3. Removing vertical segments

We first normalize a relaxed minimizer without changing its cost or increasing
the number of segments.

- Delete zero-length segments whose two endpoints coincide.
- Merge consecutive vertical segments at the same coordinate into one
  vertical segment. The endpoint values remain at least $f(c)$.
- If a vertical segment occurs at $a$ or $b$, delete it and start or end the
  path at the other endpoint.

After normalization, every remaining vertical segment is an isolated interior
segment, flanked by regular segments.

### Claim

A normalized relaxed minimizer has no vertical segment.

### Proof

Suppose a vertical segment occurs at $c\in(a,b)$, joining values $u$ and $v$,
with

$$
d=|v-u|>0.
$$

The argument is invariant under reflection of the horizontal coordinate.
Thus, replacing $f(x)$ by $f(a+b-x)$ and reflecting the entire configuration if
necessary, we may assume

$$
v>u.
$$

Let the regular segments immediately to the left and right lie on affine
functions $L$ and $R$, respectively, with

$$
L(c)=u,
\qquad
R(c)=v.
$$

Feasibility gives

$$
f(c)\le u.
$$

Let $\eta>0$ be small and set

$$
s=c-\eta,
\qquad
t=c+2\eta.
$$

For sufficiently small $\eta$, these points lie inside the adjacent regular
segments. Replace the three consecutive pieces

$$
L\;|\;\text{vertical}\;|\;R
$$

by

$$
L\;|\;M\;|\;R,
$$

where $M$ is the affine function joining

$$
(s,L(s))
\quad\text{and}\quad
(t,R(t)).
$$

The number of segments remains three.

Let $\sigma_L$ and $\sigma_R$ be the slopes of $L$ and $R$. Since

$$
L(s)=u-\sigma_L\eta,
\qquad
R(t)=v+2\sigma_R\eta,
$$

we obtain

$$
M(c)
=
\frac23L(s)+\frac13R(t)
=
u+\frac d3+O(\eta).
\tag{4}
$$

For sufficiently small $\eta$:

- On $[s,c]$, $M-L$ is affine, vanishes at $s$, and by (4) is positive
  at $c$. Hence $M\ge L\ge f$.
- The endpoint difference of $M$ is
  $d+O(\eta)>0$, so $M$ is increasing. Hence
  $M(x)\ge M(c)=u+d/3+O(\eta)$ on $[c,t]$.
  Since $f$ is continuous at $c$, there is a modulus of continuity $\omega$
  with $\omega(r)\to0$ as $r\to0$ and
  $f(x)\le f(c)+\omega(2\eta)\le u+\omega(2\eta)$ on $[c,t]$.
  Therefore $M\ge f$ on $[c,t]$ for sufficiently small $\eta$.

Thus the modified configuration remains feasible.

It remains to compare costs. The integral of $f$ is unchanged, so only the
areas between the old and new affine pieces matter. From (4),

$$
\int_s^c(M-L)\,dx
=
\frac12\eta
\left(
\frac d3+O(\eta)
\right)
=
\frac{\eta d}{6}+O(\eta^2),
$$

and

$$
\int_c^t(R-M)\,dx
=
\frac12(2\eta)
\left(
\frac{2d}{3}+O(\eta)
\right)
=
\frac{2\eta d}{3}+O(\eta^2).
$$

Hence the total change in the objective is

$$
\frac{\eta d}{6}
-
\frac{2\eta d}{3}
+
O(\eta^2)
=
-\frac{\eta d}{2}+O(\eta^2).
$$

Since $d>0$, this is strictly negative for sufficiently small $\eta$.
This contradicts minimality.

Therefore no vertical segment can occur in a normalized relaxed minimizer.
$\square$

## 4. Conclusion

The relaxed minimizer can therefore be chosen with no vertical segments. It
is then an admissible continuous piecewise-affine function $g\ge f$ with at
most $n$ nondegenerate affine pieces. By (2),

$$
I(g)
=
m_{\rm rel}
\le
\inf_{\substack{g\ge f\\g\text{ admissible}}}I(g)
\le
I(g).
$$

Thus

$$
I(g)
=
\inf_{\substack{g\ge f\\g\text{ admissible}}}I(g),
$$

and the infimum is attained. $\square$

**Remark.** Continuity of the original problem is recovered in Step 3. A
minimizing sequence may otherwise converge to a configuration containing a
vertical jump. The local replacement above removes every nontrivial jump
while decreasing the objective, so such a jump cannot occur in a relaxed
minimizer.
