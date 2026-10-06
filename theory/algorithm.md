# Breakpoint-discretized dynamic programming

The mathematical discretization is performed only in breakpoint locations. Vertex heights remain continuous real variables.

Let
$$
G_N=\{z_0<\cdots<z_m\},\\\qquad z_0=a,\\\quad z_m=b,
$$
and
$$
\delta_N=\max_j(z_{j+1}-z_j),\\\qquad
\rho_N=\min_j(z_{j+1}-z_j).
$$

Let $E_{n,N}^*$ be the optimum over continuous piecewise-affine majorants with at most $n$ segments and all breakpoints in $G_N$.

## 1. Transition and Bellman recurrence

For $u<v$ and $p\in\mathbb R$, define
$$
T_{u,v}(p)=
\sup_{u<x\\\le v}
\frac{(v-u)f(x)-(v-x)p}{x-u}.
$$

A segment from $(u,p)$ to $(v,q)$ is feasible exactly when
$$
p\\\ge f(u),\\\qquad q\\\ge T_{u,v}(p).
$$

Its integral is
$$
A(u,v;p,q)=\frac{v-u}{2}(p+q).
$$

For $k=0,\ldots,n$, grid index $j$, and $q\in\mathbb R$, let $F_k(j,q)$ be the infimum of the trapezoidal integral of all feasible $k$-segment paths ending at $(z_j,q)$. Impossible states have value $+\infty$.

The initial condition is
$$
F_0(0,q)=
\begin{cases}
0,&q\\\ge f(a),
+\infty,&q<f(a),
\end{cases}
\\\qquad
F_0(j,q)=+\infty\\\quad(j>0).
$$

The Bellman recurrence is
$$
\\\boxed{
F_{k+1}(j,q)=
\min_{0\\\le i<j}
\inf_{\substack{p\\\ge f(z_i)\q\\\ge T_{z_i,z_j}(p)}}
\left[
F_k(i,p)+\frac{z_j-z_i}{2}(p+q)
\r\right].
}
$$

Finally,
$$
\\\boxed{
E_{n,N}^*
=
\inf_{q\in\mathbb R}F_n(m,q)-\int_a^b f(x)\,dx.
}
$$

## 2. Exact mathematical pseudocode

The pseudocode is exact in an oracle model: Transition returns $T_{u,v}(p)$, and Infimum computes the indicated exact one-dimensional infimum.

~~~text
BreakpointDP(f, G, n):

    m ← |G| - 1

    F(k, j, q):
        if k = 0:
            if j = 0 and q ≥ f(a):
                return 0
            return +∞

        if j < k:
            return +∞

        best ← +∞

        for i = k-1, ..., j-1:
            candidate ←
                inf over p satisfying
                    p ≥ f(z_i)
                    q ≥ Transition(z_i,z_j,p)
                of
                    F(k-1,i,p)
                    + (z_j-z_i)(p+q)/2

            best ← min(best, candidate)

        return best

    q* ← arginf over q of F(n,m,q)

    return F(n,m,q*) - Integral(f,a,b)
~~~

The height state $q$ is continuous; it is not an index in a finite height table.

## 3. Correctness

Every feasible spline with grid breakpoints determines a sequence of shared vertex heights and hence a feasible DP path. Conversely, every feasible sequence of transitions determines a continuous piecewise-affine majorant.

Each segment contributes exactly
$$
\frac{v-u}{2}(p+q).
$$
Therefore the Bellman objective is exactly the integral of the resulting majorant. Subtracting $\int_a^b f$ gives exactly the original error.

Hence the recurrence returns $E_{n,N}^*$.

## 4. Convexity

For a fixed predecessor breakpoint, the inner value function is convex in the terminal height whenever the preceding value function is convex. This follows from convexity of $T_{u,v}$, convexity of its epigraph, joint convexity of the objective, and preservation of convexity under partial minimization.

However, the minimum over predecessor breakpoints need not be convex, because a pointwise minimum of convex functions need not be convex.

Thus no global convexity of the free-breakpoint value function is assumed.

## 5. Bounded continuous height domain

Let
$$
m_f=\min_{[a,b]}f,\\\qquad M_f=\max_{[a,b]}f,
$$
$$
C=(b-a)(M_f-m_f),\\\qquad
B_N=m_f+\frac{2C}{\rho_N}.
$$

There is an optimum satisfying
$$
\\\boxed{m_f\\\le y_i\\\le B_N.}
$$

The lower bound follows from $y_i\\\ge f(x_i)\\\ge m_f$. The constant majorant $M_f$ has error $C$, so the optimal shifted objective is at most $C$. At fixed breakpoints, after shifting by $m_f$, every trapezoidal coefficient satisfies $c_i\\\ge\rho_N/2$. Therefore, for an optimal shifted height vector,
$
\frac{\rho_N}{2}(y_i-m_f)
\\\le
c_i(y_i-m_f)
\\\le C,
$
and hence
$
y_i\\\le m_f+\frac{2C}{\rho_N}.
$

Thus the height domain is bounded but continuous.

## 6. Breakpoint discretization error

Because the breakpoint-restricted class is a subclass of the original class,
$$
E_n^*\\\le E_{n,N}^*.
$$

Let $g^*$ be an optimal $n$-segment majorant and let $K$ be a Lipschitz constant of $g^*$. For sufficiently fine grids, choose grid points within $\delta_N$ of the breakpoints of $g^*$, preserving their order. Let $p_N$ interpolate $g^*$ at these grid points. Then
$$
\|p_N-g^*\|_\infty\\\le K\delta_N.
$$

Set
$$
\widetilde g_N=p_N+K\delta_N.
$$
Then
$$
\widetilde g_N\\\ge g^*\\\ge f,
\\\qquad
0\\\le\widetilde g_N-g^*\le2K\delta_N.
$$
Hence
$$
0\\\le E_{n,N}^*-E_n^*
\le2(b-a)K\delta_N.
$$

Therefore
$$
\\\boxed{
E_{n,N}^*=E_n^*+O(\delta_N)
}
$$
and, in particular,
$$
\\\boxed{E_{n,N}^*\to E_n^*\\\quad\\\text{as }\delta_N\to0.}
$$

The constant is not uniform over all continuous $f$; it depends on an optimal majorant.

## 7. Complexity

Let $m+1=|G_N|$. There are $O(nm)$ Bellman state-function pairs $(k,j)$, and each considers $O(m)$ predecessor breakpoints. Thus the recurrence performs
$$
\\\boxed{O(nm^2)}
$$
predecessor transition/minimization operations in the exact oracle model.

The number of stored value-function objects is
$$
\\\boxed{O(nm)}.
$$

These are oracle-complexity bounds. Since the height state is continuous, an ordinary finite arithmetic-operation or bit-complexity bound requires a representation for the value functions and exact or certified procedures for the transition supremum and continuous infima. Without such assumptions, a finite numerical-operation count is not justified.

Direct enumeration of breakpoint sequences requires
$$
\b\in om{m-1}{n-1}
$$
continuous-height subproblems. The dynamic program avoids this explicit enumeration.

## 8. Total approximation error

Let $\\\widehat E_{n,N}$ be a numerical approximation to the exact breakpoint-grid optimum. Then
$$
\left|\\\widehat E_{n,N}-E_n^*\r\right|
\\\le
\left|\\\widehat E_{n,N}-E_{n,N}^*\r\right|
+
2(b-a)K\delta_N.
$$

The first term contains errors from numerical approximation of the transition supremum, continuous-height minimizations, integration, and floating-point arithmetic. The second term is the mathematically proved breakpoint-discretization error.

For merely continuous $f$, no universal rate depending only on a modulus of continuity of $f$ is asserted here. The explicit linear bound follows from the piecewise-affine, hence Lipschitz, structure of an optimizer $g^*$.

## 9. Scope

The mathematical results are: continuous real-valued heights are sufficient; the Bellman recurrence is exact on a fixed breakpoint grid; no height grid is required; the height domain can be bounded; breakpoint-grid optima converge to the unrestricted optimum; and the breakpoint error is $O(\delta_N)$.

Finite sampling, local one-dimensional optimization, floating-point tolerances, and curvature-based grid construction are implementation choices. They require separate numerical-error analysis and are not part of the mathematical theorem.
