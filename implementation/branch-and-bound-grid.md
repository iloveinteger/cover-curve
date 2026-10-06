# Branch-and-Bound Grid Solver

branchBoundGrid is a non-DP global breakpoint search built from the finite-sample relaxation in theory/branch-and-bound.md.

## Algorithm

For an internal uniform breakpoint grid:

1. For every candidate segment [z_i,z_j], sample f at a small fixed set including both endpoints.
2. Solve the resulting two-variable finite linear program by eliminating the intercept and enumerating pairwise active slopes.
3. Subtract the numerical integral of f. This is a lower bound for the true one-segment error.
4. Build a relaxed suffix problem from the independent segment lower bounds.
5. Depth-first branch over ordered breakpoint indices.
6. Prune when the accumulated lower bound plus the relaxed suffix bound is no smaller than the current feasible incumbent.
7. At a surviving leaf, solve the fixed-breakpoint continuous-height problem using the existing fast_grid_dp implementation.
8. Refine the breakpoint grid and stop when successive values stabilize.

No convexity, concavity, differentiability, Monge structure, or unique optimum assumption is used by the branch search.

## Numerical distinction

The lower-bound computation is a relaxation and is not obtained from the numerical support maximum used by the fixed-breakpoint solver. This prevents an underestimated numerical supremum from becoming the theoretical basis for pruning.

The implementation still uses numerical integration and the existing continuous-height solver at candidate leaves. It is therefore not a floating-point global certificate for arbitrary black-box continuous functions.

## Parameters

The public API is branchBoundGrid(f,a,b,n). Internal defaults are tolerance 1e-4, initial grid size 8, maximum grid size 24, and 9 finite samples per candidate segment.

These are implementation defaults, not mathematical constants.

## Expected use

This solver is primarily an independent global-search cross-check. For larger n or very fine grids, breakpoint enumeration can grow exponentially. fastGridDP remains the preferred general-purpose solver when speed is more important than independent verification.

## Verification

The test suite compares the new solver against exact small cases, adaptiveGridDP, fastGridDP, trigonometric functions, higher-degree polynomials, mixed-curvature functions, wavy functions, and continuity checks. The benchmark reports it separately.
