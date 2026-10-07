import Mathlib
import CoverCurve.Basic

namespace CoverCurve

/-- The integral objective for a continuous majorant on a closed interval. -/
def objective (f g : ℝ → ℝ) (a b : ℝ) : ℝ :=
  ∫ x in a..b, (g x - f x)

/-- A continuous function is admissible as a majorant when it dominates the
target on the whole interval.  The piece-count restriction is handled by
the later finite-dimensional representation. -/
def MajorantOn (f g : ℝ → ℝ) (a b : ℝ) : Prop :=
  ContinuousOn g (Set.Icc a b) ∧
    ∀ x ∈ Set.Icc a b, f x ≤ g x

theorem objective_nonneg
    (f g : ℝ → ℝ) {a b : ℝ}
    (hfg : MajorantOn f g a b)
    (hf : ContinuousOn f (Set.Icc a b))
    (hg : ContinuousOn g (Set.Icc a b)) :
    0 ≤ objective f g a b := by
  have hf' : IntervalIntegrable f volume a b :=
    hf.intervalIntegrable
  have hg' : IntervalIntegrable g volume a b :=
    hg.intervalIntegrable
  unfold objective
  exact integral_sub_nonneg f g hfg.2 hf' hg'

/-- A constant equal to a maximum value is a feasible continuous majorant. -/
theorem exists_constant_majorant
    (f : ℝ → ℝ) {a b : ℝ}
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ M : ℝ, MajorantOn f (fun _ => M) a b := by
  obtain ⟨M, hM⟩ := continuous_has_constant_majorant f hab hf
  refine ⟨M, ?_⟩
  constructor
  · exact continuousOn_const
  · exact hM

/-- The objective is finite for continuous functions on a compact interval. -/
theorem objective_is_real
    (f g : ℝ → ℝ) {a b : ℝ}
    (hf : ContinuousOn f (Set.Icc a b))
    (hg : ContinuousOn g (Set.Icc a b)) :
    objective f g a b = ∫ x in a..b, (g x - f x) := by
  rfl

end CoverCurve
