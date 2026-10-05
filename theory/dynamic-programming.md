# Dynamic programming

Fix a breakpoint grid

\[
G_N=\{x_0,\ldots,x_N\},\qquad x_j=a+\frac{j(b-a)}N.
\]

For every pair \(i<j\), compute the continuous one-segment cost

\[
C_{ij}=C(x_i,x_j).
\]

Define

\[
F[k][j]
\]

as the minimum cost of covering \([a,x_j]\) with exactly \(k\) segments whose breakpoints belong to the grid.

The recurrence is

\[
F[0][0]=0,
\]

with all other \(F[0][j]=+\infty\), and

\[
\boxed{
F[k][j]
=
\min_{i=k-1,\ldots,j-1}
\left(F[k-1][i]+C_{ij}\right).
}
\]

The answer is

\[
E_{n,N}=F[n][N].
\]

The predecessor index is stored for every state, so the complete optimal breakpoint sequence can be recovered by backtracking.

This is an exact solution of the finite-grid candidate problem. There is no greedy choice of breakpoints.
