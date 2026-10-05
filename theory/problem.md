# Problem

Let (f:[a,b]	omathbb R) be continuous, with (a<b), and let (nge1).

An admissible (n)-segment majorant is a continuous piecewise-affine function (g:[a,b]	omathbb R) having breakpoints
[
a=x_0<x_1<cdots<x_n=b
]
and satisfying
[
g(x)ge f(x)qquad(xin[a,b]).
]

Write
[
y_i=g(x_i).
]
On ([x_i,x_{i+1}]), continuity gives the unique affine representation
[
L_i(x)
=
rac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
rac{x-x_i}{x_{i+1}-x_i}y_{i+1}.
]
Thus the majorant condition is
[
rac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
rac{x-x_i}{x_{i+1}-x_i}y_{i+1}
ge f(x)
qquad
(x_ile xle x_{i+1}).
]

The error functional is
[
E(g)=int_a^b(g-f),dx.
]
For fixed breakpoints (X=(x_0,ldots,x_n)), define
[
E_X(y)
=
sum_{i=0}^{n-1}
left[
rac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
int_{x_i}^{x_{i+1}}f(x),dx
ight].
]

Define the fixed-breakpoint value
[
V(X)=min{E_X(y):y	ext{ is feasible for }X}.
]

Finally define
[
E_n^*
=
inf{E(g):g	ext{ is an admissible }n	ext{-segment majorant}}.
]

For fixed (X), the feasible set is
[
mathcal F_X
=
left{
yinmathbb R^{n+1}:
rac{x_{i+1}-x}{x_{i+1}-x_i}y_i+
rac{x-x_i}{x_{i+1}-x_i}y_{i+1}ge f(x)
 orall i, orall xin[x_i,x_{i+1}]
ight}.
]

## Theorem 1 — Existence for fixed breakpoints

For every
[
a=x_0<x_1<cdots<x_n=b,
]
the set (mathcal F_X) is nonempty, closed, and convex, and (V(X)) is attained.

### Proof

Let
[
M=max_{xin[a,b]}f(x).
]
The constant choice
[
y_0=cdots=y_n=M
]
is feasible, so (mathcal F_X
eqarnothing).

Every constraint defining (mathcal F_X) is a closed half-space in (y). Hence (mathcal F_X) is closed and convex.

Put
[
h_i=x_{i+1}-x_i>0.
]
Then
[
E_X(y)
=
sum_{i=0}^{n}c_i y_i-int_a^b f(x),dx,
]
where
[
c_0=rac{h_0}{2},qquad
c_i=rac{h_{i-1}+h_i}{2} (1le ile n-1),qquad
c_n=rac{h_{n-1}}2.
]
Every (c_i) is strictly positive.

Feasibility at the endpoints gives
[
y_ige f(x_i)
qquad(0le ile n).
]
Consequently, every sublevel set
[
{yinmathcal F_X:E_X(y)le C}
]
is bounded above in every coordinate, and it is bounded below by the finite numbers (f(x_i)). It is therefore compact. Since (E_X) is continuous, its minimum on the nonempty feasible set is attained. ∎

## Theorem 2 — Fixed-breakpoint problem is a convex semi-infinite linear program

For fixed (X), (E_X) is linear in (y), and (mathcal F_X) is an intersection of affine half-spaces. Hence the fixed-breakpoint problem is a convex optimization problem, equivalently a linear semi-infinite program. ∎
