# Convergence of breakpoint and height discretization

Let A_n be the admissible continuous piecewise-affine majorants with at most n nondegenerate affine pieces, and let E_n^* be the minimum from existence.md.

## 1. Breakpoint-grid convergence

Let G_N={z_0^(N),...,z_{m_N}^(N)} contain a and b. Define the maximum mesh delta_N=max_j(z_{j+1}^(N)-z_j^(N)) and the minimum gap rho_N=min_j(z_{j+1}^(N)-z_j^(N)). For breakpoint convergence we require delta_N -> 0. Let A_{n,N} be the admissible majorants whose breakpoints belong to G_N, and let E_{n,N}^* be their optimum.

Then E_{n,N}^* tends to E_n^*.

### Proof

Because A_{n,N} is a subset of A_n, E_n^* <= E_{n,N}^*.

Fix epsilon>0 and choose an optimal g* in A_n. If g* has fewer than n pieces, split affine pieces without changing the function. Let x_0,...,x_n be breakpoints. For all sufficiently large N, choose strictly ordered grid points z_i^(N) with |z_i^(N)-x_i|<=delta_N. Let p_N be the piecewise-affine interpolant of g* at these grid points.

Since g* is continuous and piecewise affine, it is Lipschitz. The same endpoint-strip argument as in the interpolation lemma gives ||p_N-g*||_infinity -> 0.

Put eta_N=||p_N-g*||_infinity and g_N=p_N+eta_N. Then g_N>=g*>=f, so g_N is admissible on the grid. Also 0<=g_N-g*<=2 eta_N, hence E(g_N)->E(g*). Therefore limsup E_{n,N}^*<=E_n^*. Together with the reverse inequality, convergence follows. 

## 2. A finite algorithm

The exact grid DP still has continuous height states. To obtain a genuinely finite algorithm, discretize heights as well.

Let m=min f, M=max f, and C_0=(b-a)(M-m). For the height bound, define

B_N = m + 4 C_0 / rho_N.

Thus a fully general grid also requires rho_N>0. For uniform grids, rho_N=delta_N.

Let H_N be a finite height grid containing m and B_N with mesh at most eta_N, where eta_N tends to zero. Round every height upward to the next point of H_N.

### Lemma 2 — Uniform height bound

Every optimum of every fixed breakpoint problem whose breakpoints belong to G_N has all vertex heights at most B_N.

### Proof

The constant function M is feasible and has error C_0, so an optimum has error at most C_0. For a fixed breakpoint vector, write h_i=x_{i+1}-x_i and let c_i be the positive objective coefficient of y_i. Every vertex satisfies y_i>=m and c_i>=rho_N/2. Since

sum_i c_i(y_i-m) = E_X(y) + integral_a^b(f-m) <= 2 C_0,

we obtain y_i-m <= 2 C_0/c_i <= 4 C_0/rho_N. ∎

### Lemma 3 — Height rounding error

Let E_{n,N,eta}^* be the optimum when both breakpoints and vertex heights are restricted to G_N and H_N. Then

E_{n,N}^* <= E_{n,N,eta}^* <= E_{n,N}^* + (b-a) eta_N.

### Proof

Take an exact grid optimum. Round every vertex height upward by at most eta_N. Increasing either endpoint of a segment can only increase its affine function, so feasibility is preserved. The objective increase is at most eta_N times the sum of all trapezoid coefficients, and those coefficients sum to b-a. The opposite inequality is immediate because height discretization restricts the feasible set. 

## Theorem 4 — Fully discrete convergence

If delta_N tends to zero, eta_N tends to zero, and each height grid uses the bound B_N above, then

E_{n,N,eta}^* -> E_n^*.

### Proof

By Lemma 3, the difference between E_{n,N,eta}^* and E_{n,N}^* is at most (b-a)eta_N. By Theorem 1, E_{n,N}^* tends to E_n^*. Therefore the fully discrete values also tend to E_n^*. ∎

## 3. What this proves about an implementation

A finite implementation is mathematically convergent only if its computed problem is an increasingly accurate realization of the fully discrete problem above, with breakpoint mesh tending to zero, height mesh tending to zero, and numerical errors in integrals and feasibility tests tending to zero.

The current scalar one-segment-cost DP does not satisfy this condition because it removes the shared-height state before optimization. Its numerical stabilization or post-processing cannot turn it into the exact coupled algorithm.

Likewise, stopping when two successive objective values differ by a relative tolerance is a practical stopping rule, not an a priori error certificate. A convergence theorem must first establish convergence of the underlying optimization problems.