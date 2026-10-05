# Slope Minimization

For $L(x)=\alpha+\beta x$ on $[u,v]$, feasibility gives

```math
\alpha\ge\max_{x\in[u,v]}(f(x)-\beta x).
```

For fixed $\beta$, choosing the smallest feasible intercept is always optimal because increasing $\alpha$ increases the integral error.

Therefore the implementation minimizes the scalar function

```math
\Phi(\beta)
=(v-u)\max_x(f(x)-\beta x)
+\beta\frac{v^2-u^2}{2}
-\int_u^v f(x)\,dx.
```

## Convexity

The support term

```math
\beta\mapsto\max_x(f(x)-\beta x)
```

is a pointwise maximum of affine functions of $\beta$, hence convex. Adding the linear term in $\beta$ and a constant preserves convexity.

This means a one-dimensional bracketing method such as golden-section search is appropriate for the exact objective.

## Numerical implementation

The implementation:

- evaluates the support maximum numerically;
- searches for a finite slope bracket;
- performs golden-section minimization inside the bracket;
- reconstructs the optimal intercept from the support maximum at the selected slope.

The result contains the segment endpoints and numerical cost, together with the line parameters supplied by the solver's internal representation.

Because the support maximum itself is approximated, numerical convexity is not guaranteed point-for-point. The minimizer should therefore be interpreted with the requested numerical tolerance rather than as a symbolic certificate.

