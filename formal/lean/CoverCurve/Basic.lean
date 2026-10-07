import Mathlib

namespace CoverCurve

theorem continuous_attains_min_on_Icc
    (f : ℝ → ℝ) {a b : ℝ} (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ x ∈ Set.Icc a b, ∀ y ∈ Set.Icc a b, f x ≤ f y := by
  obtain ⟨x, hx, hmin⟩ :=
    isCompact_Icc.exists_isMinOn (Set.nonempty_Icc.mpr hab) hf
  exact ⟨x, hx, hmin⟩

theorem continuous_attains_max_on_Icc
    (f : ℝ → ℝ) {a b : ℝ} (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ x ∈ Set.Icc a b, ∀ y ∈ Set.Icc a b, f y ≤ f x := by
  obtain ⟨x, hx, hmax⟩ :=
    isCompact_Icc.exists_isMaxOn (Set.nonempty_Icc.mpr hab) hf
  exact ⟨x, hx, hmax⟩

theorem continuous_has_constant_majorant
    (f : ℝ → ℝ) {a b : ℝ} (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ M : ℝ, ∀ x ∈ Set.Icc a b, f x ≤ M := by
  obtain ⟨x, hx, hmax⟩ := continuous_attains_max_on_Icc f hab hf
  exact ⟨f x, hmax⟩

theorem continuous_uniformlyContinuousOn_Icc
    (f : ℝ → ℝ) {a b : ℝ}
    (hf : ContinuousOn f (Set.Icc a b)) :
    UniformContinuousOn f (Set.Icc a b) := by
  exact isCompact_Icc.uniformContinuousOn_of_continuous hf

/-- The feasible objective gap is nonnegative for a continuous majorant. -/
theorem integral_sub_nonneg
    (f g : ℝ → ℝ) {a b : ℝ}
    (hfg : ∀ x ∈ Set.Icc a b, f x ≤ g x)
    (hf : IntervalIntegrable f volume a b)
    (hg : IntervalIntegrable g volume a b) :
    0 ≤ ∫ x in a..b, (g x - f x) := by
  have hnonneg : ∀ x ∈ Set.uIcc a b, 0 ≤ g x - f x := by
    intro x hx
    have hx' : x ∈ Set.Icc a b := by
      simpa [Set.uIcc_of_le] using hx
    linarith [hfg x hx']
  have hint : IntervalIntegrable (fun x => g x - f x) volume a b :=
    hg.sub hf
  exact intervalIntegral.integral_nonneg_of_ae (Filter.Eventually.of_forall hnonneg)

end CoverCurve
