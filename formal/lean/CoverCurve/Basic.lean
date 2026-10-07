import Mathlib

namespace CoverCurve

/-- A continuous function on a nonempty closed interval attains its minimum. -/
theorem continuous_attains_min_on_Icc
    (f : ℝ → ℝ) {a b : ℝ} (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ x ∈ Set.Icc a b, ∀ y ∈ Set.Icc a b, f x ≤ f y := by
  obtain ⟨x, hx, hmin⟩ :=
    isCompact_Icc.exists_isMinOn (Set.nonempty_Icc.mpr hab) hf
  exact ⟨x, hx, hmin⟩

/-- A continuous function on a nonempty closed interval attains its maximum. -/
theorem continuous_attains_max_on_Icc
    (f : ℝ → ℝ) {a b : ℝ} (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ x ∈ Set.Icc a b, ∀ y ∈ Set.Icc a b, f y ≤ f x := by
  obtain ⟨x, hx, hmax⟩ :=
    isCompact_Icc.exists_isMaxOn (Set.nonempty_Icc.mpr hab) hf
  exact ⟨x, hx, hmax⟩

/-- A continuous function on a closed interval has a constant majorant. -/
theorem continuous_has_constant_majorant
    (f : ℝ → ℝ) {a b : ℝ} (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ M : ℝ, ∀ x ∈ Set.Icc a b, f x ≤ M := by
  obtain ⟨x, hx, hmax⟩ := continuous_attains_max_on_Icc f hab hf
  exact ⟨f x, hmax⟩

/-- A continuous function on a closed interval is uniformly continuous there. -/
theorem continuous_uniformlyContinuousOn_Icc
    (f : ℝ → ℝ) {a b : ℝ}
    (hf : ContinuousOn f (Set.Icc a b)) :
    UniformContinuousOn f (Set.Icc a b) := by
  exact isCompact_Icc.uniformContinuousOn_of_continuous hf

end CoverCurve
