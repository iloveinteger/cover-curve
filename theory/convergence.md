# Grid refinement and convergence

Let \(E_n^*\) be the optimum when breakpoint positions are continuous, and let \(E_{n,N}\) be the exact optimum when all breakpoints must lie on the grid

\[
G_N=\left\{a+\frac{j(b-a)}N\right\}.
\]

Since every grid-feasible solution is also continuously feasible,

\[
E_n^*\le E_{n,N}.
\]

Now take a continuous optimum

\[
X^*=(x_1^*,\ldots,x_{n-1}^*).
\]

Choose grid points \(X_N'\) converging coordinatewise to \(X^*\). If the one-segment cost \(C(u,v)\) is continuous, then

\[
J(X)=\sum_{i=0}^{n-1}C(x_i,x_{i+1})
\]

is continuous, so

\[
J(X_N')\to J(X^*)=E_n^*.
\]

Because the grid DP minimizes over all grid candidates,

\[
E_n^*
\le E_{n,N}
\le J(X_N').
\]

Therefore

\[
\boxed{E_{n,N}\to E_n^*.}
\]

If the continuous optimum is unique, the grid-optimal breakpoint vectors converge to it. In the non-unique case, every accumulation point of grid optimizers is a continuous global optimum.

The convergence result is the mathematical justification for repeatedly refining the breakpoint grid.
