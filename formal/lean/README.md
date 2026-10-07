# Lean formalization

This directory contains machine-checked mathematical results for the cover-curve problem.

## Toolchain

The formalization is a Lean 4 project using Mathlib, pinned to Lean/Mathlib v4.34.1.

Build from this directory with `lake exe cache get` followed by `lake build`.

The project intentionally contains no `sorry`, `admit`, or placeholder axioms for project theorems.

## Verification policy

The formal development follows the same dependency order as the mathematical theory:

1. compactness and basic analytic facts;
2. existence of an optimal majorant;
3. fixed-breakpoint formulation;
4. exact dynamic programming;
5. breakpoint-grid coverage and convergence;
6. explicit error bounds.

A theorem is considered formalized only when Lean checks its proof. Numerical implementation details, floating-point behavior, benchmarks, and complexity bounds remain outside the pure theorem-proving layer unless a separate formal result is useful.

The Markdown files under `theory/` remain the readable mathematical exposition. When formalization exposes a missing hypothesis or an invalid inference, the Lean development takes precedence as the correctness check and the corresponding Markdown proof is revised to match it.
