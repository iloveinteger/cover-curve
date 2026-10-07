import Mathlib
import CoverCurve.Basic

namespace CoverCurve

open MeasureTheory

noncomputable section

/-- Objective for a continuous majorant on a closed interval. -/
def objective (f g : ℝ → ℝ) (a b : ℝ) : ℝ :=
  ∫ x in a..b, (g x - f x)

/-- Continuous majorization on a closed interval. -/
def MajorantOn (f g : ℝ → ℝ) (a b : ℝ) : Prop :=
  ContinuousOn g (Set.Icc a b) ∧
    ∀ x ∈ Set.Icc a b, f x ≤ g x

theorem objective_nonneg
    (f g : ℝ → ℝ) {a b : ℝ}
    (hab : a ≤ b)
    (hfg : MajorantOn f g a b) :
    0 ≤ objective f g a b := by
  unfold objective
  exact integral_sub_nonneg f g hab hfg.2

/-- A constant equal to a maximum value is a feasible continuous majorant. -/
theorem exists_constant_majorant
    (f : ℝ → ℝ) {a b : ℝ}
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ M : ℝ, MajorantOn f (fun _ => M) a b := by
  obtain ⟨M, hM⟩ := continuous_has_constant_majorant f hab hf
  exact ⟨M, continuousOn_const, hM⟩

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

/-- Feasibility of one relaxed segment. -/
def segmentFeasible
    (f : ℝ → ℝ) (x₀ y₀ x₁ y₁ : ℝ) : Prop :=
  if h : x₀ < x₁ then
    ∀ x ∈ Set.Icc x₀ x₁,
      f x ≤ y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)
  else
    x₀ = x₁ ∧ f x₀ ≤ y₀ ∧ f x₀ ≤ y₁

theorem segmentFeasible_left
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (h : segmentFeasible f x₀ y₀ x₁ y₁) :
    f x₀ ≤ y₀ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentFeasible, dif_pos hlt] at h
    exact h x₀ ⟨le_rfl, le_of_lt hlt⟩
  · rw [segmentFeasible, dif_neg hlt] at h
    exact h.2.1

theorem segmentFeasible_right
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (h : segmentFeasible f x₀ y₀ x₁ y₁) :
    f x₁ ≤ y₁ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentFeasible, dif_pos hlt] at h
    exact h x₁ ⟨le_of_lt hlt, le_rfl⟩
  · rw [segmentFeasible, dif_neg hlt] at h
    have hxy : x₀ = x₁ := h.1
    simpa [hxy] using h.2.2

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
    exact segmentFeasible_left f (hP ⟨0, hn⟩)
  · by_cases hilast : i.val = n
    · have hiLast : i = Fin.last n := Fin.ext hilast
      subst i
      exact segmentFeasible_right f (hP ⟨n - 1, by omega⟩)
    · let j : Fin n := ⟨i.val - 1, by omega⟩
      have hj : j.succ = i := by
        apply Fin.ext
        dsimp [j]
        omega
      rw [← hj]
      exact segmentFeasible_right f (hP j)

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

/-- Every knot abscissa lies in the endpoint interval. -/
theorem knotX_mem_Icc
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    (hab : a ≤ b) :
    ∀ i : Fin (n + 1), knotX P i ∈ Set.Icc a b := by
  intro i
  have hleft : a ≤ knotX P i := by
    rw [← P.2.1]
    exact (P.2.2).transitive (by omega)
  have hright : knotX P i ≤ b := by
    rw [← P.2.2.1]
    exact (P.2.2).transitive (by omega)
  exact ⟨hleft, hright⟩

theorem knotX_mono
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) :
    ∀ i : Fin n, knotX P i.castSucc ≤ knotX P i.succ := by
  intro i
  exact P.2.2 i

/-- The total horizontal width of an ordered configuration is b - a. -/
theorem sum_knot_widths
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) :
    ∑ i : Fin n, (knotX P i.succ - knotX P i.castSucc) = b - a := by
  rw [← P.2.2.1, ← P.2.1]
  exact Fin.sum_univ_succ_sub (fun i => knotX P i)

/-- Every feasible relaxed objective is nonnegative. -/
theorem relaxedObjective_nonneg
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    (hP : RelaxedFeasible f P)
    (hf : ContinuousOn f (Set.Icc a b)) :
    0 ≤ relaxedObjective f P := by
  unfold relaxedObjective
  apply Finset.sum_nonneg
  intro i hi
  by_cases hlt : knotX P i.castSucc < knotX P i.succ
  · rw [segmentCost, dif_pos hlt]
    have hs := hP i
    rw [segmentFeasible, dif_pos hlt] at hs
    have hfi : ContinuousOn f
        (Set.Icc (knotX P i.castSucc) (knotX P i.succ)) :=
      hf.mono (by
        intro x hx
        have hleft : a ≤ knotX P i.castSucc := by
          rw [← P.2.1]
          exact (P.2.2).transitive (by omega)
        have hright : knotX P i.succ ≤ b := by
          rw [← P.2.2.1]
          exact (P.2.2).transitive (by omega)
        exact ⟨le_trans hleft hx.1, le_trans hx.2 hright⟩)
    have hline : ContinuousOn
        (fun x =>
          knotY P i.castSucc +
            (x - knotX P i.castSucc) *
              (knotY P i.succ - knotY P i.castSucc) /
                (knotX P i.succ - knotX P i.castSucc))
        (Set.Icc (knotX P i.castSucc) (knotX P i.succ)) := by
      continuity
    have hgap : 0 ≤ ∫ x in knotX P i.castSucc..knotX P i.succ,
        ((knotY P i.castSucc +
          (x - knotX P i.castSucc) *
            (knotY P i.succ - knotY P i.castSucc) /
              (knotX P i.succ - knotX P i.castSucc)) - f x) := by
      have hnonneg : ∀ x ∈ Set.Icc (knotX P i.castSucc) (knotX P i.succ),
          0 ≤ (knotY P i.castSucc +
            (x - knotX P i.castSucc) *
              (knotY P i.succ - knotY P i.castSucc) /
                (knotX P i.succ - knotX P i.castSucc)) - f x := by
        intro x hx
        linarith [hs x hx]
      exact intervalIntegral.integral_nonneg
        (le_of_lt hlt) hnonneg
    have hline_int :
        ∫ x in knotX P i.castSucc..knotX P i.succ,
          (knotY P i.castSucc +
            (x - knotX P i.castSucc) *
              (knotY P i.succ - knotY P i.castSucc) /
                (knotX P i.succ - knotX P i.castSucc))
        =
        (knotX P i.succ - knotX P i.castSucc) / 2 *
          (knotY P i.castSucc + knotY P i.succ) := by
      rw [intervalIntegral.integral_affine]
      ring
    have hsub :
        (∫ x in knotX P i.castSucc..knotX P i.succ,
          (knotY P i.castSucc +
            (x - knotX P i.castSucc) *
              (knotY P i.succ - knotY P i.castSucc) /
                (knotX P i.succ - knotX P i.castSucc))) -
        ∫ x in knotX P i.castSucc..knotX P i.succ, f x
        =
        ∫ x in knotX P i.castSucc..knotX P i.succ,
          ((knotY P i.castSucc +
            (x - knotX P i.castSucc) *
              (knotY P i.succ - knotY P i.castSucc) /
                (knotX P i.succ - knotX P i.castSucc)) - f x) := by
      rw [← intervalIntegral.integral_sub]
      · exact hline.intervalIntegrable
      · exact hfi.intervalIntegrable
    rw [← hline_int]
    rw [← hsub]
    exact hgap
  · rw [segmentCost, dif_neg hlt]
    exact le_rfl

/-- A constant-knot configuration gives a nonempty relaxed feasible set. -/
theorem exists_relaxed_feasible
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ P : OrderedKnots a b n, RelaxedFeasible f P := by
  obtain ⟨M, hM⟩ := continuous_has_constant_majorant f hab hf
  let P : Fin (n + 1) → ℝ × ℝ :=
    fun i => (if i = Fin.last n then b else a, M)
  have h0 : P 0 |>.1 = a := by
    simp [P, hn.ne']
  have hlast : P (Fin.last n) |>.1 = b := by
    simp [P]
  have hmono : ∀ i : Fin n, (P i.castSucc).1 ≤ (P i.succ).1 := by
    intro i
    by_cases h : i.succ = Fin.last n
    · simp [P, h, hab]
    · simp [P, h]
  let Q : OrderedKnots a b n := ⟨P, h0, hlast, hmono⟩
  refine ⟨Q, ?_⟩
  intro i
  have hxi : knotX Q i.castSucc = a := by
    simp [knotX, Q, P]
  by_cases hlast_i : i.succ = Fin.last n
  · have hxj : knotX Q i.succ = b := by
      simp [knotX, Q, P, hlast_i]
    rw [hxi, hxj]
    by_cases hablt : a < b
    · rw [segmentFeasible, dif_pos hablt]
      intro x hx
      exact hM x hx
    · have habeq : a = b := le_antisymm hab (le_of_not_gt hablt)
      rw [hablt, segmentFeasible, dif_neg]
      exact ⟨rfl, hM a ⟨le_rfl, hab⟩, hM a ⟨le_rfl, hab⟩⟩
  · have hxj : knotX Q i.succ = a := by
      simp [knotX, Q, P, hlast_i]
    rw [hxi, hxj, segmentFeasible, dif_neg]
    exact ⟨rfl, hM a ⟨le_rfl, hab⟩, hM a ⟨le_rfl, hab⟩⟩

/-- The relaxed feasible set is nonempty. -/
theorem relaxed_feasible_nonempty
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    Set.Nonempty {P : OrderedKnots a b n | RelaxedFeasible f P} := by
  obtain ⟨P, hP⟩ := exists_relaxed_feasible f hn hab hf
  exact ⟨P, hP⟩

end
