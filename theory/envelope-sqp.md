# Envelope optimization of breakpoints

## 1. Value-function formulation

For breakpoints
\[
a=x_0<x_1<\cdots<x_n=b,
\]
define
\[
V(x)=\min_y E(x,y),
\qquad
E(x,y)=
\sum_{i=0}^{n-1}\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-\int_a^b f.
\]

For fixed \(x\), the minimization over shared endpoint heights is a
semi-infinite linear program.  The direct-height solver is its numerical
cutting-plane implementation.

The outer problem
\[
\min_{a<x_1<\cdots<x_{n-1}<b}V(x)
\]
is generally nonconvex.  Envelope-SQP/L-BFGS is therefore a local optimizer;
there is no general global-optimality theorem.

## 2. Local L1 majorant constant

Let an interval have length \(h\), and suppose \(f\in C^2\) with
nonzero curvature of constant sign on the interval.  After translating,
scaling \(x=x_0+ht\), Taylor expansion gives
\[
f(x_0+ht)=f(x_0)+hf'(x_0)t+
\frac12f''(x_0)h^2t^2+o(h^2)
\]
uniformly for \(t\in[0,1]\).

The affine and linear Taylor terms can be absorbed into the majorant line.
Thus the leading local problem is the best affine majorant of
\(q t^2/2\).

### Convex case

For \(q>0\), the optimal affine majorant is the chord \(qt/2\). Hence
\[
\int_0^1\left(\frac q2t-\frac q2t^2\right)dt=\frac q{12}.
\]
Therefore
\[
E_I=\frac{f''(x_0)}{12}h^3+o(h^3).
\]

### Concave case

For \(q=-\kappa<0\), the optimal affine majorant is the tangent at
\(t=1/2\):
\[
\ell(t)=-\frac\kappa2t+\frac\kappa4.
\]
Consequently
\[
\int_0^1\left(
-\frac\kappa2t+\frac\kappa4+
\frac\kappa2t^2
\right)dt
=\frac\kappa{24}.
\]
Thus
\[
E_I=\frac{|f''(x_0)|}{24}h^3+o(h^3).
\]

So the correct constants are
\[
c_+=\frac1{12},\qquad c_- =\frac1{24}.
\]
The statement that a tangent is less expensive than the shifted chord is
incorrect: for a quadratic concave function the tangent at the midpoint and
the appropriately shifted chord coincide.  The constant is nevertheless
\(1/24\).

## 3. Optimal knot density

Write
\[
c(x)=
\begin{cases}
1/12,&f''(x)>0,\\
1/24,&f''(x)<0.
\end{cases}
\]

For a fine partition,
\[
E\sim\sum_i c(\xi_i)|f''(\xi_i)|h_i^3.
\]
Set
\[
w(x)=c(x)^{1/3}|f''(x)|^{1/3}.
\]
For \(n\) intervals, Holder's inequality gives
\[
\sum_i w_i h_i^3
\ge
\frac{(\sum_i w_i^{1/3}h_i)^3}{n^2}
\]
in the corresponding discrete form.  Passing to the Riemann limit yields
the lower bound
\[
\liminf_{n\to\infty}n^2E_n^*
\ge
\left(\int_a^b w(x)\,dx\right)^3.
\]

Conversely, choosing breakpoints by equal increments of
\[
\Phi(x)=\int_a^x w(t)\,dt
\]
gives \(\int_{x_i}^{x_{i+1}}w=A/n\), and the local upper expansion gives
\[
\limsup_{n\to\infty}n^2E_n^*
\le A^3,
\qquad
A=\int_a^b c(x)^{1/3}|f''(x)|^{1/3}dx.
\]

Hence, whenever the local expansion is uniform (in particular on a
single-sign curvature interval),
\[
\boxed{
\lim_{n\to\infty}n^2E_n^*
=
\left(
\int_a^b c(x)^{1/3}|f''(x)|^{1/3}dx
\right)^3.
}
\]

For globally convex \(f\), this reduces to
\[
\lim n^2E_n^*
=\frac1{12}
\left(\int_a^b(f'')^{1/3}\right)^3.
\]

For globally concave \(f\),
\[
\lim n^2E_n^*
=\frac1{24}
\left(\int_a^b|f''|^{1/3}\right)^3.
\]

## 4. Finite nondegenerate inflections

Suppose \(f\in C^3\) and has finitely many interior inflections
\(p_j\), with
\[
f''(p_j)=0,\qquad f'''(p_j)\ne0,
\]
and no other curvature zeros.  Away from small neighborhoods of the
\(p_j\), the curvature has a fixed sign and the preceding local argument
applies uniformly.

Near an inflection,
\[
f''(x)=f'''(p_j)(x-p_j)+O((x-p_j)^2),
\]
so the density behaves as
\[
w(x)\asymp |x-p_j|^{1/3}.
\]
Equal-density allocation therefore gives the crossing-cell scale
\[
h=O(n^{-3/4})
\]
(up to constants), and its cubic majorant error is
\(O(h^3\sup|f''|)=O(n^{-3})\) or smaller.  There are only finitely many
cells crossing the inflections, hence their total contribution is
\(o(n^{-2})\).

More formally, remove fixed neighborhoods of the inflections, apply the
single-sign lower/upper estimates there, and then let the neighborhoods
shrink.  The omitted weighted integral tends to zero because
\(|f''|^{1/3}\) is continuous.  The finitely many crossing cells contribute
only lower order.  Thus
\[
\boxed{
\lim_{n\to\infty}n^2E_n^*
=
\left(
\int_a^b c(x)^{1/3}|f''(x)|^{1/3}dx
\right)^3
}
\]
also holds under the stated finite-nondegenerate-inflection assumptions.

The earlier version of this document treated the mixed-curvature formula as
unproved; the argument above supplies the missing leading-order coupling
argument.  A full finite-\(n\) \(O(n^{-3})\) expansion still requires a
higher-order analysis of the inflection cells.

## 5. Fixed-node certification

Let \(LB_x\) be the value of the finite cutting-plane LP. Then
\[
LB_x\le OPT(x).
\]
If the continuous separation oracle certifies
\[
\max_{t\in[x_i,x_{i+1}]}(f(t)-L_i(t))\le\varepsilon
\]
for every segment, then shifting the constructed majorant upward by
\(\varepsilon\) gives a feasible majorant and
\[
\boxed{
LB_x\le OPT(x)\le E(g)+0
\le LB_x+\varepsilon(b-a).
}
\]
Thus the fixed-node problem has a numerical certificate with a rigorous
objective gap, subject to the separation oracle's global certification.

## 6. Envelope derivative

Use the active-contact Lagrangian
\[
\mathcal L=E+\sum_k\lambda_k(f(z_k)-L(z_k))
+\sum_j\mu_j(f(x_j)-y_j).
\]
At a differentiable value-function point, the envelope derivative is obtained
by differentiating this Lagrangian while holding the optimal primal/dual
variables fixed.

For a contact \(z\in[x_i,x_{i+1}]\),
\[
\frac{\partial(f(z)-L(z))}{\partial x_i}
=
\frac{(y_{i+1}-y_i)(x_{i+1}-z)}
{(x_{i+1}-x_i)^2},
\]
and
\[
\frac{\partial(f(z)-L(z))}{\partial x_{i+1}}
=
\frac{(y_{i+1}-y_i)(z-x_i)}
{(x_{i+1}-x_i)^2}.
\]

For an interior breakpoint,
\[
\frac{\partial E}{\partial x_j}
=\frac{y_{j-1}-y_{j+1}}2.
\]

At active-set transitions the value function can be nonsmooth; the computed
vector should then be interpreted as a local sensitivity direction rather
than an everywhere-valid classical gradient.  The outer method has no global
optimality guarantee.

## 7. Fast theory-based initialization

The asymptotic theorem directly gives the breakpoint density
\[
\boxed{
\rho(x)\propto
c(x)^{1/3}|f''(x)|^{1/3}.
}
\]

The implementation now estimates \(f''\) from a small uniform sample and
constructs the inverse cumulative-density partition.  It keeps one uniform
seed as a robustness baseline and uses the curvature-density seed as the
second default seed.  This removes the previous arbitrary power-law seed
from the default path.

The curvature seed is only an initializer; it is not itself claimed to be
the finite-\(n\) global optimum.

## 8. Algorithm

~~~text
x <- uniform seed
x_curv <- inverse-CDF seed using c^(1/3)|f''|^(1/3)

for each selected seed:
    (y, lambda, contacts) <- directHeight(x)
    g <- envelopeGradient(x, y, lambda, contacts)

    if ||g||_inf <= tolerance:
        stop

    p <- L-BFGS(g)
    restrict step to a < x_1 < ... < x_{n-1} < b
    backtrack until V(x) decreases
    update L-BFGS history

return best feasible result
~~~

The expensive global grid solver is not required for the default initialization.

## 9. Guarantees

- fixed breakpoints: certified numerical gap \(\le\varepsilon(b-a)\);
- majorant feasibility: certified by the global separation pass, up to its
  numerical tolerance;
- asymptotic knot density: proved under the stated smoothness/curvature
  assumptions;
- finite nondegenerate inflections: leading \(n^{-2}\) constant is proved;
- outer breakpoint optimization: local only; no general global certificate.
