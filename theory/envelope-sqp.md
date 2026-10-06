# Envelope optimization of breakpoints

## 1. Outer value function

For strict breakpoints
$$
X=(x_0,\ldots,x_n),\qquad a=x_0<\cdots<x_n=b,
$$
define
$$
V(X)=min_{y\in \mathcal F_X}
\left[
sum_{i=0}^{n-1}\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-int_a^b f(x),dx
\r\right].
$$

For fixed $X$, this is a linear semi-infinite program in the shared heights. The free-breakpoint problem is
$$
E_n^*=min_{a<x_1<\cdots<x_{n-1}<b}V(X).
$$

The outer problem is generally nonconvex. Envelope-SQP is therefore a local numerical method, not a global solver.

## 2. Fixed-breakpoint oracle

For a finite retained contact set $S$, the cutting-plane LP is
$$
min_y c(X)^Ty-int_a^b f(x),dx
$$
subject to
$$
f(z)-w_i(z;X)^Tyle0,\qquad zin S,
$$
where $w_i$ contains the two interpolation weights on the segment containing $z$.

The separation problem is
$$
M_i=max_{zin[x_i,x_{i+1}]}{f(z)-L_i(z)}.
$$

If a certified global separation pass establishes
$$
M_ile\arepsilon
\qquad\ext{for every }i,
$$
then the returned spline is an $\arepsilon$-majorant. For an exact finite LP optimum $LB$,
$$
LBle V(X)\le LB+\arepsilon(b-a).
$$

The C++ support search is numerical rather than certified, so this inequality is a conditional numerical certificate.

## 3. Envelope sensitivity

The sensitivity formula is valid only at a differentiability point of the exact value function and under a valid primal-dual envelope/KKT representation. It is not an unconditional formula at active-set changes.

Let
$$
c(z;X,y)=f(z)-L_i(z)
$$
for a contact $zin[x_i,x_{i+1}]$, and put
$$
h=x_{i+1}-x_i,\qquad d=y_{i+1}-y_i.
$$

Because
$$
L_i(z)=y_i+d\frac{z-x_i}{h},
$$
direct differentiation gives
$$
\frac{\partial L_i(z)}{\partial x_i}
=
-d\frac{x_{i+1}-z}{h^2},
\qquad
\frac{\partial L_i(z)}{\partial x_{i+1}}
=
-d\frac{z-x_i}{h^2}.
$$

Therefore, for the constraint convention $f-Lle0$,
$$
\oxed{
\frac{\partial c}{\partial x_i}
=
d\frac{x_{i+1}-z}{h^2},
\qquad
\frac{\partial c}{\partial x_{i+1}}
=
d\frac{z-x_i}{h^2}.
}
$$

Both signs are positive. This is the sign convention used by the current implementation.

The direct trapezoidal objective contributes, for an interior breakpoint $x_j$,
$$
\oxed{
\frac{\partial E}{\partial x_j}
=
\frac{y_{j-1}-y_{j+1}}2.
}
$$

If an endpoint constraint
$$
f(x_j)-y_jle0
$$
is active with multiplier $\mu_j$, its direct breakpoint derivative is
$$
\mu_j f'(x_j).
$$

The current API exposes only $f(x)$, so $f'(x_j)$ is estimated by a centered finite difference. Consequently the implemented gradient is a numerical sensitivity direction even when the exact value function is differentiable.

### Proposition — finite-contact envelope formula

Assume, locally in $X$, that the exact fixed-breakpoint problem has a finite active contact set with a primal-dual optimum satisfying the envelope/KKT hypotheses, and that the active set and multipliers admit a differentiable local continuation. Then
$$
\nabla_XV(X)
=
\nabla_Xmathcal L(X,y^*,\lambda^*,\mu^*)
$$
with $y^*,\lambda^*,\mu^*$ held fixed in the partial derivative.

In particular, each active contact contributes the two boxed terms above, the trapezoidal objective contributes $(y_{j-1}-y_{j+1})/2$, and active endpoint constraints contribute $\mu_j f'(x_j)$.

### Proof

Under the stated differentiability and KKT/envelope assumptions, the value function is the optimal value of a parameterized constrained optimization problem whose local optimizer and multipliers satisfy the envelope theorem. Differentiating the Lagrangian with respect to the parameter $X$ while holding the optimal primal and dual variables fixed gives the derivative of the value. The displayed contact derivatives follow by direct differentiation of the interpolation formula, and the objective and endpoint derivatives follow from the product rule and the chain rule. ∎

At active-set transitions the hypotheses can fail. The value function may be nonsmooth and the dual multipliers may be nonunique. The implementation therefore treats the computed vector as a search direction, not as a globally valid classical gradient.

## 4. Outer algorithm

The current implementation is L-BFGS-style rather than textbook SQP: it does not solve a quadratic-program subproblem.

~~~text
for each selected seed:
    x <- seed
    current <- DirectHeight(x)
    history <- empty

    repeat at most maxIterations:
        g <- envelopeSensitivity(x, current)

        if ||g||_infinity <= gradientTolerance:
            stop

        p <- L-BFGS(g, history)
        if g dot p >= 0:
            p <- -g

        alpha <- largest ordering-preserving step

        repeat at most lineSearchSteps:
            trial <- x + alpha * p

            if trial is invalid:
                alpha <- alpha / 2
                continue

            candidate <- DirectHeight(trial)

            if candidate.value <= current.value
                    + sufficientDecrease * alpha * (g dot p):
                accept trial
                update L-BFGS history
                x <- trial
                current <- candidate
                break

            alpha <- alpha / 2

        if no trial was accepted:
            stop

return the best result over all seeds
~~~

Every accepted breakpoint vector is re-solved by the fixed-breakpoint inner solver.

## 5. What is actually guaranteed

The following statements follow directly from the implemented acceptance rule.

### Proposition — monotone accepted values

For one seed, every accepted step satisfies
$$
V_{\mathrm{num}}(X_{k+1})
\le
V_{\mathrm{num}}(X_k)
+
\sigma\alpha_k\nabla V_{\mathrm{num}}(X_k)^Tp_k.
$$

Since the direction is required to satisfy
$$
\nabla V_{\mathrm{num}}(X_k)^Tp_k<0,
$$
the right-hand side is strictly smaller than $V_{\mathrm{num}}(X_k)$ whenever $\sigma>0$ and $\alpha_k>0$.

Hence accepted objective values are non-increasing.

Because every returned majorant has nonnegative error up to numerical roundoff, the exact objective is bounded below by zero. Therefore, if infinitely many accepted steps occur and the numerical evaluations are finite, the sequence of accepted objective values has a finite limit.

This does **not** imply convergence of the breakpoint vector, stationarity, or global optimality.

## 6. Exact quadratic reference problems

The two quadratic cases used by the benchmark admit exact finite-$n$ formulas.

### Theorem — convex quadratic

For
$$
f(x)=x^2,\qquad xin[0,1],
$$
the exact optimum with $n$ segments is
$$
\oxed{E_n^*=\frac{1}{6n^2}}.
$$

### Proof

For any fixed breakpoints, feasibility at the endpoints forces
$$
y_ige f(x_i)=x_i^2.
$$
The chord through the endpoint values is a majorant because $x^2$ is convex. Any other feasible affine segment has endpoint values no smaller than the chord endpoints, so it lies pointwise above that chord. Thus the optimal segment is the chord.

For a segment of length $h_i$, the chord error is
$$
int_0^{h_i}\left(h_i t-t^2\r\right),dt
=
\frac{h_i^3}{6}.
$$
Therefore
$$
E=\frac16sum_{i=0}^{n-1}h_i^3,
\qquad
sum_i h_i=1.
$$
By Jensen's inequality,
$$
sum_i h_i^3ge nleft(\frac1n\r\right)^3=\frac1{n^2},
$$
with equality for $h_i=1/n$. Hence the formula. ∎

### Theorem — concave quadratic

For
$$
f(x)=-x^2,\qquad xin[0,1],
$$
the exact optimum with $n$ segments is
$$
\boxed{E_n^*=\frac{1}{12n^2}}.
$$

### Proof

On an interval of length $h$, translate the interval to $[0,h]$. The midpoint tangent to $-t^2$ is
$$
L(t)=-ht+\frac{h^2}{4}.
$$
Its error is
$$
L(t)+t^2
=
\left(t-\frac h2\r\right)^2,
$$
so
$$
\int_0^h\bigl(L(t)+t^2\bigr),dt
=
\frac{h^3}{12}.
$$

At both endpoints the tangent has the same excess $h^2/4$ above $-t^2$. Hence for equal adjacent lengths the midpoint tangents agree at their common breakpoint and form a continuous feasible spline.

To see the one-cell lower bound directly, write the error polynomial as
$
q(t)=L(t)+t^2=t^2+At+B.
$
Feasibility is exactly $q(t)\ge0$ on $[0,h]$. If $-2h\le A\le0$, the minimum of $q$ is at the vertex, so $B\ge A^2/4$, and hence
$
\int_0^h q(t)\,dt
\ge
\frac{h^3}{3}+\frac{Ah^2}{2}+\frac{A^2h}{4}
=
\frac{h\bigl(3(A+h)^2+h^2\bigr)}{12}
\ge\frac{h^3}{12}.
$
If $A\ge0$, feasibility gives $B\ge0$ and the integral is at least $h^3/3$. If $A\le-2h$, feasibility at $t=h$ gives $B\ge-Ah-h^2$, and the integral is at least
$
\frac{h^2(-3A-4h)}6\ge\frac{h^3}{3}.
$
Thus every affine majorant has error at least $h^3/12$, with equality for the midpoint tangent.

Therefore every $n$-segment feasible spline satisfies
$$
E\ge\frac1{12}\sum_{i=0}^{n-1}h_i^3,
\qquad
\sum_i h_i=1.
$$
Jensen's inequality gives
$$
\sum_i h_i^3\ge\frac1{n^2}.
$$
Uniform breakpoints attain equality because the midpoint tangents join continuously. Thus
$$
E_n^*=\frac1{12n^2}.
$$
∎

## 7. Curvature-density model

The following local calculation is rigorous, but its use as a global free-breakpoint asymptotic law requires additional approximation-theory arguments and is **not** used as a correctness theorem for Envelope-SQP.

For a constant-sign quadratic model on an interval of length $h$,
$$
f(x_0+t)=f(x_0)+f'(x_0)t+\frac12q t^2,
$$
the best affine majorant has leading error
$$
\egin{cases}
q h^3/12,&q>0,
|q| h^3/24,&q<0.
\end{cases}
$$

Hence the local density model is
$$
w(x)=c(x)^{1/3}|f''(x)|^{1/3},
$$
where
$$
c(x)=
\egin{cases}
1/12,&f''(x)>0,
1/24,&f''(x)<0.
\end{cases}
$$

Equal increments of $\int w$ are therefore a principled asymptotic seed. They are not a finite-$n$ optimality certificate.

For $x^4$ on $[0,1]$, the corresponding formal asymptotic reference is
$$
n^2E_n^*\sim
\left(int_0^1x^{2/3},dx\r\right)^3
=
\frac{27}{125}.
$$

For $\sin x$ on $[0,2\pi]$, the mixed-curvature reference used by the benchmark is
$$
n^2E_n^*\sim
\left[
\left(int_0^\pi \sin(x)^{1/3},dx\r\right)
\left(
\left(\frac1{12}\r\right)^{1/3}
+
\left(\frac1{24}\r\right)^{1/3}
\r\right)
\r\right]^3.
$$

These last two formulas are benchmark references, not project-level global-optimality theorems.

## 8. Complexity

Let $d=n-1$, $S$ the number of seeds, $K$ the maximum outer iterations per seed, and $L$ the line-search budget. If one fixed-breakpoint solve costs $C_{\mathrm{DH}}(n,\varepsilon)$, then
$$
T_{\mathrm{outer}}
=
O(SKLC_{\mathrm{DH}}(n,\varepsilon))
$$
plus $O(SKLd)$ vector/L-BFGS work per accepted/trial step.

The curvature seed costs $O(M+n)$ arithmetic work for $M$ samples, apart from function evaluations.

There is no polynomial worst-case bound for the complete implementation: the dense simplex inner solver has no polynomial worst-case guarantee, the separation oracle is numerical, and the outer problem is nonconvex.

## 9. Numerical error budget

A finite run has distinct error sources:

1. finite LP/cutting-plane error;
2. separation-oracle error;
3. numerical integration error;
4. finite-difference error in endpoint sensitivities;
5. outer line-search/stopping error;
6. floating-point error;
7. nonconvex optimization error.

For a numerical output $\widehat E$ and exact optimum $E_n^*$, it is therefore appropriate to decompose
$$
|\widehat E-E_n^*|
\le
|\widehat E-V(X_{\mathrm{out}})|
+
|V(X_{\mathrm{out}})-E_n^*|.
$$

The first term is the inner/numerical error at the returned breakpoints. The second is the nonconvex outer optimization gap and cannot be bounded from the local stopping criteria alone.

The asymptotic curvature references are not finite-run error bounds.
