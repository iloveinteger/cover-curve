# Coordinate Breakpoint Search

## 1. Problem

For a=x0<x1<...<x(n-1)<xn=b, let V(x1,...,x(n-1)) be the minimum continuous shared-height majorant error for these fixed breakpoints.

The free-breakpoint problem is E*n = inf V(X).

This solver does not discretize breakpoint locations globally. It performs cyclic one-coordinate minimization.

## 2. Exact coordinate subproblem

Fix every breakpoint except xi. Its admissible interval is (x(i-1),x(i+1)). Define phi_i(t)=V(x1,...,x(i-1),t,x(i+1),...,x(n-1)).

A coordinate update chooses a global minimizer of phi_i on a compact interior interval [x(i-1)+delta, x(i+1)-delta].

For every trial t, the fixed-breakpoint oracle receives exactly n+1 breakpoints. Since the oracle must construct exactly n segments, every supplied breakpoint is forced. Thus this evaluation contains no breakpoint-choice DP.

## 3. Descent theorem

If an update replaces xi by a global minimizer of its coordinate subproblem, then V(X(k+1)) <= V(X(k)), because the old coordinate is feasible for the same subproblem.

Therefore the objective sequence is monotone non-increasing and bounded below by zero, so it converges.

This does not imply convergence to the global free-breakpoint optimum. Nonconvex coordinate minimization can converge to a coordinatewise minimum. This distinction is standard in block-coordinate optimization. citeturn0search0turn0search29

## 4. Limit-point theorem

Assume:
1. V is continuous on a compact ordered region;
2. every coordinate subproblem is solved globally;
3. all iterates remain in that region.

Then every accumulation point X* of the cyclic sequence is coordinatewise minimizing. For each i and every admissible t,

V(X*) <= V(x1*,...,t,...,x(n-1)*).

Proof: take a convergent subsequence immediately before the same coordinate update. Global optimality gives the inequality at each member of the subsequence. Continuity permits passage to the limit.

If V is differentiable at an interior limit point, coordinatewise minimality gives partial_i V(X*)=0 for every i, hence the limit point is stationary.

## 5. Numerical line search

An arbitrary continuous one-dimensional phi_i cannot in general be globally minimized in finite time. The implementation therefore uses deterministic coarse-to-fine sampling:

1. uniformly sample the admissible interval;
2. keep the best sample and its neighboring interval;
3. refine that interval for several rounds;
4. accept the best tested breakpoint only if it improves the incumbent.

With increasing refinement and exact oracle evaluations, continuity implies convergence of the sampled minimum to the one-dimensional minimum. A finite production budget is therefore a numerical approximation, not a global optimality certificate.

## 6. Complexity advantage

The previous breakpoint search subdivided an (n-1)-dimensional box. Coordinate search reduces every update to one dimension.

A sweep requires approximately (n-1) times S times R fixed-breakpoint oracle evaluations, where S is the coarse sample count and R is the number of refinement rounds, rather than exponentially many ambient boxes.

Every oracle evaluation still solves the continuous shared-height problem, so common vertex heights and continuity are preserved.

## 7. Solver relationship

adaptiveGridDP is the baseline breakpoint-grid DP. fastGridDP is its optimized implementation. breakpointSearch is the exhaustive direct breakpoint-space solver and is mainly a cross-check for small n. coordinateSearch is the new non-DP numerical optimizer using the fast fixed-breakpoint oracle.

The new solver has a rigorous descent and coordinatewise-limit theorem, but it is not claimed to globally solve every nonconvex free-breakpoint instance.
