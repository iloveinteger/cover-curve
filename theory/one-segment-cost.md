# Independent one-segment problem

Let $a\le u<v\le b$. Define
$$
C_{\mathrm{ind}}(u,v)
=
\min_{\substack{L\text{ affine}\\L(x)\ge f(x)\ \forall x\in[u,v]}}
\int_u^v(L(x)-f(x))\,dx.
$$

Write
$$
L(x)=\alpha+\beta x.
$$

## Lemma 1 — Elimination of the intercept

For fixed $\beta$, the least feasible intercept is
$$
\alpha(\beta)
=
\max_{x\in[u,v]}(f(x)-\beta x).
$$

### Proof

The condition $L\ge f$ is equivalent to
$$
\alpha\ge f(x)-\beta x
\qquad(x\in[u,v]).
$$
The function $x\mapsto f(x)-\beta x$ is continuous on the compact interval $[u,v]$, so its maximum exists. The least feasible $\alpha$ is therefore the stated maximum. ∎

## Corollary 2

$$
C_{\mathrm{ind}}(u,v)
=
\min_{\beta\in\mathbb R}
\left[
(v-u)\max_{x\in[u,v]}(f(x)-\beta x)
+
\beta\frac{v^2-u^2}{2}
-
\int_u^v f(x)\,dx
\right].
$$

### Proof

For fixed $\beta$, the objective is increasing in $\alpha$, so Lemma 1 gives the minimizing intercept. Substitution yields the formula. ∎

## Lemma 3 — Convexity

The function minimized in Corollary 2 is convex in $\beta$.

### Proof

For each $x\in[u,v]$, $\beta\mapsto f(x)-\beta x$ is affine. Its pointwise supremum is convex. The remaining terms are affine or constant. ∎

## Proposition 4 — Relation to the continuous problem

For prescribed endpoint heights $p,q$, the unique affine segment joining $(u,p)$ and $(v,q)$ is
$$
L_{u,v;p,q}(x)
=
\frac{v-x}{v-u}p+\frac{x-u}{v-u}q.
$$
Its conditional cost is
$$
C(u,v;p,q)
=
\frac{v-u}{2}(p+q)-\int_u^v f(x)\,dx
$$
when $L_{u,v;p,q}\ge f$ on $[u,v]$, and is $+\infty$ otherwise.

Consequently, independent minimization does not impose the shared endpoint heights required by the continuous problem and is, in general, a relaxation of it.

### Proof

The displayed affine function is the unique affine function taking the prescribed endpoint values. Its integral is the trapezoidal area
$$
\frac{v-u}{2}(p+q).
$$
The final statement follows because the independent problem allows its two endpoint values to be chosen freely, whereas in a continuous piecewise-affine majorant each endpoint value is shared by the adjacent segments. ∎
