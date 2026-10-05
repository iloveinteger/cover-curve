# Finite-grid optimization with continuity

Fix a candidate breakpoint grid
\[
G_N=\{z_0,\ldots,z_N\},
\qquad
z_j=a+j\frac{b-a}{N}.
\]

The original problem cannot be represented by the old scalar transition
\[
F[k][j]=\min_i(F[k-1][i]+C(z_i,z_j)),
\]
because the height at $z_i$ is shared by the segment ending there and the segment starting there.

## Fixed breakpoint sequence

For a selected sequence
\[
z_{j_0}=a<z_{j_1}<\cdots<z_{j_n}=b,
\]
introduce shared heights
\[
y_0,\ldots,y_n.
\]

The exact conditional problem is
\[
\min_y
\sum_{r=0}^{n-1}
\left[
\frac{z_{j_{r+1}}-z_{j_r}}2(y_r+y_{r+1})
-
\int_{z_{j_r}}^{z_{j_{r+1}}}f
\right]
\]
subject to
\[
\frac{z_{j_{r+1}}-x}{z_{j_{r+1}}-z_{j_r}}y_r
+
\frac{x-z_{j_r}}{z_{j_{r+1}}-z_{j_r}}y_{r+1}
\ge f(x)
\]
for every $x$ in the corresponding segment.

This is a linear semi-infinite program in the shared heights.

## Finite constraint discretization

A numerical implementation may replace each continuum of segment constraints by a finite support sample set $S_{ij}\subset[z_i,z_j]$. The resulting fixed-breakpoint problem is an ordinary finite linear program.

An exchange/refinement procedure can add violated support points and re-solve. Such a procedure is a numerical method and must be distinguished from the exact mathematical problem.

## Consequence for the old DP

The old recurrence remains valid only for the **independent relaxation**
\[
\min\sum_r C_{\mathrm{ind}}(z_{j_r},z_{j_{r+1}}).
\]

It is not the recurrence for the continuous majorant problem.

The implementation must not reconstruct a continuous result by modifying independent segment lines after the DP. Continuity has to be enforced during optimization through the shared vertex heights.

The exact breakpoint-search strategy is deliberately kept separate from the fixed-breakpoint LP formulation until the coupled optimization structure has been established. This prevents an incorrect scalar DP decomposition from being presented as an exact algorithm.
