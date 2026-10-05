# One-segment cost

For an interval $[u,v]$, define

```math
C(u,v)=
\min_{\substack{L\text{ affine}\\L(x)\ge f(x)\ \forall x\in[u,v]}}
\int_u^v(L(x)-f(x))\,dx.
```

Write

```math
L(x)=\alpha+\beta x.
```

For a fixed slope $\beta$, the smallest feasible intercept is

```math
\alpha(\beta)
=
\max_{x\in[u,v]}(f(x)-\beta x).
```

Therefore

```math
C(u,v)
=
\min_{\beta\in\mathbb R}
\left[
\alpha(\beta)(v-u)
+
\beta\frac{v^2-u^2}{2}
-
\int_u^v f(x)\,dx
\right].
```

The function

```math
\beta\mapsto\max_{x\in[u,v]}(f(x)-\beta x)
```

is convex, so the one-dimensional objective is convex.

Every optimal majorant touches $f$ somewhere. Otherwise, the line could be shifted downward.

The reference implementation evaluates the maximum of $f(x)-\beta x$ numerically on a dense interval grid and minimizes the resulting convex function in $\beta$. This is intended as a clear baseline implementation; higher-accuracy implementations can replace this routine without changing the DP layer.
