# Adaptive Grid Dynamic Programming

The global breakpoint problem is continuous, so the implementation introduces a temporary uniform breakpoint grid.

```math
G_N=\left\{a+j\frac{b-a}{N}:j=0,\ldots,N\right\}.
```

Only breakpoint positions are discretized. Each candidate segment still uses the continuous numerical one-segment solver on its interval.

## Grid dynamic programming

Let $F[k][j]$ be the minimum cost of covering the first $j$ grid intervals using $k$ segments. The recurrence is

```math
F[k][j]=\min_{i=k-1,\ldots,j-1}
\left(F[k-1][i]+C(x_i,x_j)\right).
```

A predecessor table stores the minimizing index. Backtracking from $(n,N)$ reconstructs the breakpoint sequence.

The implementation evaluates the one-segment costs needed by the DP and then fills the DP table in increasing numbers of segments.

## Adaptive refinement

The outer solver starts from an initial grid size and repeatedly doubles it:

```text
N -> 2N -> 4N -> ...
```

After two consecutive grids, it compares their objective values using

```math
\frac{|E_{2N}-E_N|}
{\max(1,|E_{2N}|,|E_N|)}.
```

When this quantity is below the requested tolerance, the current solution is returned. A maximum grid size provides a hard computational limit.

The grid size is an internal numerical parameter; callers specify the approximation problem and tolerance rather than having to choose a grid manually.

## Complexity

For a grid with $N+1$ points and $n$ segments, the basic DP has $O(nN^2)$ candidate transitions. Each transition requires a one-segment cost unless values are cached or reused.

The present implementation favors a clear and dependable baseline over aggressive caching and parallelization. Those optimizations can be added independently without changing the mathematical DP recurrence.

