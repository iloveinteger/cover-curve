# Breakpoint-grid dynamic programming with continuous heights

The mathematical discretization can be performed **only in the breakpoint locations**. Vertex heights do not need to be discretized.

Let
\[
G_N=\{z_0<\cdots<z_m\},\qquad z_0=a,\quad z_m=b,
\]
with
\[
\delta_N=\max_j(z_{j+1}-z_j)\to0.
\]

For a fixed ordered breakpoint sequence
\[
X=(x_0,\ldots,x_n),\qquad x_i\in G_N,
\]
the vertex heights remain arbitrary real numbers. Let \(E_{n,N}^*\) denote the minimum of the original majorant problem over all continuous piecewise-affine functions with at most \(n\) segments whose breakpoints belong to \(G_N\).

There is **no height-grid parameter** in this definition.

## 1. Continuous-height Bellman recurrence

For \(u<v\) and \(p\in\mathbb R\), define
\[
T_{u,v}(p)=
\sup_{u<x\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}.
\]

By the segment-feasibility lemma, a segment joining \((u,p)\) to \((v,q)\) is a majorant exactly when
\[
p\ge f(u),\qquad q\ge T_{u,v}(p).
\]

For \(k=0,\ldots,n\), grid index \(j\), and real height \(p\), define \(F_k(j,p)\) to be the infimum of the trapezoidal integral of the first \(k\) segments among all feasible grid-breakpoint paths ending at \((z_j,p)\). An impossible state has value \(+\infty\).

The initial condition is
\[
F_0(0,p)=
\begin{cases}
0,&p\ge f(a),\\
+\infty,&p<f(a),
\end{cases}
\]
and \(F_0(j,p)=+\infty\) for \(j>0\).

For \(k<n\),
\[
\boxed{
F_{k+1}(j,q)=
\min_{0\le i<j}
\inf_{\substack{p\in\mathbb R\\p\ge f(z_i)\\q\ge T_{z_i,z_j}(p)}}
\left[
F_k(i,p)+\frac{z_j-z_i}{2}(p+q)
\right].
}
\]

Only transitions leaving enough grid cells for the remaining segments are included.

The breakpoint-grid optimum is
\[
\boxed{
E_{n,N}^*=
\min_{q\in\mathbb R}F_n(m,q)
-\int_a^b f(x)\,dx.
}
\]

This is an exact recurrence. It contains no height discretization.

## 2. Exactness

Every feasible spline with grid breakpoints has a unique sequence of shared vertex heights \((y_0,\ldots,y_n)\). Its consecutive pairs satisfy the segment-feasibility inequalities, so it determines a feasible DP path.

Conversely, every finite sequence of DP transitions gives shared heights at the common breakpoints and hence a continuous piecewise-affine majorant.

The contribution of a segment \([z_i,z_j]\) is exactly
\[
\int_{z_i}^{z_j}L(x)\,dx
=\frac{z_j-z_i}{2}(p+q).
\]

Thus the DP optimizes exactly the trapezoidal integral over the breakpoint-restricted feasible class. Subtracting \(\int f\) gives exactly \(E_{n,N}^*\).

No rounding or approximation of vertex heights is involved.

## 3. Convexity: what is true and what is not

For a **fixed predecessor breakpoint \(i\)**, the inner value function
\[
q\mapsto
\inf_{\substack{p\ge f(z_i)\\q\ge T_{z_i,z_j}(p)}}
\left[
F_k(i,p)+\frac{z_j-z_i}{2}(p+q)
\right]
\]
is convex whenever \(F_k(i,\cdot)\) is convex. This follows because:

1. \(T_{z_i,z_j}\) is convex;
2. its epigraph constraint is convex;
3. the objective is jointly convex in \((p,q)\);
4. partial minimization over \(p\) preserves convexity.

However,
\[
F_{k+1}(j,q)=\min_i(\text{convex function of }q)
\]
is **not generally convex**, because a pointwise minimum of convex functions need not be convex.

Therefore the stronger claim that the complete breakpoint-grid value function is always convex and can always be minimized by one convex one-dimensional search is deliberately not made.

The exact recurrence remains valid regardless.

## 4. A finite continuous height interval is sufficient

Let
\[
m_f=\min_{[a,b]}f,\qquad M_f=\max_{[a,b]}f,
\]
\[
C=(b-a)(M_f-m_f),\qquad
\rho_N=\min_j(z_{j+1}-z_j).
\]

For every fixed breakpoint sequence whose breakpoints lie in \(G_N\), there is an optimal height vector satisfying
\[
\boxed{
m_f\le y_i\le B_N:=
m_f+\frac{4C}{\rho_N}.
}
\]

The lower bound follows from \(y_i=g(x_i)\ge f(x_i)\ge m_f\).

For the upper bound, the constant function \(M_f\) is feasible and has error \(C\). At fixed breakpoints,
\[
E(g)+\int_a^b(f-m_f)
=
\sum_i c_i(y_i-m_f),
\]
where every trapezoidal coefficient satisfies
\[
c_i\ge\frac{\rho_N}{2}.
\]
Since \(\int_a^b(f-m_f)\le C\),
\[
\sum_i c_i(y_i-m_f)\le2C.
\]
Hence
\[
\frac{\rho_N}{2}(y_i-m_f)\le2C,
\]
which gives the stated \(B_N\).

Thus the exact breakpoint-grid DP may restrict all height variables to the bounded continuous interval \([m_f,B_N]\) without changing \(E_{n,N}^*\).

This is a bounded continuous domain, not a height grid.

## 5. Breakpoint-grid convergence

The exact continuous-height breakpoint-grid problem satisfies
\[
E_n^*\le E_{n,N}^*.
\]

If
\[
\delta_N\to0,
\]
then
\[
\boxed{E_{n,N}^*\to E_n^*.}
\]

### Proof

Let \(g^*\) be an optimizer of the original problem and choose an exactly \(n\)-segment representation
\[
a=x_0^*<x_1^*<\cdots<x_n^*=b.
\]

Choose grid points \(z_i^{(N)}\in G_N\) with
\[
z_i^{(N)}\to x_i^*.
\]
For sufficiently large \(N\), their order is preserved.

Let \(p_N\) be the piecewise-affine interpolant of \(g^*\) through
\[
(z_i^{(N)},g^*(z_i^{(N)})).
\]
Since \(g^*\) is continuous and piecewise affine,
\[
\|p_N-g^*\|_\infty\to0.
\]

Set
\[
\varepsilon_N=\|p_N-g^*\|_\infty,\qquad
\widetilde g_N=p_N+\varepsilon_N.
\]
Then
\[
\widetilde g_N\ge g^*\ge f,
\]
so \(\widetilde g_N\) is feasible for the breakpoint-grid problem, and
\[
0\le E(\widetilde g_N)-E(g^*)
\le2(b-a)\varepsilon_N\to0.
\]

Therefore
\[
E_n^*
\le E_{n,N}^*
\le E(\widetilde g_N)
\to E_n^*,
\]
which proves the result.

## 6. Consequence

The mathematical convergence theorem requires only
\[
\boxed{\delta_N\to0}.
\]

There is no height-mesh condition \(\eta_N\to0\), because there is no height grid.

This removes the previous problem in which the admissible height range can grow with \(N\) while the chosen number of height samples grows too slowly.

## 7. Exact mathematics versus numerical implementation

The recurrence above is exact, but it is not automatically a finite-time numerical algorithm for arbitrary continuous black-box \(f\).

A practical implementation must approximate at least:

- the transition supremum \(T_{u,v}\);
- the continuous-height value functions;
- the global minimizations;
- the integral of \(f\).

In particular:

- adaptive height samples are an approximation, not the mathematical definition;
- finite samples of \(f\) do not certify \(T_{u,v}\) for arbitrary continuous black-box \(f\);
- a local one-dimensional optimizer does not by itself prove the global minimum of a pointwise minimum of convex functions.

Therefore no numerical implementation is covered by the mathematical convergence theorem unless its additional approximation and global-search errors are separately controlled.

## 8. Implementation target

The correct mathematical target is
\[
\boxed{
\text{breakpoint discretization}
+
\text{continuous height optimization}
}
\]
rather than
\[
\text{breakpoint discretization}
+
\text{height discretization}.
\]

The former has only the breakpoint mesh \(\delta_N\) as a mathematical discretization parameter.
