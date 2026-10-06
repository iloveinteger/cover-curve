# Adaptive Breakpoint Search

## 1. Goal

This solver is a non-DP outer algorithm for the free-breakpoint problem.

It searches the internal breakpoint coordinates directly and delegates every candidate breakpoint sequence to the existing continuous-height shared-height solver.

The design is intentionally conservative: it does not claim a branch-and-bound certificate from the independently optimized one-segment relaxation.

## 2. Public interface

The solver is exposed as:

    struct BreakpointSearchOptions {
        int maxDepth = 8;
        int maxEvaluations = 128;
    };

    Result breakpointSearch(
        const Function& f,
        double a,
        double b,
        int n,
        const BreakpointSearchOptions& options = {}
    );

For n=1 there are no internal breakpoint variables, so the solver directly evaluates the single fixed segment.

## 3. Node representation

For d=n-1 internal breakpoints a node stores

    lower[0..d-1]
    upper[0..d-1]
    depth

The root is the full ambient box [a,b]^d.

The box is an outer search representation; it is not itself a feasible breakpoint set.

## 4. Feasible sample construction

The nominal sample is the box midpoint.

If the midpoint is not strictly ordered, the implementation constructs a fallback strictly ordered point inside the node whenever one exists. Infeasible nodes are not passed to the inner solver.

The fallback is only a sampling rule. It does not alter the mathematical search domain.

## 5. Inner solve

For a sampled breakpoint vector X=(a,x1,...,x(n-1),b), the solver calls the existing fixed-breakpoint continuous-height implementation.

No independent segment optimization is used to construct the returned spline.

Consequently the returned vertices are shared and the resulting spline is continuous.

## 6. Subdivision rule

The longest breakpoint-coordinate interval is bisected.

If coordinate i has r_i-l_i equal to the maximum width, its children are

    [l_i,m_i] and [m_i,r_i],
    m_i=(l_i+r_i)/2.

All other coordinates are unchanged.

This guarantees that repeated subdivision drives the maximum node width to zero along any exhaustively refined path.

## 7. Search order

Nodes are processed in breadth-first order. Therefore all nodes at a shallower depth are processed before their descendants at a deeper depth.

This makes the finite-budget sequence compatible with the exhaustive-subdivision convergence argument: increasing the evaluation budget extends the same breadth-first search sequence.

The implementation does not currently use a lower-bound priority queue or branch-and-bound pruning.

## 8. Stopping

The implementation stops when either maxDepth is reached, or maxEvaluations is reached.

There is no claim that the numerical incumbent is globally certified when the budget is exhausted.

Increasing maxDepth and maxEvaluations allows progressively finer exhaustive subdivision in the ideal exact-oracle model.

## 9. Numerical feasibility

The fixed-breakpoint inner solver already enforces its shared-height transition constraints numerically.

The outer solver therefore does not perform a separate independent-segment stitching step.

For black-box continuous functions, transition maximization is still numerical and cannot certify a supremum from finitely many samples alone. This limitation is inherited from the existing numerical implementation.

## 10. Performance

The principal cost is the fixed-breakpoint solve. Therefore the implementation avoids:

- constructing a full breakpoint grid;
- running the DP over all predecessor breakpoints;
- evaluating invalid unordered breakpoint vectors;
- retaining a lower-bound data structure that has not been proved consistent.

The method is intended mainly for small n and cross-validation. The grid DP remains the preferred method when a dense breakpoint grid is acceptable.

## 11. Correctness boundary

The implementation has two distinct correctness layers.

### Mathematical layer

With exact fixed-breakpoint values and exhaustive subdivision,

    U_d -> E_n^*.

This follows from the continuity-at-an-optimum theorem and exhaustive refinement in the theory/breakpoint-search.md document.

### Numerical layer

The actual C++ implementation replaces the exact inner oracle by the existing numerical continuous-height solver.

Therefore:

- the returned spline is a numerical feasible candidate;
- the result is not a finite-time global certificate;
- convergence should be checked empirically against the existing DP/reference solver.

## 12. Regression tests

The smoke test now covers:

1. f(x)=x^2 on [0,1], n=2;
2. the known value 1/24;
3. continuity at every returned breakpoint;
4. non-worsening of the incumbent when the evaluation budget is increased.

Additional cross-validation should use sine, cosine, quartic and piecewise-continuous functions.

The outer search should be benchmarked separately because each breakpoint evaluation invokes a complete continuous-height inner solve.
