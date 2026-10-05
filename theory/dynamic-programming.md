# Fixed-breakpoint dynamic programming

Fix
[
X=(x_0,ldots,x_n),
qquad
a=x_0<cdots<x_n=b,
]
and put
[
h_i=x_{i+1}-x_i.
]

For (u<v) and (yinmathbb R), define the extended-real transition map
[
T_{u,v}(y)
=
sup_{u<xle v}
rac{(v-u)f(x)-(v-x)y}{x-u}.
]

## Lemma 1 — Exact one-segment feasibility

Let (p,qinmathbb R). The affine segment joining ((u,p)) to ((v,q)) majorizes (f) on ([u,v]) if and only if
[
pge f(u)
qquad	ext{and}qquad
qge T_{u,v}(p).
]

### Proof

At (x=u), feasibility is exactly (pge f(u)).

For (u<xle v),
[
rac{v-x}{v-u}p+rac{x-u}{v-u}qge f(x)
]
is equivalent, since (x-u>0), to
[
qge
rac{(v-u)f(x)-(v-x)p}{x-u}.
]
Therefore feasibility for every (xin(u,v]) is equivalent to
[
qge
sup_{u<xle v}
rac{(v-u)f(x)-(v-x)p}{x-u}
=
T_{u,v}(p).
]
Together with the endpoint condition at (u), this proves the equivalence. ∎

## Lemma 2 — Convexity and monotonicity of the transition map

For fixed (u<v), (T_{u,v}) is an extended-real-valued convex, nonincreasing, lower-semicontinuous function.

### Proof

For each (xin(u,v]),
[
pmapsto
rac{(v-u)f(x)-(v-x)p}{x-u}
]
is affine with slope
[
-rac{v-x}{x-u}le0.
]
The supremum of affine functions is convex and lower semicontinuous. Since every affine function in the supremum is nonincreasing, their supremum is nonincreasing. ∎

Define the terminal value function
[
V_n(y)=
egin{cases}
0,&yge f(b),\
+infty,&y<f(b).
end{cases}
]
For (i=n-1,ldots,0), define
[
V_i(y)=
egin{cases}
displaystyle
inf_{qge T_{x_i,x_{i+1}}(y)}
left[
rac{h_i}{2}(y+q)+V_{i+1}(q)
ight],
&yge f(x_i),\[2ex]
+infty,&y<f(x_i).
end{cases}
]

## Theorem 3 — Exact functional Bellman recursion

For every (i), (V_i(y)) equals the minimum downstream area
[
sum_{r=i}^{n-1}
left[
rac{h_r}{2}(y_r+y_{r+1})
-int_{x_r}^{x_{r+1}}f(x),dx
ight]
]
over all feasible suffixes with (y_i=y), after omitting the fixed integral terms from the definition above.

Consequently,
[
V(X)
=
V_0^*(x_0)-int_a^b f(x),dx,
qquad
V_0^*=inf_{yinmathbb R}V_0(y).
]

### Proof

For the last vertex, feasibility requires (y_nge f(b)), giving (V_n).

Assume the statement holds for (i+1). Once (y_i=y) is fixed, Lemma 1 says that the first segment is feasible exactly when
[
yge f(x_i),
qquad
y_{i+1}ge T_{x_i,x_{i+1}}(y).
]
For a chosen (y_{i+1}=q), the first segment contributes
[
rac{h_i}{2}(y+q),
]
and the minimum feasible cost of the remaining suffix is (V_{i+1}(q)) by the induction hypothesis. Minimizing over all admissible (q) gives exactly the displayed recursion.

At (i=0), minimizing over (y_0) gives the complete fixed-breakpoint objective, with the common constant (int_a^b f) restored. ∎

## Theorem 4 — Convexity of the value functions

Every (V_i) is convex as an extended-real-valued function.

### Proof

(V_n) is the indicator function of the closed half-line ([f(b),infty)), hence convex.

Assume (V_{i+1}) is convex. The set
[
D_i=
{(y,q):yge f(x_i), qge T_{x_i,x_{i+1}}(y)}
]
is convex because it is the intersection of a half-space with the epigraph of the convex function (T_{x_i,x_{i+1}}).

On (D_i), the function
[
(y,q)mapsto rac{h_i}{2}(y+q)+V_{i+1}(q)
]
is convex. The partial infimum of a convex function over a convex set is convex. Hence (V_i) is convex. ∎
