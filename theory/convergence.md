# Breakpoint-grid convergence

Let $\mathcal A_n$ be the set of admissible continuous piecewise-affine majorants with exactly $n$ nondegenerate intervals, and define

$E_n^* = \inf_{g\in\mathcal A_n}E(g).$

Let

$G_N=\{z_0^{(N)},\ldots,z_{m_N}^{(N)}\}$

be finite ordered grids satisfying

$a=z_0^{(N)} < \cdots < z_{m_N}^{(N)}=b$

and

$\delta_N = \max_j \left( z_{j+1}^{(N)}-z_j^{(N)} \right) \longrightarrow0.$

Let $\mathcal A_{n,N}\subseteq\mathcal A_n$ consist of admissible majorants whose breakpoints belong to $G_N$, and define

$E_{n,N}^* = \inf_{g\in\mathcal A_{n,N}}E(g).$

## Lemma 1 — Grid interpolation approximation

Let $g\in\mathcal A_n$ have breakpoints

$a=x_0<x_1<\cdots<x_n=b.$

For every sufficiently large $N$, there exist grid points

$a=z_0^{(N)} < z_1^{(N)} < \cdots < z_n^{(N)}=b$

such that

$\left|z_i^{(N)}-x_i\right| \le\delta_N \qquad (0\le i\le n).$

If $p_N$ is the piecewise-affine interpolant satisfying

$p_N(z_i^{(N)})=g(z_i^{(N)}),$

then

$\lVert p_N-g\rVert_\infty\longrightarrow0.$

### Proof

Let

$d = \min_{0\le i<n}(x_{i+1}-x_i)>0.$

For sufficiently large $N$,

$2\delta_N<d.$

Choose, for each interior breakpoint $x_i$, a grid point $z_i^{(N)}$ satisfying

$\left|z_i^{(N)}-x_i\right| \le\delta_N,$

and set

$z_0^{(N)}=a, \qquad z_n^{(N)}=b.$

Then

$z_{i+1}^{(N)}-z_i^{(N)} \ge (x_{i+1}-x_i)-2\delta_N >0,$

so the selected points are strictly ordered.

Because $g$ is continuous and affine on finitely many closed intervals, it is Lipschitz on $[a,b]$. Let $K$ be a Lipschitz constant.

For each $i$, let $L_i$ be the affine extension to $\mathbb R$ of the restriction of $g$ to $[x_i,x_{i+1}]$. For sufficiently large $N$, the interval $[z_i^{(N)},z_{i+1}^{(N)}]$ differs from $[x_i,x_{i+1}]$ only by endpoint strips of width at most $\delta_N$.

At either selected endpoint,

$\left|g(z_i^{(N)})-L_i(z_i^{(N)})\right| \le2K\delta_N,$

and the analogous estimate holds at $z_{i+1}^{(N)}$.

Since $p_N-L_i$ is affine on $[z_i^{(N)},z_{i+1}^{(N)}]$, its absolute value is bounded by the maximum of its absolute values at the two endpoints. Hence

$\left|p_N(x)-L_i(x)\right| \le2K\delta_N$

throughout $[z_i^{(N)},z_{i+1}^{(N)}]$.

If $x\in[x_i,x_{i+1}]$, then $g(x)=L_i(x)$, so

$\left|p_N(x)-g(x)\right| \le2K\delta_N.$

If $x$ lies in an endpoint strip where $x$ is outside $[x_i,x_{i+1}]$, then its distance from the corresponding original breakpoint is at most $\delta_N$. Therefore

$\left|g(x)-L_i(x)\right| \le K\delta_N,$

and consequently

$\left|p_N(x)-g(x)\right| \le3K\delta_N.$

Thus

$\lVert p_N-g\rVert_\infty \le3K\delta_N \longrightarrow0.$

∎

## Theorem 2 — Grid convergence

If $\delta_N\to0$, then

$E_{n,N}^*\longrightarrow E_n^*.$

### Proof

Because

$\mathcal A_{n,N}\subseteq\mathcal A_n,$

we have

$E_n^*\le E_{n,N}^*.$

Fix $\varepsilon>0$. Choose $g\in\mathcal A_n$ such that

$E(g)<E_n^*+\varepsilon.$

By Lemma 1, choose $p_N$ with

$\lVert p_N-g\rVert_\infty\longrightarrow0.$

Put

$\eta_N=\lVert p_N-g\rVert_\infty, \qquad g_N=p_N+\eta_N.$

Then

$g_N(x)\ge g(x)\ge f(x)$

for every $x$, so $g_N\in\mathcal A_{n,N}$ for all sufficiently large $N$.

Moreover,

$0\le g_N-g\le2\eta_N,$

hence

$0\le E(g_N)-E(g) \le2(b-a)\eta_N \longrightarrow0.$

Therefore, for sufficiently large $N$,

$E_{n,N}^* \le E(g_N) < E_n^*+2\varepsilon.$

Together with $E_n^*\le E_{n,N}^*$,

$E_n^* \le \liminf_{N\to\infty}E_{n,N}^* \le \limsup_{N\to\infty}E_{n,N}^* \le E_n^*+2\varepsilon.$

Since $\varepsilon>0$ is arbitrary,

$E_{n,N}^*\to E_n^*.$

∎

## Corollary 3 — Uniform grids

For the uniform grid

$G_N = \left\{ a+j\frac{b-a}{N}:0\le j\le N \right\},$

the exact coupled optimization restricted to grid breakpoints converges to the original $n$-segment problem. ∎
