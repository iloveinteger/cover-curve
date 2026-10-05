# Curvature-adaptive solver implementation

## 1. Solver role

The curvature-adaptive solver is a separate experimental solver. It does not modify the baseline `adaptive_grid_dp` algorithm.

Its pipeline is

```text
f
↓
numerical estimate of f''
↓
w(x)=sqrt(|f''(x)|)
↓
cumulative density
↓
curvature-adaptive candidate grid
↓
finite-grid DP
↓
grid refinement
```

The finite-grid optimization reuses the baseline one-segment cost routine.

## 2. Grid construction

The theoretical target density is

```math
w(x)=sqrt{|f''(x)|}.
```

The implementation samples this quantity at equally spaced points and integrates it with the trapezoidal rule. Breakpoints are obtained by linearly inverting the cumulative integral at equal fractions of its total mass.

A positive floor and an upper cap prevent zero-density intervals and pathological clustering. These parameters are implementation safeguards rather than theoretical constants.

## 3. Numerical second derivative

Interior samples use the symmetric finite difference

```math
D_2f(x)=
rac{f(x+h)-2f(x)+f(x-h)}{h^2}.
```

Endpoint samples use a second-order one-sided difference when the required neighboring samples exist.

The public function remains a generic black-box callable; the curvature estimate is used only by the grid-generation layer.

## 4. Finite-grid DP

For a generated grid

```math
a=x_0<x_1<cdots<x_N=b,
```

the solver uses

```math
F[k][j]
=
min_{i=k-1,ldots,j-1}
left(
F[k-1][i]+C(x_i,x_j)
ight).
```

Every candidate pair $(i,j)$ has its one-segment cost computed once and reused across all DP layers.

## 5. Refinement

The default refinement sequence is

```text
N -> 2N -> 4N -> ...
```

until either the same relative objective-change criterion as the baseline is satisfied or the maximum grid size is reached:

```math
rac{|E_{2N}-E_N|}
{max(1,|E_{2N}|,|E_N|)}
<arepsilon.
```

The default parameters are tolerance $10^{-6}$, initial grid $32$, maximum grid $1024$, and $257$ curvature samples.

## 6. Numerical status

This is a heuristic grid-selection method followed by finite-grid DP. It does not establish global optimality for the continuous problem beyond the finite-grid formulation.

The baseline remains the reference method for robustness and convergence comparison.

## 7. Experimental comparison

The curvature solver should be evaluated against the baseline using:

- objective value;
- runtime;
- final grid size;
- breakpoint distribution;
- refinement history.

A difference in objective value is not by itself evidence that one continuous optimization result is globally better, because the two solvers use different candidate grids.
