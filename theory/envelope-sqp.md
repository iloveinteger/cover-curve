# Envelope optimization of breakpoints

## 1. Outer value function

For strict breakpoints
$$\nX=(x_0,\ldots,x_n),\\qquad a=x_0<\cdots<x_n=b,\n$$
define
$$\nV(X)=min_{y\\in mathcal F_X}\n\\left[\nsum_{i=0}^{n-1}\\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})\n-int_a^b f(x),dx\n\\right].\n$$

For fixed $X$, this is a linear semi-infinite program in the shared heights. The free-breakpoint problem is
$$\nE_n^*=min_{a<x_1<\cdots<x_{n-1}<b}V(X).\n$$

The outer problem is generally nonconvex. Envelope-SQP is therefore a local numerical method, not a global solver.

## 2. Fixed-breakpoint oracle

For a finite retained contact set $S$, the cutting-plane LP is
$$\nmin_y c(X)^Ty-int_a^b f(x),dx\n$$
subject to
$$\nf(z)-w_i(z;X)^Tyle0,\\qquad zin S,\n$$
where $w_i$ contains the two interpolation weights on the segment containing $z$.

The separation problem is
$$\nM_i=max_{zin[x_i,x_{i+1}]}{f(z)-L_i(z)}.\n$$

If a certified global separation pass establishes
$$\nM_ile\arepsilon\n\\qquad\ext{for every }i,\n$$
then the returned spline is an $\arepsilon$-majorant. For an exact finite LP optimum $LB$,
$$\nLBle V(X)\\\le LB+\arepsilon(b-a).\n$$

The C++ support search is numerical rather than certified, so this inequality is a conditional numerical certificate.

## 3. Envelope sensitivity

The sensitivity formula is valid only at a differentiability point of the exact value function and under a valid primal-dual envelope/KKT representation. It is not an unconditional formula at active-set changes.

Let
$$\nc(z;X,y)=f(z)-L_i(z)\n$$
for a contact $zin[x_i,x_{i+1}]$, and put
$$\nh=x_{i+1}-x_i,\\qquad d=y_{i+1}-y_i.\n$$

Because
$$\nL_i(z)=y_i+d\\frac{z-x_i}{h},\n$$
direct differentiation gives
$$\n\\frac{\\\partial L_i(z)}{\\\partial x_i}\n=\n-d\\frac{x_{i+1}-z}{h^2},\n\\qquad\n\\frac{\\\partial L_i(z)}{\\\partial x_{i+1}}\n=\n-d\\frac{z-x_i}{h^2}.\n$$

Therefore, for the constraint convention $f-Lle0$,
$$\n\oxed{\n\\frac{\\\partial c}{\\\partial x_i}\n=\nd\\frac{x_{i+1}-z}{h^2},\n\\qquad\n\\frac{\\\partial c}{\\\partial x_{i+1}}\n=\nd\\frac{z-x_i}{h^2}.\n}\n$$

Both signs are positive. This is the sign convention used by the current implementation.

The direct trapezoidal objective contributes, for an interior breakpoint $x_j$,
$$\n\oxed{\n\\frac{\\\partial E}{\\\partial x_j}\n=\n\\frac{y_{j-1}-y_{j+1}}2.\n}\n$$

If an endpoint constraint
$$\nf(x_j)-y_jle0\n$$
is active with multiplier $\mu_j$, its direct breakpoint derivative is
$$\n\mu_j f'(x_j).\n$$

The current API exposes only $f(x)$, so $f'(x_j)$ is estimated by a centered finite difference. Consequently the implemented gradient is a numerical sensitivity direction even when the exact value function is differentiable.

### Proposition — finite-contact envelope formula

Assume, locally in $X$, that the exact fixed-breakpoint problem has a finite active contact set with a primal-dual optimum satisfying the envelope/KKT hypotheses, and that the active set and multipliers admit a differentiable local continuation. Then
$$\n\\nabla_XV(X)\n=\n\\nabla_Xmathcal L(X,y^*,\lambda^*,\mu^*)\n$$
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
$$\nV_{\\\mathrm{num}}(X_{k+1})\n\\\le\nV_{\\\mathrm{num}}(X_k)\n+\n\sigma\alpha_k\\nabla V_{\\\mathrm{num}}(X_k)^Tp_k.\n$$

Since the direction is required to satisfy
$$\n\\nabla V_{\\\mathrm{num}}(X_k)^Tp_k<0,\n$$
the right-hand side is strictly smaller than $V_{\\\mathrm{num}}(X_k)$ whenever $\sigma>0$ and $\alpha_k>0$.

Hence accepted objective values are non-increasing.

Because every returned majorant has nonnegative error up to numerical roundoff, the exact objective is bounded below by zero. Therefore, if infinitely many accepted steps occur and the numerical evaluations are finite, the sequence of accepted objective values has a finite limit.

This does **not** imply convergence of the breakpoint vector, stationarity, or global optimality.

## 6. Exact quadratic reference problems

The two quadratic cases used by the benchmark admit exact finite-$n$ formulas.

### Theorem — convex quadratic

For
$$\nf(x)=x^2,\\qquad xin[0,1],\n$$
the exact optimum with $n$ segments is
$$\n\oxed{E_n^*=\\frac{1}{6n^2}}.\n$$

### Proof

For any fixed breakpoints, feasibility at the endpoints forces
$$\ny_ige f(x_i)=x_i^2.\n$$
The chord through the endpoint values is a majorant because $x^2$ is convex. Any other feasible affine segment has endpoint values no smaller than the chord endpoints, so it lies pointwise above that chord. Thus the optimal segment is the chord.

For a segment of length $h_i$, the chord error is
$$\nint_0^{h_i}\\left(h_i t-t^2\\right),dt\n=\n\\frac{h_i^3}{6}.\n$$
Therefore
$$\nE=\\frac16sum_{i=0}^{n-1}h_i^3,\n\\qquad\nsum_i h_i=1.\n$$
By Jensen's inequality,
$$\nsum_i h_i^3ge nleft(\\frac1n\\right)^3=\\frac1{n^2},\n$$
with equality for $h_i=1/n$. Hence the formula. ∎

### Theorem — concave quadratic

For
$$\nf(x)=-x^2,\\qquad xin[0,1],\n$$
the exact optimum with $n$ segments is
$$\n\\\boxed{E_n^*=\frac{1}{12n^2}}.\n$$

### Proof

On an interval of length $h$, translate the interval to $[0,h]$. The midpoint tangent to $-t^2$ is
$$\nL(t)=-ht+\frac{h^2}{4}.\n$$
Its error is
$$\nL(t)+t^2\n=\n\\left(t-\frac h2\right)^2,\n$$
so
$$\n\int_0^h\bigl(L(t)+t^2\bigr),dt\n=\n\frac{h^3}{12}.\n$$

At both endpoints the tangent has the same excess $h^2/4$ above $-t^2$. Hence for equal adjacent lengths the midpoint tangents agree at their common breakpoint and form a continuous feasible spline.

To see the one-cell lower bound directly, write the error polynomial as
$\nq(t)=L(t)+t^2=t^2+At+B.\n$
Feasibility is exactly $q(t)\ge0$ on $[0,h]$. If $-2h\\\le A\le0$, the minimum of $q$ is at the vertex, so $B\\\ge A^2/4$, and hence
$\n\int_0^h q(t)\,dt\n\\\ge\n\frac{h^3}{3}+\frac{Ah^2}{2}+\frac{A^2h}{4}\n=\n\frac{h\bigl(3(A+h)^2+h^2\bigr)}{12}\n\\\ge\frac{h^3}{12}.\n$
If $A\ge0$, feasibility gives $B\ge0$ and the integral is at least $h^3/3$. If $A\\\le-2h$, feasibility at $t=h$ gives $B\\\ge-Ah-h^2$, and the integral is at least
$\n\frac{h^2(-3A-4h)}6\\\ge\frac{h^3}{3}.\n$
Thus every affine majorant has error at least $h^3/12$, with equality for the midpoint tangent.

Therefore every $n$-segment feasible spline satisfies
$$\nE\\\ge\frac1{12}\sum_{i=0}^{n-1}h_i^3,\n\\\qquad\n\sum_i h_i=1.\n$$
Jensen's inequality gives
$$\n\sum_i h_i^3\\\ge\frac1{n^2}.\n$$
Uniform breakpoints attain equality because the midpoint tangents join continuously. Thus
$$\nE_n^*=\frac1{12n^2}.\n$$
∎

## 7. Curvature-density model

The following local calculation is rigorous, but its use as a global free-breakpoint asymptotic law requires additional approximation-theory arguments and is **not** used as a correctness theorem for Envelope-SQP.

For a constant-sign quadratic model on an interval of length $h$,
$$\nf(x_0+t)=f(x_0)+f'(x_0)t+\\frac12q t^2,\n$$
the best affine majorant has leading error
$$\n\egin{cases}\nq h^3/12,&q>0,\\n|q| h^3/24,&q<0.\n\end{cases}\n$$

Hence the local density model is
$$\nw(x)=c(x)^{1/3}|f''(x)|^{1/3},\n$$
where
$$\nc(x)=\n\egin{cases}\n1/12,&f''(x)>0,\\n1/24,&f''(x)<0.\n\end{cases}\n$$

Equal increments of $\int w$ are therefore a principled asymptotic seed. They are not a finite-$n$ optimality certificate.

For $x^4$ on $[0,1]$, the corresponding formal asymptotic reference is
$$\nn^2E_n^*\sim\n\\left(int_0^1x^{2/3},dx\\right)^3\n=\n\\frac{27}{125}.\n$$

For $\sin x$ on $[0,2\pi]$, the mixed-curvature reference used by the benchmark is
$$\nn^2E_n^*\sim\n\\left[\n\\left(int_0^\pi \sin(x)^{1/3},dx\right)\n\\left(\n\\left(\frac1{12}\right)^{1/3}\n+\n\\left(\frac1{24}\right)^{1/3}\n\right)\n\right]^3.\n$$

These last two formulas are benchmark references, not project-level global-optimality theorems.

## 8. Complexity

Let $d=n-1$, $S$ the number of seeds, $K$ the maximum outer iterations per seed, and $L$ the line-search budget. If one fixed-breakpoint solve costs $C_{\\\mathrm{DH}}(n,\varepsilon)$, then
$$\nT_{\\\mathrm{outer}}\n=\nO(SKLC_{\\\mathrm{DH}}(n,\varepsilon))\n$$
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

For a numerical output $\\\widehat E$ and exact optimum $E_n^*$, it is therefore appropriate to decompose
$$\n|\\\widehat E-E_n^*|\n\\\le\n|\\\widehat E-V(X_{\\\mathrm{out}})|\n+\n|V(X_{\\\mathrm{out}})-E_n^*|.\n$$

The first term is the inner/numerical error at the returned breakpoints. The second is the nonconvex outer optimization gap and cannot be bounded from the local stopping criteria alone.

The asymptotic curvature references are not finite-run error bounds.
