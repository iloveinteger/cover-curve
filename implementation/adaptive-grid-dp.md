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

## Structural performance optimizations

The baseline recurrence and candidate set are unchanged, but the implementation removes avoidable computation.

### Segment-cost reuse

For a fixed grid, every pair $(i,j)$ has one segment cost $C(x_i,x_j)$. The implementation computes each pair once before the DP and stores both the cost and the corresponding segment. The DP then reuses these values across every layer.

Thus the expensive one-segment numerical optimization is not repeated for the same pair.

### Parallel segment-cost evaluation

All candidate pairs with a fixed left endpoint are independent. The implementation evaluates these rows concurrently on native C++ builds. The WebAssembly build keeps this stage sequential unless threading is explicitly enabled by the web build configuration.

The function supplied to the native solver should therefore behave as a mathematical, side-effect-free function when parallel execution is used.

### Parallel DP states

For a fixed layer $k$, every destination $j$ reads only $F[k-1][i]$ and the precomputed cost matrix, then writes only $F[k][j]$. Therefore all $j$ states in one layer are independent and can be evaluated concurrently.

Layers themselves remain sequential because layer $k$ depends on layer $k-1$.

### Final support reuse

The one-segment solver already evaluates the support maximum at the final optimized slope. The final cost is now formed directly from that support value instead of calling the objective once more, which would repeat the same adaptive support maximization.

These are implementation-level optimizations only. They do not change the candidate grid, DP recurrence, one-segment objective, or refinement criterion.

