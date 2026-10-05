# Independent one-segment problem

Let $a\le u<v\le b$.

Define $C_{\mathrm{ind}}(u,v)$ as the minimum of $\int_u^v(L(x)-f(x))\,dx$ over affine $L$ satisfying $L(x)\ge f(x)$ for every $x\in[u,v]$.

Write

$L(x)=\alpha+\beta x.$

## Lemma 1 — Elimination of the intercept

For fixed $\beta$, the least feasible intercept is

$\alpha(\beta) = \max_{x\in[u,v]}(f(x)-\beta x).$

### Proof

The condition $L\ge f$ is equivalent to

$\alpha\ge f(x)-\beta x \qquad (x\in[u,v]).$

The function $x\mapsto f(x)-\beta x$ is continuous on the compact interval $[u,v]$, so its maximum exists. The least feasible $\alpha$ is therefore the stated maximum. ∎

## Corollary 2 — Reduction to one variable

$C_{\mathrm{ind}}(u,v) = \min_{\beta\in\mathbb R} [ (v-u)\max_{x\in[u,v]}(f(x)-\beta x) + \beta\frac{v^2-u^2}{2} - \int_u^v f(x)\,dx  ].$

### Proof

For fixed $\beta$, the objective is increasing in $\alpha$, so Lemma 1 gives the minimizing intercept. Substitution yields the formula. ∎

## Lemma 3 — Convexity of the reduced objective

The function

$\Phi_{u,v}(\beta) = (v-u)\max_{x\in[u,v]}(f(x)-\beta x) + \beta\frac{v^2-u^2}{2} - \int_u^v f(x)\,dx$

is convex in $\beta$.

### Proof

For each $x\in[u,v]$, the map

$\beta\longmapsto f(x)-\beta x$

is affine. Its pointwise supremum is convex. The remaining terms are affine or constant. Hence $\Phi_{u,v}$ is convex. ∎

## Proposition 4 — Existence of the independent minimum

The minimum defining $C_{\mathrm{ind}}(u,v)$ is attained.

### Proof

Let

$F(\beta)=\Phi_{u,v}(\beta).$

For $\beta\to+\infty$, using $x=u$ in the maximum gives

$F(\beta) \ge (v-u)f(u) + \beta\frac{(v-u)^2}{2} - \int_u^v f(x)\,dx,$

so $F(\beta)\to+\infty$.

For $\beta\to-\infty$, using $x=v$ in the maximum gives

$F(\beta) \ge (v-u)f(v) - \beta\frac{(v-u)^2}{2} - \int_u^v f(x)\,dx,$

so again $F(\beta)\to+\infty$.

Thus $F$ is continuous and coercive on $\mathbb R$, and therefore attains its minimum. ∎

## Proposition 5 — Relation to the continuous problem

For prescribed endpoint heights $p,q$, the unique affine segment joining $(u,p)$ and $(v,q)$ is

$L_{u,v;p,q}(x) = \frac{v-x}{v-u}p + \frac{x-u}{v-u}q.$

Its conditional cost is

$C(u,v;p,q) = \frac{v-u}{2}(p+q) - \int_u^v f(x)\,dx$

when $L_{u,v;p,q}\ge f$ on $[u,v]$, and is $+\infty$ otherwise.

Consequently, independent minimization does not impose the shared endpoint heights required by the continuous problem and is, in general, a relaxation of it.

### Proof

The displayed affine function is the unique affine function taking the prescribed endpoint values. Its integral is

$\int_u^v L_{u,v;p,q}(x)\,dx = \frac{v-u}{2}(p+q).$

The final statement follows because the independent problem allows its two endpoint values to be chosen freely, whereas in a continuous piecewise-affine majorant each endpoint value is shared by the adjacent segments. ∎
