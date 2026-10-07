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

/-- A finite list of ordered knots, with endpoints fixed at a and b. -/
def OrderedKnots (a b : ℝ) (n : ℕ) : Type :=
  {P : Fin (n + 1) → ℝ × ℝ //
    P 0 |>.1 = a ∧
    P (Fin.last n) |>.1 = b ∧
    ∀ i : Fin n, (P i.castSucc).1 ≤ (P i.succ).1}

/-- The horizontal coordinate of a knot. -/
def knotX {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) (i : Fin (n + 1)) : ℝ :=
  (P.1 i).1

/-- The height coordinate of a knot. -/
def knotY {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) (i : Fin (n + 1)) : ℝ :=
  (P.1 i).2

/-- Feasibility of one relaxed segment.

For a nondegenerate segment this is the affine majorization condition.
For a degenerate segment at c, both endpoint heights must dominate f(c). -/
def segmentFeasible
    (f : ℝ → ℝ) (x₀ y₀ x₁ y₁ : ℝ) : Prop :=
  if h : x₀ < x₁ then
    ∀ x ∈ Set.Icc x₀ x₁,
      f x ≤ y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)
  else
    x₀ = x₁ ∧ f x₀ ≤ y₀ ∧ f x₀ ≤ y₁

/-- A feasible segment has both endpoint heights above f at its endpoints. -/
theorem segmentFeasible_left
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (h : segmentFeasible f x₀ y₀ x₁ y₁) :
    f x₀ ≤ y₀ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentFeasible, dif_pos hlt] at h
    exact h x₀ ⟨le_rfl, le_of_lt hlt⟩
  · rw [segmentFeasible, dif_neg hlt] at h
    exact h.2.1

/-- A feasible segment has both endpoint heights above f at its endpoints. -/
theorem segmentFeasible_right
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (h : segmentFeasible f x₀ y₀ x₁ y₁) :
    f x₁ ≤ y₁ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentFeasible, dif_pos hlt] at h
    have hx := h x₁ ⟨le_of_lt hlt, le_rfl⟩
    simpa using hx
  · rw [segmentFeasible, dif_neg hlt] at h
    have heq : x₀ = x₁ := h.1
    simpa [heq] using h.2.2

/-- Feasibility of every segment of a relaxed configuration. -/
def RelaxedFeasible
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) : Prop :=
  ∀ i : Fin n,
    segmentFeasible f
      (knotX P i.castSucc) (knotY P i.castSucc)
      (knotX P i.succ) (knotY P i.succ)

/-- Every knot of a feasible relaxed configuration lies above f. -/
theorem relaxed_feasible_knot_lower_bound
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (P : OrderedKnots a b n)
    (hP : RelaxedFeasible f P) :
    ∀ i : Fin (n + 1), f (knotX P i) ≤ knotY P i := by
  intro i
  by_cases hi : i.val = 0
  · have hi0 : i = 0 := Fin.ext hi
    subst i
    have hs := hP ⟨0, hn⟩
    simpa [knotX, knotY] using
      segmentFeasible_left f hs
  · by_cases hilast : i.val = n
    · have hilast' : i = Fin.last n := Fin.ext hilast
      subst i
      have hs := hP ⟨n - 1, by omega⟩
      simpa [knotX, knotY] using
        segmentFeasible_right f hs
    · let j : Fin n := ⟨i.val - 1, by omega⟩
      have hjcast : j.succ = i := by
        apply Fin.ext
        dsimp [j]
        omega
      have hs := hP j
      rw [hjcast] at hs
      simpa [knotX, knotY] using
        segmentFeasible_right f hs

/-- Objective contribution of one relaxed segment. -/
def segmentCost
    (f : ℝ → ℝ) (x₀ y₀ x₁ y₁ : ℝ) : ℝ :=
  if h : x₀ < x₁ then
    (x₁ - x₀) / 2 * (y₀ + y₁) - ∫ x in x₀..x₁, f x
  else
    0

/-- Objective of a relaxed finite-segment configuration. -/
def relaxedObjective
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) : ℝ :=
  ∑ i : Fin n,
    segmentCost f
      (knotX P i.castSucc) (knotY P i.castSucc)
      (knotX P i.succ) (knotY P i.succ)

/-- Adjacent knot coordinates are nondecreasing. -/
theorem knotX_mono
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) :
    ∀ i : Fin n,
      knotX P i.castSucc ≤ knotX P i.succ := by
  intro i
  exact P.2.2 i

/-- The total horizontal width of an ordered configuration is exactly b - a. -/
theorem sum_knot_widths
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) :
    ∑ i : Fin n, (knotX P i.succ - knotX P i.castSucc) = b - a := by
  rw [← P.2.2.1, ← P.2.1]
  exact Fin.sum_univ_succ_sub (fun i => knotX P i)

/-- Each relaxed segment has nonnegative objective contribution when feasible. -/
theorem segmentCost_nonneg
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (hseg : segmentFeasible f x₀ y₀ x₁ y₁)
    (hf : ContinuousOn f (Set.uIcc x₀ x₁)) :
    0 ≤ segmentCost f x₀ y₀ x₁ y₁ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentCost, dif_pos hlt]
    have hmajor : ∀ x ∈ Set.Icc x₀ x₁,
        f x ≤ y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀) := by
      simpa [segmentFeasible, hlt] using hseg
    have hcont : ContinuousOn
        (fun x => y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀))
        (Set.Icc x₀ x₁) := by continuity
    have hint : IntervalIntegrable
        (fun x => y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)) volume x₀ x₁ :=
      hcont.intervalIntegrable
    have hf' : IntervalIntegrable f volume x₀ x₁ := by
      exact hf.intervalIntegrable
    have hnonneg : ∀ x ∈ Set.uIcc x₀ x₁,
        0 ≤ (y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)) - f x := by
      intro x hx
      have hx' : x ∈ Set.Icc x₀ x₁ := by
        simpa [Set.uIcc_of_le (le_of_lt hlt)] using hx
      linarith [hmajor x hx']
    have hgap : 0 ≤ ∫ x in x₀..x₁,
        ((y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)) - f x) := by
      exact intervalIntegral.integral_nonneg_of_ae
        (Filter.Eventually.of_forall hnonneg)
    have hline :
        (∫ x in x₀..x₁,
          (y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀))) =
        (x₁ - x₀) / 2 * (y₀ + y₁) := by
      rw [intervalIntegral.integral_add]
      · rw [intervalIntegral.integral_sub]
        · simp
          ring
        · exact intervalIntegrable_const.sub intervalIntegrable_id
        · exact intervalIntegrable_const.sub intervalIntegrable_id
      · exact hint
      · exact hf'
    rw [← hline]
    exact hgap
  · rw [segmentCost, dif_neg hlt]
    exact le_rfl

/-- A feasible relaxed configuration has nonnegative total objective. -/
theorem relaxedObjective_nonneg
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    (hP : RelaxedFeasible f P)
    (hf : ContinuousOn f (Set.Icc a b)) :
    0 ≤ relaxedObjective f P := by
  unfold relaxedObjective
  apply Finset.sum_nonneg
  intro i hi
  apply segmentCost_nonneg f (hP i)
  exact hf.mono (by
    intro x hx
    have hleft : a ≤ knotX P i.castSucc := by
      rw [← P.2.1]
      exact (P.2.2).transitive (by omega)
    have hright : knotX P i.succ ≤ b := by
      rw [← P.2.2.1]
      exact (P.2.2).transitive (by omega)
    exact ⟨le_trans hleft hx.1, le_trans hx.2 hright⟩)


/-
The full existence theorem is intentionally not asserted here yet. The next
steps formalize the bounded minimizing-sequence argument and the elimination
of nontrivial interior degenerate segments. If Lean requires a stronger
hypothesis or a different configuration representation, the mathematical
statement in theory/existence.md will be revised to match it.
-/


/-- CI marker. -/
theorem lean_verification_stage_marker : True := by trivial

end CoverCurve

