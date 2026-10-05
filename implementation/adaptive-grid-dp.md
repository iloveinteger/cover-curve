# Adaptive Grid Optimization

The original solver is defined by a continuous piecewise-linear majorant problem with a crucial continuity constraint at every breakpoint.

A breakpoint grid
\[
G_N=\{z_0,\ldots,z_N\}
\]
discretizes only the **locations** of candidate breakpoints. It does not make adjacent segments independent.

## Current mathematical target

For a selected breakpoint sequence
\[
z_{j_0}=a<\cdots<z_{j_n}=b,
\]
the implementation must optimize shared vertex heights
\[
y_0,\ldots,y_n.
\]

The segment is
\[
L_i(x)
=
y_i+
\frac{y_{i+1}-y_i}{z_{j_{i+1}}-z_{j_i}}
(x-z_{j_i}),
\]
and feasibility requires
\[
L_i(x)\ge f(x)
\]
throughout the segment.

For fixed breakpoints this is a linear semi-infinite program.

## Important implementation invariant

The following pattern is **not valid** for the target problem:

1. independently call oneSegmentCost for every candidate pair;
2. run scalar-cost DP;
3. alter the resulting lines afterward to make them continuous.

Continuity must be enforced while optimizing the candidate solution.

## Numerical route

The intended implementation should separate:

1. candidate breakpoint selection;
2. shared-height optimization for a selected breakpoint sequence;
3. support/constraint refinement;
4. final result construction.

The existing oneSegmentCost routine may remain as an independent-relaxation/reference routine, but it must not supply the exact global transition cost unless a new theorem proves an equivalent coupled decomposition.

## Refinement

Grid refinement remains conceptually
\[
N\to2N\to4N\to\cdots,
\]
but the convergence quantity must be computed from the objective of the **continuous shared-height problem**.

Numerical convergence of an independent relaxation is not sufficient.

## Performance

The old $O(nN^2)$ scalar DP complexity describes the independent relaxation only. The coupled solver has a different computational structure and must be analyzed after its optimization method is implemented.

Until then, complexity claims inherited from the old scalar DP must not be presented as complexity bounds for the target problem.
