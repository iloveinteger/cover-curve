import Mathlib
import CoverCurve.Basic

namespace CoverCurve

/-- Objective for a continuous majorant on a closed interval. -/
def objective (f g : ℝ → ℝ) (a b : ℝ) : ℝ :=
  ∫ x in a..b, (g x - f x)

/-- Continuous majorization on a closed interval. -/
def MajorantOn (f g : ℝ → ℝ) (a b : ℝ) : Prop :=
  ContinuousOn g (Set.Icc a b) ∧
    ∀ x ∈ Set.Icc a b, f x ≤ g x

theorem objective_nonneg
    (f g : ℝ → ℝ) {a b : ℝ}
    (hfg : MajorantOn f g a b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    0 ≤ objective f g a b := by
  have hg : ContinuousOn g (Set.Icc a b) := hfg.1
  have hf' : IntervalIntegrable f volume a b := hf.intervalIntegrable
  have hg' : IntervalIntegrable g volume a b := hg.intervalIntegrable
  unfold objective
  exact CoverCurve.integral_sub_nonneg f g hfg.2 hf' hg'

/-- A constant equal to a maximum value is a feasible continuous majorant. -/
theorem exists_constant_majorant
    (f : ℝ → ℝ) {a b : ℝ}
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ M : ℝ, MajorantOn f (fun _ => M) a b := by
  obtain ⟨M, hM⟩ := continuous_has_constant_majorant f hab hf
  refine ⟨M, continuousOn_const, ?_⟩
  exact hM

/-
The remaining existence proof is deliberately developed below these
foundational lemmas rather than asserted as an axiom.  The next formal layer
will define finite-segment relaxed configurations and prove compactness of
bounded minimizing sequences, including the degenerate-segment limit and the
removal of interior vertical segments.

This file therefore does not yet contain the full existence theorem.
-/

end CoverCurve
