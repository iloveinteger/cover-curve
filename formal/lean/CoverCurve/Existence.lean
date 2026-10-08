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
    (P 0).1 = a ∧
    (P (Fin.last n)).1 = b ∧
    ∀ i : Fin n, (P i.castSucc).1 ≤ (P i.succ).1}

/-- The horizontal coordinate of a knot. -/
def knotX {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) (i : Fin (n + 1)) : ℝ :=
  (P.1 i).1

/-- The height coordinate of a knot. -/
def knotY {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) (i : Fin (n + 1)) : ℝ :=
  (P.1 i).2

/-- Feasibility of one relaxed segment. Degenerate segments are vertical. -/
def segmentFeasible
    (f : ℝ → ℝ) (x₀ y₀ x₁ y₁ : ℝ) : Prop :=
  if x₀ < x₁ then
    ∀ x ∈ Set.Icc x₀ x₁,
      f x ≤ y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)
  else
    x₀ = x₁ ∧ f x₀ ≤ y₀ ∧ f x₀ ≤ y₁

theorem segmentFeasible_left
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (h : segmentFeasible f x₀ y₀ x₁ y₁) :
    f x₀ ≤ y₀ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentFeasible, ite_eq_left hlt] at h
    have hx := h x₀ ⟨le_rfl, le_of_lt hlt⟩
    simpa using hx
  · rw [segmentFeasible, ite_eq_right hlt] at h
    exact h.2.1

theorem segmentFeasible_right
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (h : segmentFeasible f x₀ y₀ x₁ y₁) :
    f x₁ ≤ y₁ := by
  by_cases hlt : x₀ < x₁
  · rw [segmentFeasible, ite_eq_left hlt] at h
    have hx := h x₁ ⟨le_of_lt hlt, le_rfl⟩
    calc
      f x₁ ≤ y₀ + (x₁ - x₀) * (y₁ - y₀) / (x₁ - x₀) := hx
      _ = y₁ := by
        field_simp [ne_of_gt (sub_pos.mpr hlt)]
        ring
  · rw [segmentFeasible, ite_eq_right hlt] at h
    rcases h with ⟨hxeq, hy₀, hy₁⟩
    simpa [hxeq] using hy₁

/-- Feasibility of every segment of a relaxed configuration. -/
def RelaxedFeasible
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) : Prop :=
  ∀ i : Fin n,
    segmentFeasible f
      (knotX P i.castSucc) (knotY P i.castSucc)
      (knotX P i.succ) (knotY P i.succ)

/-- Objective contribution of one relaxed segment. -/
def segmentCost
    (f : ℝ → ℝ) (x₀ y₀ x₁ y₁ : ℝ) : ℝ :=
  if x₀ < x₁ then
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

/-- Consecutive knot abscissas are nondecreasing. -/
theorem knotX_mono
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) :
    ∀ i : Fin n, knotX P i.castSucc ≤ knotX P i.succ := by
  intro i
  exact P.2.2.2 i

/-- A continuous function bounded above by M has integral at most the integral of M. -/
theorem interval_integral_le_const
    (f : ℝ → ℝ) {a b M : ℝ}
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b))
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M) :
    ∫ x in a..b, f x ≤ (b - a) * M := by
  have hfi : IntervalIntegrable f volume a b :=
    hf.intervalIntegrable_of_Icc hab
  have hconst : IntervalIntegrable (fun _ : ℝ => M) volume a b :=
    intervalIntegrable_const
  have hle :=
    intervalIntegral.integral_mono_on hab hfi hconst hM
  rw [intervalIntegral.integral_const] at hle
  exact hle

/-- On a positive-width segment, its cost is bounded below by its
endpoint-height average minus an upper bound for f. -/
theorem segmentCost_lower_bound
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ M : ℝ}
    (hxy : x₀ < x₁)
    (hf : ContinuousOn f (Set.Icc x₀ x₁))
    (hM : ∀ x ∈ Set.Icc x₀ x₁, f x ≤ M) :
    segmentCost f x₀ y₀ x₁ y₁ ≥
      (x₁ - x₀) * ((y₀ + y₁) / 2 - M) := by
  rw [segmentCost, ite_eq_left hxy]
  have hI := interval_integral_le_const f (le_of_lt hxy) hf hM
  linarith


/-- A feasible positive-width segment has nonnegative objective contribution. -/
theorem segmentCost_nonneg
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (hxy : x₀ < x₁)
    (hf : ContinuousOn f (Set.Icc x₀ x₁))
    (hfeas : segmentFeasible f x₀ y₀ x₁ y₁) :
    0 ≤ segmentCost f x₀ y₀ x₁ y₁ := by
  rw [segmentCost, ite_eq_left hxy]
  let L : ℝ → ℝ :=
    fun x => y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀)
  have hmajor : ∀ x ∈ Set.Icc x₀ x₁, f x ≤ L x := by
    rw [segmentFeasible, ite_eq_left hxy] at hfeas
    exact hfeas
  have hfI : IntervalIntegrable f volume x₀ x₁ :=
    hf.intervalIntegrable_of_Icc (le_of_lt hxy)
  have hLI : IntervalIntegrable L volume x₀ x₁ := by
    have hLc : ContinuousOn L (Set.Icc x₀ x₁) := by
      dsimp [L]
      fun_prop
    exact hLc.intervalIntegrable_of_Icc (le_of_lt hxy)
  have hnonneg : 0 ≤ ∫ x in x₀..x₁, (L x - f x) := by
    apply intervalIntegral.integral_nonneg (le_of_lt hxy)
    intro x hx
    exact sub_nonneg.mpr (hmajor x hx)
  have hcalc :
      ∫ x in x₀..x₁, L x
        = (x₁ - x₀) / 2 * (y₀ + y₁) := by
    have hne : x₁ - x₀ ≠ 0 := ne_of_gt (sub_pos.mpr hxy)
    have hxI : IntervalIntegrable (fun x : ℝ => x - x₀) volume x₀ x₁ := by
      have hxc : ContinuousOn (fun x : ℝ => x - x₀) (Set.Icc x₀ x₁) := by
        fun_prop
      exact hxc.intervalIntegrable_of_Icc (le_of_lt hxy)
    have hterm :
        IntervalIntegrable
          (fun x : ℝ => (x - x₀) * (y₁ - y₀) / (x₁ - x₀))
          volume x₀ x₁ :=
      (hxI.mul_const (y₁ - y₀)).div_const (x₁ - x₀)
    have hconst : IntervalIntegrable (fun _ : ℝ => y₀) volume x₀ x₁ :=
      intervalIntegrable_const
    have hadd := intervalIntegral.integral_add hconst hterm
    have hxI_id : IntervalIntegrable (fun x : ℝ => x) volume x₀ x₁ := by
      have hxc : ContinuousOn (fun x : ℝ => x) (Set.Icc x₀ x₁) := by
        fun_prop
      exact hxc.intervalIntegrable_of_Icc (le_of_lt hxy)
    have hxI_x₀ : IntervalIntegrable (fun _ : ℝ => x₀) volume x₀ x₁ :=
      intervalIntegrable_const
    calc
      (∫ x in x₀..x₁, L x)
          = (∫ x in x₀..x₁, y₀) +
            ∫ x in x₀..x₁, (x - x₀) * (y₁ - y₀) / (x₁ - x₀) := by
        change
          (∫ x in x₀..x₁,
              y₀ + (x - x₀) * (y₁ - y₀) / (x₁ - x₀))
            =
            (∫ x in x₀..x₁, y₀) +
              ∫ x in x₀..x₁, (x - x₀) * (y₁ - y₀) / (x₁ - x₀)
        exact hadd
      _ = (x₁ - x₀) * y₀ +
            ∫ x in x₀..x₁, (x - x₀) * (y₁ - y₀) / (x₁ - x₀) := by
        rw [intervalIntegral.integral_const]
        simp [smul_eq_mul]
    rw [intervalIntegral.integral_div]
    rw [intervalIntegral.integral_mul_const]
    rw [intervalIntegral.integral_sub hxI_id hxI_x₀]
    rw [integral_id]
    rw [intervalIntegral.integral_const]
    field_simp [hne]
    ring
  have hdiff :
      (∫ x in x₀..x₁, (L x - f x))
        = (∫ x in x₀..x₁, L x) - ∫ x in x₀..x₁, f x :=
    intervalIntegral.integral_sub hLI hfI
  rw [hdiff, hcalc] at hnonneg
  linarith

/-- A feasible positive-width segment with bounded cost has bounded width-height products. -/
theorem segment_width_height_bound
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ μ M C : ℝ}
    (hxy : x₀ < x₁)
    (hf : ContinuousOn f (Set.Icc x₀ x₁))
    (hfeas : segmentFeasible f x₀ y₀ x₁ y₁)
    (hμ : ∀ x ∈ Set.Icc x₀ x₁, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc x₀ x₁, f x ≤ M)
    (hcost : segmentCost f x₀ y₀ x₁ y₁ ≤ C) :
    (x₁ - x₀) * (y₁ - (2 * M - μ)) ≤ 2 * C := by
  have hy₀ : μ ≤ y₀ := by
    exact le_trans (hμ x₀ ⟨le_rfl, le_of_lt hxy⟩)
      (segmentFeasible_left f hfeas)
  have hy₁ : μ ≤ y₁ := by
    exact le_trans (hμ x₁ ⟨le_of_lt hxy, le_rfl⟩)
      (segmentFeasible_right f hfeas)
  have hlow :=
    segmentCost_lower_bound f (y₀ := y₀) (y₁ := y₁) hxy hf hM
  nlinarith

/-- A bounded width-height product forces a small width when the height is large. -/
theorem width_lt_of_height_gt
    {w y A B δ : ℝ}
    (hw : 0 ≤ w)
    (hB : 0 ≤ B)
    (hδ : 0 < δ)
    (hbound : w * (y - A) ≤ B)
    (hy : A + B / δ < y) :
    w < δ := by
  by_contra hnot
  have hδw : δ ≤ w := le_of_not_gt hnot
  have hpos : 0 < y - A := by
    have hdiv : 0 ≤ B / δ := div_nonneg hB (le_of_lt hδ)
    have hAy : A < y := by
      linarith [hy, hdiv]
    exact sub_pos.mpr hAy
  have hmul : δ * (y - A) ≤ w * (y - A) := by
    gcongr
  have hupper : B < δ * (y - A) := by
    have hdiv : B / δ < y - A := by
      linarith
    calc
      B = δ * (B / δ) := by field_simp [ne_of_gt hδ]
      _ < δ * (y - A) := by gcongr
  linarith

/-- A bounded width-height product and diverging heights force the widths to zero. -/
theorem tendsto_zero_of_mul_sub_le
    {w y : ℕ → ℝ} {A B : ℝ}
    (hw : ∀ k, 0 ≤ w k)
    (hB : 0 ≤ B)
    (hbound : ∀ k, w k * (y k - A) ≤ B)
    (hy : Tendsto y atTop atTop) :
    Tendsto w atTop (𝓝 0) := by
  refine Metric.tendsto_atTop.2 ?_
  intro ε hε
  have hyε : ∀ᶠ k in atTop, A + B / ε < y k :=
    hy.eventually (eventually_gt_atTop (A + B / ε))
  filter_upwards [hyε] with k hk
  exact width_lt_of_height_gt (hw k) hB hε (hbound k) hk


/-- A constant-height configuration is always a relaxed feasible configuration. -/
theorem exists_relaxed_feasible
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ P : OrderedKnots a b n, RelaxedFeasible f P := by
  obtain ⟨M, hM⟩ := continuous_has_constant_majorant f hab hf
  let P : Fin (n + 1) → ℝ × ℝ :=
    fun i => (if i = Fin.last n then b else a, M)
  have h0 : (P 0).1 = a := by
    simp [P, Nat.ne_of_gt hn]
  have hlast : (P (Fin.last n)).1 = b := by
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
    · rw [segmentFeasible, ite_eq_left hablt]
      intro x hx
      simpa [knotY, Q, P] using hM x hx
    · have heq : a = b := le_antisymm hab (le_of_not_gt hablt)
      subst heq
      rw [segmentFeasible, ite_eq_right hablt]
      exact ⟨rfl, hM a ⟨le_rfl, hab⟩, hM a ⟨le_rfl, hab⟩⟩
  · have hxj : knotX Q i.succ = a := by
      simp [knotX, Q, P, hlast_i]
    rw [hxi, hxj, segmentFeasible, ite_eq_right (lt_irrefl a)]
    exact ⟨rfl, hM a ⟨le_rfl, hab⟩, hM a ⟨le_rfl, hab⟩⟩

/-- The relaxed feasible set is nonempty. -/
theorem relaxed_feasible_nonempty
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    Set.Nonempty {P : OrderedKnots a b n | RelaxedFeasible f P } := by
  obtain ⟨P, hP⟩ := exists_relaxed_feasible f hn hab hf
  exact ⟨P, hP⟩

end
