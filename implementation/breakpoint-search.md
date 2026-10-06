# Adaptive Breakpoint Search

## 1. Goal

This solver is a non-DP outer algorithm for the free-breakpoint problem.

It searches the internal breakpoint coordinates directly and delegates every candidate breakpoint sequence to the existing continuous-height shared-height solver.

The design is intentionally conservative: it does not claim a branch-and-bound certificate from the independently optimized one-segment relaxation.

## 2. Public interface

The solver is exposed as:

```cpp
struct BreakpointSearchOptions {
    int maxDepth = 10;
    int maxEvaluations = 256;
};

Result breakpointSearch(
    const Function& f,
    double a,
    double b,
    int n,
    const BreakpointSearchOptions& options = {}
);
```

For n=1 there are no internal breakpoint variables, so the solver directly evaluates the single fixed segment.

## 3. Node representation

For d=n-1 internal breakpoints a node stores

```text
lower[0..d-1]
upper[0..d-1]
depth
```

The root is the full ambient box

[
[a,b]^d.
]

The box is an outer search representation; it is not itself a feasible breakpoint set.

## 4. Feasible sample construction

The nominal sample is the box midpoint.

If the midpoint is not strictly ordered, the implementation constructs a fallback strictly ordered point inside the node whenever one exists. Infeasible nodes are not passed to the inner solver.

The fallback is only a sampling rule. It does not alter the mathematical search domain.

## 5. Inner solve

For a sampled breakpoint vector

[
X=(a,x_1,ldots,x_{n-1},b),
]

the solver calls the existing fixed-breakpoint continuous-height implementation.

No independent segment optimization is used to construct the returned spline.

Consequently the returned vertices are shared and the resulting spline is continuous.

## 6. Subdivision rule

The longest breakpoint-coordinate interval is bisected.

If coordinate i has

[
r_i-ell_i
]

equal to the maximum width, its children are

[
[ell_i,m_i],
qquad
[m_i,r_i],
qquad
m_i=(ell_i+r_i)/2.
]

All other coordinates are unchanged.

This guarantees that repeated subdivision drives the maximum node width to zero along any exhaustively refined path.

## 7. Search order

Nodes are processed by increasing depth (breadth-first). This is important for the convergence interpretation: a complete level is explored before deeper levels are preferred.

Within a level, nodes with larger geometric width are processed first.

The implementation also avoids evaluating the same breakpoint vector twice when floating-point midpoint arithmetic produces an identical point.

## 8. Stopping

The implementation stops when either:

- maxDepth is reached, or
- maxEvaluations is reached.

There is no claim that the numerical incumbent is globally certified when the budget is exhausted.

Increasing maxDepth and maxEvaluations produces a nested/exhaustive search schedule in the ideal exact-oracle model.

## 9. Numerical feasibility

The fixed-breakpoint inner solver already enforces its shared-height transition constraints numerically.

The outer solver therefore does not perform a separate independent-segment stitching step.

For black-box continuous functions, transition maximization is still numerical and cannot certify a supremum from finitely many samples alone. This limitation is inherited from the existing numerical implementation.

## 10. Performance

The principal cost is the fixed-breakpoint solve. Therefore the implementation avoids:

- constructing a full breakpoint grid;
- running the DP over all predecessor breakpoints;
- evaluating invalid unordered breakpoint vectors;
- repeated evaluation of identical midpoint vectors.

The method is intended mainly for small n and cross-validation. The grid DP remains the preferred method when a dense breakpoint grid is acceptable.

## 11. Correctness boundary

The implementation has two distinct correctness layers.

### Mathematical layer

With exact fixed-breakpoint values and exhaustive subdivision,

[
U_d	o E_n^*.
]

This follows from the continuity-at-an-optimum theorem and exhaustive midpoint refinement in `theory/breakpoint-search.md`.

### Numerical layer

The actual C++ implementation replaces the exact inner oracle by the existing numerical continuous-height solver.

Therefore:

- the returned spline is a numerical feasible candidate;
- the result is not a finite-time global certificate;
- convergence should be checked empirically against the existing DP/reference solver.

## 12. Required regression tests

The implementation should test:

1. n=1 against the known single-segment result;
2. linear f, where the optimum is zero;
3. f(x)=x^2 on [0,1], n=2, whose optimum is 1/24;
4. continuity at every returned breakpoint;
5. agreement with adaptive/fast grid DP on small cases;
6. improvement/non-worsening of the incumbent as maxDepth increases;
7. sine, cosine, quartic and piecewise-continuous functions.

The last two tests specifically check the outer non-convex search rather than only the fixed-breakpoint inner solver.
