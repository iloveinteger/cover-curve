# Coordinate Breakpoint Search — Implementation

## Interface

coordinateSearch(f, a, b, n, options) performs cyclic breakpoint optimization.

## Fixed-breakpoint oracle

For candidate points [x0,...,xn], the solver calls algorithms::fast_grid_dp::solveFastGridDPOnGrid(f, points, n).

Because points.size() is exactly n+1 and exactly n segments are requested, every supplied breakpoint is forced. The oracle therefore evaluates the continuous shared-height problem at those fixed breakpoints.

## Coordinate update

For coordinate i, the admissible interval is bounded by its neighbors. A small positive margin preserves strict ordering.

Each update:
1. samples the interval uniformly;
2. evaluates the fixed-breakpoint oracle;
3. chooses the best sample;
4. repeatedly refines the neighboring interval;
5. accepts the best candidate only when it improves the incumbent.

Sweeps stop when relative improvement is below tolerance or the sweep budget is exhausted.

## Numerical status

The exact theory assumes a globally solved one-dimensional coordinate subproblem. The finite implementation uses bounded coarse-to-fine sampling, so it is not a global certificate. It is intended as a fast numerical solver and must be cross-validated against adaptiveGridDP and fastGridDP.

## Complexity

For n-1 internal breakpoints, approximately samples times refinements oracle calls are made per sweep, with the number of sweeps bounded by the option. The search dimension is one at every update.

## Invariant

Every accepted candidate has exactly n segments and uses common vertex heights from the fixed-breakpoint oracle. Continuity is therefore inherited from that oracle.
