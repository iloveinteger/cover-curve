# Breakpoint-grid convergence

Let (mathcal A_n) be the set of admissible continuous piecewise-linear majorants with exactly (n) nondegenerate segments, and let
[
E_n^*=inf_{ginmathcal A_n}E(g).
]

Let (G_Nsubset[a,b]) be finite grids containing (a,b), and let
[
delta_N=max_j(z_{j+1}-z_j)
]
be the mesh of the ordered grid points. Assume
[
delta_N	o0.
]

Let (mathcal A_{n,N}subsetmathcal A_n) consist of functions whose (n+1) breakpoints all belong to (G_N), and define
[
E_{n,N}^*
=
inf_{ginmathcal A_{n,N}}E(g).
]

## Theorem 1 — Grid convergence

If (delta_N	o0), then
[
oxed{E_{n,N}^*longrightarrow E_n^*.}
]

### Proof

Since
[
mathcal A_{n,N}subseteqmathcal A_n,
]
we have
[
E_n^*le E_{n,N}^*.
]

Fix (arepsilon>0). By the definition of (E_n^*), choose
[
ginmathcal A_n
]
such that
[
E(g)<E_n^*+arepsilon.
]
Let its breakpoints be
[
a=x_0<x_1<cdots<x_n=b.
]

Because the set of breakpoints is finite and strictly ordered, for all sufficiently large (N) there exist grid points
[
a=z_0^{(N)}<z_1^{(N)}<cdots<z_n^{(N)}=b
]
such that
[
max_i|z_i^{(N)}-x_i|ledelta_N.
]

Let (p_N) be the piecewise-linear interpolant of the values
[
g(z_i^{(N)})
]
at these grid breakpoints. Since (g) is continuous and piecewise linear, its uniform modulus of continuity tends to zero, and the perturbation of finitely many breakpoints implies
[
|p_N-g|_inftylongrightarrow0.
]

Set
[
eta_N=|p_N-g|_infty
]
and define
[
g_N=p_N+eta_N.
]
Then
[
g_Nge gge f
]
on ([a,b]). Moreover (g_N) has the same (n) nondegenerate breakpoint intervals for all sufficiently large (N), so
[
g_Ninmathcal A_{n,N}.
]

Finally,
[
|E(g_N)-E(g)|
=
left|int_a^b(g_N-g),dxight|
le
(b-a)|g_N-g|_infty
le
2(b-a)eta_N
longrightarrow0.
]
Hence, for all sufficiently large (N),
[
E_{n,N}^*
le E(g_N)
< E_n^*+2arepsilon.
]
Together with (E_n^*le E_{n,N}^*),
[
E_n^*leliminf_{N	oinfty}E_{n,N}^*
lelimsup_{N	oinfty}E_{n,N}^*
le E_n^*+2arepsilon.
]
Since (arepsilon>0) is arbitrary,
[
E_{n,N}^*	o E_n^*.
]
∎

## Corollary 2

For uniform grids,
[
G_N=left{a+jrac{b-a}{N}:0le jle Night},
]
the exact coupled optimization restricted to the grid converges to the original (n)-segment problem as (N	oinfty). ∎
