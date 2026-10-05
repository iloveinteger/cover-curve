# One-segment problem

Let (ale u<vle b). The independent one-segment problem is
[
C_{mathrm{ind}}(u,v)
=
min_{substack{L mathrm{affine}\Lge f	ext{ on }[u,v]}}
int_u^v(L-f),dx.
]

Write
[
L(x)=alpha+eta x.
]

## Lemma 1 — Elimination of the intercept

For fixed (eta), the least feasible intercept is
[
alpha(eta)
=
max_{xin[u,v]}(f(x)-eta x).
]

### Proof

The inequality
[
alpha+eta xge f(x)
]
is equivalent to
[
alphage f(x)-eta x
]
for every (xin[u,v]). Since (f) is continuous, the maximum on the compact interval exists. The smallest admissible (alpha) is therefore the stated maximum. ∎

## Corollary 2

[
C_{mathrm{ind}}(u,v)
=
min_{etainmathbb R}
left[
(v-u)max_{xin[u,v]}(f(x)-eta x)
+
etarac{v^2-u^2}{2}
-
int_u^v f(x),dx
ight].
]

### Proof

Substitute the minimizing intercept from Lemma 1 into
[
int_u^v(alpha+eta x-f(x)),dx.
]
∎

## Lemma 3 — Convexity

The objective in Corollary 2 is a convex function of (eta).

### Proof

For each fixed (x), the function
[
etamapsto f(x)-eta x
]
is affine. The pointwise maximum of affine functions is convex. Adding the affine term
[
etarac{v^2-u^2}{2}
]
and the constant (-int_u^v f) preserves convexity. ∎

## Proposition 4 — Relation to the continuous problem

The quantity (C_{mathrm{ind}}(u,v)) is not, in general, the contribution of an interval in the continuous piecewise-linear problem.

For prescribed endpoint heights (p,q), the unique continuous segment is
[
L_{u,v;p,q}(x)
=
rac{v-x}{v-u}p+rac{x-u}{v-u}q,
]
and its conditional cost is
[
C(u,v;p,q)
=
rac{v-u}{2}(p+q)-int_u^v f(x),dx
]
if (L_{u,v;p,q}ge f) on ([u,v]), and (+infty) otherwise.

Thus independent minimization removes the shared-height constraint and is a relaxation of the continuous problem. ∎
