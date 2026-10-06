# Envelope optimization of breakpoints

## 1. Value-function formulation

For breakpoints
\[
a=x_0<x_1<\cdots<x_n=b,
\]
define
\[
V(x)=\min_y E(x,y)
\]
where
\[
E(x,y)
=
\sum_{i=0}^{n-1}
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
\int_a^b f.
\]

For fixed x, the minimization over the shared endpoint heights is a semi-infinite linear program:
\[
L_i(t;x,y)\ge f(t),
\qquad t\in[x_i,x_{i+1}].
\]

The directHeight solver is the numerical cutting-plane implementation of this inner problem.

The outer problem is
\[
\min_{a<x_1<\cdots<x_{n-1}<b}V(x).
\]

This separates the inner convex/linear problem from the generally nonconvex breakpoint problem.

## 2. Lagrangian envelope derivative

Assume the fixed-breakpoint problem has an optimal primal solution and dual multipliers for its active constraints, and that the relevant derivatives exist.

For a contact z in [x_i,x_{i+1}], write
\[
L_i(z)=
\frac{x_{i+1}-z}{h_i}y_i+
\frac{z-x_i}{h_i}y_{i+1},
\qquad h_i=x_{i+1}-x_i.
\]

Use the constraint
\[
c_i(z;x,y)=f(z)-L_i(z)\le0.
\]

Let lambda_k >= 0 be the multiplier of an active contact z_k, and let mu_j >= 0 be the multiplier of the endpoint constraint
\[
f(x_j)-y_j\le0.
\]

The Lagrangian is
\[
\mathcal L
=
E+
\sum_k\lambda_k c_k+
\sum_j\mu_j(f(x_j)-y_j).
\]

At a differentiable value-function point, the envelope derivative is obtained by differentiating this Lagrangian while holding the optimal primal/dual variables fixed. This is the standard sensitivity principle for parametric constrained optimization and semi-infinite programming. citeturn0search0turn0search1

For an interior breakpoint x_j, the objective contribution is
\[
\frac{\partial E}{\partial x_j}
=
\frac{y_{j-1}-y_{j+1}}{2}.
\]

For a contact z in [x_i,x_{i+1}],
\[
\frac{\partial c_i}{\partial x_i}
=
-\frac{(y_{i+1}-y_i)(x_{i+1}-z)}
{h_i^2},
\]
and
\[
\frac{\partial c_i}{\partial x_{i+1}}
=
-\frac{(y_{i+1}-y_i)(z-x_i)}
{h_i^2}.
\]

Stationarity with respect to the endpoint height gives
\[
\mu_j
=
c_j-
\sum_k\lambda_k w_{kj},
\]
where
\[
c_j=\frac{x_{j+1}-x_{j-1}}2
\]
for an interior node and w_kj is the linear interpolation weight of contact k at node j.

Therefore the endpoint contribution is mu_j f'(x_j).

The implementation evaluates this last derivative numerically because the public Function interface exposes function values, not derivatives.

## 3. Guarantees and limitations

The inner cutting-plane LP is the source of the majorant feasibility guarantee, up to the numerical separation tolerance.

The envelope gradient is valid under the usual differentiability, regularity, and multiplier assumptions. At a breakpoint where the optimal active set changes or the value function is nonsmooth, the returned vector should be interpreted as a local sensitivity direction rather than a globally valid classical gradient. Parametric optimization value functions can be nonsmooth even when an optimizer is unique. citeturn0search5

The outer problem is not convex in general. Consequently envelope-SQP/L-BFGS is a local optimizer, not a global-optimality certificate.

The solver therefore provides:
- a cheaper local refinement than repeatedly solving a breakpoint grid;
- a dual/envelope-based direction rather than finite-differencing the entire LP value;
- a feasible breakpoint path preserving a < x_1 < ... < x_{n-1} < b;
- no general theorem that its final point is the global optimum.

Global search can still use breakpointSearch, multiple seeds, or a coarse DP result as an external initializer.

## 4. Algorithm

~~~text
x <- initial breakpoint configuration

repeat:
    (y, lambda, contacts) <- directHeight(x)
    g <- envelopeGradient(x, y, lambda, contacts)

    if ||g||_inf <= tolerance:
        stop

    p <- L-BFGS(g)

    restrict step so a < x_1 < ... < x_{n-1} < b

    backtrack until the fixed-x LP value decreases

    update L-BFGS history

return best feasible result
~~~

The current implementation defaults to a uniform initialization and does not invoke fastGridDP automatically. This is intentional: the new solver is an independent local refinement method rather than a wrapper around the slow global grid solver.

## 5. Relationship to SQP

The current implementation is a safeguarded quasi-Newton/envelope method with SQP-style feasible line search. It does not yet solve the full quadratic-programming subproblem of a textbook SQP method. The name envelopeSQP reflects the intended outer architecture; the implementation deliberately uses L-BFGS because it is simpler and robust for the small breakpoint dimensions used by the project.

A future full-SQP implementation can replace the L-BFGS direction without changing the fixed-breakpoint LP or envelope-gradient layer.

## 6. Validation

For f(x)=x^2 on [0,1], the known optimum is
\[
V_n^*=\frac{1}{6n^2}
\]
with uniform breakpoints. The regression test checks this for n=2 and verifies dense majorant feasibility.

A sine test separately verifies continuous majorant feasibility after outer optimization.
