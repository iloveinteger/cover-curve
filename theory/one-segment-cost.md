# One-segment cost

For an interval $[u,v]$, define the **independent one-segment relaxation**
\[
C_{\mathrm{ind}}(u,v)=
\min_{\substack{L\text{ affine}\\L(x)\ge f(x)\ \forall x\in[u,v]}}
\int_u^v(L(x)-f(x))\,dx.
\]

Writing
\[
L(x)=\alpha+\beta x,
\]
the smallest feasible intercept for a fixed slope is
\[
\alpha(\beta)
=
\max_{x\in[u,v]}(f(x)-\beta x).
\]

Therefore
\[
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
\]

The map
\[
\beta\mapsto\max_{x\in[u,v]}(f(x)-\beta x)
\]
is convex, so the one-dimensional objective is convex.

Every optimal independent majorant touches $f$ somewhere; otherwise its intercept could be lowered.

## Role in the continuous problem

$C_{\mathrm{ind}}(u,v)$ is **not** the exact contribution of an interval in the original continuous piecewise-linear problem.

In the original problem, the endpoint heights are shared with neighboring segments. If
\[
y_u=g(u),\qquad y_v=g(v),
\]
then the segment is fixed as
\[
L_{u,v;y_u,y_v}(x)
=
y_u+
\frac{y_v-y_u}{v-u}(x-u),
\]
and its cost is
\[
C(u,v;y_u,y_v)
=
\frac{v-u}{2}(y_u+y_v)
-
\int_u^v f(x)\,dx,
\]
provided
\[
L_{u,v;y_u,y_v}(x)\ge f(x)
\quad\forall x\in[u,v].
\]
Otherwise the conditional cost is $+\infty$.

Thus the exact fixed-breakpoint problem is a coupled optimization over shared endpoint heights, not a sum of independent $C_{\mathrm{ind}}$ values.

The existing numerical support/slope routine remains useful as a solver for the independent relaxation and as a diagnostic/reference calculation. It must not be treated as the global segment cost of the continuous problem.
