# Curvature-adaptive solver implementation

## 1. Solver role

The curvature-adaptive solver is a candidate-grid strategy for the continuous piecewise-linear majorant problem. It must ultimately feed the same shared-height optimization as the baseline.

Pipeline:

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
    shared-height coupled optimization

The curvature stage selects candidate breakpoint locations; it does not remove the continuity constraint.

## 2. Grid construction

The theoretical heuristic is $w(x)=\sqrt{|f''(x)|}$. The implementation samples this quantity, integrates it numerically, and places candidate breakpoints at approximately equal cumulative-density increments.

A positive floor and upper cap are implementation safeguards, not theoretical constants.

## 3. Numerical second derivative

For a black-box callable, interior samples may use $D_2f(x)=[f(x+h)-2f(x)+f(x-h)]/h^2$, with suitable one-sided formulas near the endpoints.

## 4. Coupled finite-grid optimization

For a selected sequence $z_{j_0}=a<\cdots<z_{j_n}=b$, introduce shared heights $y_0,\ldots,y_n$. The segments are determined by these heights and must satisfy all majorant constraints simultaneously.

For fixed breakpoints this is a linear semi-infinite program. A finite-constraint implementation may use support samples together with exchange/refinement.

The old scalar recurrence using independent $C_{\mathrm{ind}}$ is not the exact coupled solver.

## 5. Refinement

Grid refinement may still use $N\to2N\to4N\to\cdots$, but objective stabilization is meaningful only when each grid solves the same shared-height problem.

## 6. Numerical status

The curvature density is a heuristic candidate-grid rule. It does not establish global optimality or the exact asymptotic density for the continuous coupled problem.

A rigorous curvature result must first derive the local asymptotic error of the shared-height formulation.

## 7. Separation of concerns

The implementation should keep curvature estimation, candidate breakpoint generation, shared-height feasibility/optimization, numerical constraint refinement, and grid refinement/convergence diagnostics as separate stages.