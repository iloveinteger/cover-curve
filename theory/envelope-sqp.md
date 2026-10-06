# Envelope optimization of breakpoints

## 1. Outer value function

For strict breakpoints
\[
X=(x_0,\ldots,x_n),\qquad a=x_0<\cdots<x_n=b,
\]
define
\[
V(X)=\min_{y\in\mathcal F_X}
\left[
\sum_{i=0}^{n-1}\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-\int_a^b f
\right].
\]

For fixed \(X\), this is a linear semi-infinite program in \(y\). The implementation directHeightSolveDetailed is a numerical cutting-plane method for this inner problem.

The free-breakpoint problem is
\[
E_n^*=\min_{a<x_1<\cdots<x_{n-1}<b}V(X).
\]
It is generally nonconvex. Therefore the Envelope-SQP implementation is a local numerical optimizer, not a globally correct algorithm for arbitrary \(f\).

## 2. Fixed-breakpoint oracle

For a finite set of contact points, the LP is
\[
\min_y c(X)^Ty-\int_a^b f,
\]
subject to
\[
f(z)-w_i(z;X)^Ty\le0,
\]
where \(w_i\) contains the two linear interpolation weights on segment \(i\).

The separation oracle searches
\[
\max_{z\in[x_i,x_{i+1}]}\{f(z)-L_i(z)\}.
\]
If the maximum is at most \(\varepsilon\), then the returned spline is an \(\varepsilon\)-majorant.

## 3. Envelope derivative

Assume \(V\) is differentiable at \(X\), and let \((y,\lambda,\mu)\) be a primal/dual optimum satisfying the usual LP regularity conditions. Here \(\lambda_k\ge0\) are multipliers for contact constraints
\[
f(z_k)-L(z_k)\le0,
\]
and \(\mu_j\ge0\) are endpoint multipliers for
\[
f(x_j)-y_j\le0.
\]

The Lagrangian is
\[
\mathcal L
=E(X,y)+\sum_k\lambda_k(f(z_k)-L(z_k))
+\sum_j\mu_j(f(x_j)-y_j).
\]

At a differentiability point, the envelope theorem gives
\[
\nabla_XV=\nabla_X\mathcal L
\]
with the optimal primal/dual variables held fixed.

For a contact \(z\in[x_i,x_{i+1}]\), \(h=x_{i+1}-x_i\),
\[
\frac{\partial(f(z)-L(z))}{\partial x_i}
=
\frac{(y_{i+1}-y_i)(x_{i+1}-z)}{h^2},
\]
\[
\frac{\partial(f(z)-L(z))}{\partial x_{i+1}}
=
\frac{(y_{i+1}-y_i)(z-x_i)}{h^2}.
\]

Both signs are positive: differentiating the interpolation line with respect to either endpoint gives a negative contribution to \(L\), hence a positive contribution to \(f-L\).

The direct objective contributes, for an interior breakpoint,
\[
\frac{\partial E}{\partial x_j}
=\frac{y_{j-1}-y_{j+1}}2.
\]

For the endpoint constraint \(f(x_j)-y_j\le0\),
\[
\frac{\partial}{\partial x_j}[f(x_j)-y_j]=f'(x_j).
\]
The current API accepts only a value function \(f(x)\), so the implementation estimates this term by a centered finite difference.

At active-set transitions \(V\) can be nonsmooth and the LP dual can be nonunique. The computed vector is then a numerical sensitivity direction, not an everywhere-valid classical gradient.

## 4. Outer algorithm

The implementation is L-BFGS-style rather than textbook SQP: it does not solve a quadratic-program subproblem. The name Envelope-SQP describes the envelope-based constrained outer architecture.

Pseudocode:

    for each selected seed:
        x <- seed
        current <- DirectHeight(x)
        history <- empty

        repeat at most maxIterations:
            g <- envelopeSensitivity(x, current)
            if ||g||_infinity <= gradientTolerance:
                break

            p <- L-BFGS(g, history)
            if g dot p >= 0:
                p <- -g

            alpha <- largest ordering-preserving step

            backtrack:
                trial <- x + alpha*p
                if trial is invalid:
                    alpha <- alpha/2
                    continue

                candidate <- DirectHeight(trial)

                if V(trial) <= V(x)
                    + sufficientDecrease * alpha * (g dot p):
                    accept trial
                    update history
                    x <- trial
                    current <- candidate
                    break

                alpha <- alpha/2

            if no trial is accepted:
                break

        return the best result over all seeds

Every accepted step is reevaluated by the fixed-breakpoint solver.

## 5. Convergence statement

There is no general global-convergence theorem for this implementation.

What follows directly from the code is conditional:

1. every accepted step satisfies the configured sufficient-decrease test;
2. therefore objective values along one seed are non-increasing;
3. the objective is bounded below by zero;
4. if infinitely many accepted steps occur, the objective values converge to a finite limit.

This does not imply convergence of the breakpoint vector, stationarity, or global optimality. Those conclusions require additional assumptions that are not enforced by the implementation.

## 6. Curvature-density seed

On a constant-sign \(C^2\) interval, the local majorant error is
\[
c(x)|f''(x)|h^3+o(h^3),
\qquad
c(x)=
\begin{cases}
1/12,&f''(x)>0,\\
1/24,&f''(x)<0.
\end{cases}
\]

Balancing the leading term over \(n\) cells gives
\[
\boxed{\rho(x)\propto c(x)^{1/3}|f''(x)|^{1/3}}.
\]

The implementation estimates \(f''\) by a centered three-point difference, integrates this density numerically, and inverts its cumulative distribution. This is an initializer, not a finite-\(n\) optimality theorem.

## 7. Local asymptotic constant

If \(f\in C^2\) and \(f''\) has one strict sign on \([a,b]\), then
\[
\boxed{
\lim_{n\to\infty}n^2E_n^*
=
\left(
\int_a^b c^{1/3}|f''(x)|^{1/3}\,dx
\right)^3.
}
\]

For one cell of length \(h\), Taylor expansion reduces the leading problem to an affine majorant of \(qt^2/2\). For \(q>0\), the optimal majorant is \(qt/2\), with error \(q/12\). For \(q=-\kappa<0\), the optimal majorant is the midpoint tangent
\[
-\kappa t/2+\kappa/8,
\]
with error \(\kappa/24\).

Put \(w=c^{1/3}|f''|^{1/3}\). The discrete leading term is \(\sum_i w_i^3h_i^3\). Hölder gives
\[
\sum_i w_i^3h_i^3
\ge
\frac{\left(\sum_i w_i h_i\right)^3}{n^2}.
\]
For fine partitions \(\sum_i w_i h_i\to\int_a^b w\), giving the lower bound. Equal increments of
\[
\Phi(x)=\int_a^xw(t)\,dt
\]
give the matching upper bound. Hence the displayed limit.

## 8. Mixed curvature

A completely unconditional mixed-sign theorem is not claimed for arbitrary \(C^2\) functions.

A sufficient assumption for the formula used in the large-\(n\) benchmark is:

- \(f\in C^3([a,b])\);
- \(f''\) has finitely many isolated zeros.

Then
\[
\boxed{
\lim_{n\to\infty}n^2E_n^*
=
\left(
\int_a^b
c(x)^{1/3}|f''(x)|^{1/3}\,dx
\right)^3,
}
\]
where \(c=1/12\) on \(f''>0\) and \(c=1/24\) on \(f''<0\).

Proof idea with the required estimates: remove the finitely many cells crossing zeros of \(f''\). On every remaining cell the constant-sign expansion applies. Near an isolated zero, \(f''(x)=O(|x-x_0|)\) because \(f\in C^3\); the quadratic term therefore vanishes to first order and the local majorant error is \(O(h^4)\). The density quantile construction makes the finitely many crossing-cell contributions \(o(n^{-2})\), while their density mass is \(o(1)\). Applying the constant-sign lower bound to the remaining cells and the density construction for the upper bound yields the same liminf and limsup.

This proof is intentionally stated with these explicit regularity assumptions. No universal finite-\(n\) \(O(n^{-3})\) remainder is claimed.

## 9. Complexity

Let \(d=n-1\), \(S\) be the number of seeds, \(K\) the maximum accepted outer iterations per seed, and \(L\) the line-search budget. If one fixed-breakpoint solve costs \(C_{\mathrm{DH}}(n,\varepsilon)\), then
\[
T_{\mathrm{outer}}
=
O(SKLC_{\mathrm{DH}}(n,\varepsilon))
\]
plus \(O(SKnd)\) vector/L-BFGS work.

The implementation has no polynomial worst-case bound: the inner simplex method has no polynomial worst-case guarantee, and the separation oracle is numerical.

The curvature seed costs \(O(M+n)\) arithmetic work for \(M\) samples, apart from the cost of evaluating \(f\).

## 10. Error budget

A finite numerical result contains several distinct errors:

- cutting-plane/LP error;
- separation-oracle error;
- numerical integration error;
- finite-difference error in endpoint sensitivities;
- outer stopping/optimization error;
- floating-point error;
- and nonconvex outer optimization error.

The asymptotic \(n^{-2}\) formula is a theorem about the exact optimum \(E_n^*\); it is not a finite-run error bound.
