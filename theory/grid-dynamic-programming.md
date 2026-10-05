# Exact grid dynamic program

Let G_N={z_0<...<z_N}, with z_0=a and z_N=b. Only breakpoint locations are discretized. Vertex heights remain continuous.

For k>=0, j>=k, and p in R, define W_k(j,p) as the minimum accumulated trapezoid area over all grid indices 0=j_0<...<j_k=j and heights y_0,...,y_k with y_k=p, subject to the exact majorant constraint on every selected segment. The value is +infinity when no such continuation exists.

## Bellman recursion

The initial state is W_0(0,p)=0 for p>=f(a), and +infinity otherwise.

For k>=0 and j>k,

W_{k+1}(j,q) = min_{k<=i<j} inf_p [ (z_j-z_i)(p+q)/2 + W_k(i,p) ],

where the inner infimum is restricted to p>=f(z_i) and q>=T_{z_i,z_j}(p).

At the final grid point,

E_{n,N}^* = inf_q W_n(N,q) - integral_a^b f(x) dx.

## Theorem 1 — Exactness on a breakpoint grid

The recursion above returns exactly the optimum of the continuous shared-height problem restricted to breakpoint sequences in G_N.

### Proof

Every feasible grid spline has a unique ordered breakpoint-index sequence and a unique sequence of shared vertex heights. Its first segment ends at some grid point z_i with endpoint heights p,q. The exact one-segment feasibility lemma gives p>=f(z_i) and q>=T_{z_i,z_j}(p). After the first segment, the remaining spline is an admissible continuation represented by W_k(i,p). Conversely, concatenating a feasible first segment with any feasible continuation gives a continuous feasible spline because the shared breakpoint height is the same p. Induction on the number of segments proves the recursion, and minimizing the final endpoint height gives the grid-restricted optimum. 

## Theorem 2 — The grid optimum is attained

For each fixed finite grid G_N, E_{n,N}^* is a minimum, not merely an infimum.

### Proof

There are finitely many ordered breakpoint-index sequences. For each sequence, the fixed-breakpoint problem attains its minimum. Taking the minimum over finitely many sequences gives an attained global grid optimum. 

## Important distinction

This is the exact DP for the mathematical grid problem. It is not the scalar recurrence D_k(j)=min_i {D_{k-1}(i)+C_ind(z_i,z_j)}. That scalar recurrence loses the shared breakpoint height and therefore solves a relaxation rather than the target problem.

The continuous state p is essential. A practical implementation must approximate or otherwise represent these one-dimensional value functions; replacing them by independent segment costs is not an equivalent algorithm.