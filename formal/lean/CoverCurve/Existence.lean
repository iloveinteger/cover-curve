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




/-- A positive lower bound on segment width gives an upper bound on
the right endpoint height from a uniform cost bound. -/
theorem segment_right_height_bound
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ μ M C δ : ℝ}
    (hxy : x₀ < x₁)
    (hwidth : δ ≤ x₁ - x₀)
    (hδ : 0 < δ)
    (hf : ContinuousOn f (Set.Icc x₀ x₁))
    (hfeas : segmentFeasible f x₀ y₀ x₁ y₁)
    (hμ : ∀ x ∈ Set.Icc x₀ x₁, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc x₀ x₁, f x ≤ M)
    (hcost : segmentCost f x₀ y₀ x₁ y₁ ≤ C)
    (hC : 0 ≤ C) :
    y₁ ≤ (2 * M - μ) + 2 * C / δ := by
  have hbound :=
    segment_width_height_bound f hxy hf hfeas hμ hM hcost
  by_contra hnot
  have hy : (2 * M - μ) + (2 * C) / δ < y₁ := by
    linarith
  have hw := width_lt_of_height_gt
    (w := x₁ - x₀) (y := y₁) (A := 2 * M - μ) (B := 2 * C)
    (δ := δ) (mul_nonneg (by norm_num) hC) hδ hbound hy
  linarith

/-- A bounded width-height product and diverging heights force the widths to zero. -/
theorem tendsto_zero_of_mul_sub_le
    {w y : ℕ → ℝ} {A B : ℝ}
        (hB : 0 ≤ B)
    (hbound : ∀ k, w k * (y k - A) ≤ B)
    (hy : ∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y k)
    (hw_nonneg : ∀ᶠ k in (Filter.atTop : Filter ℕ), 0 ≤ w k) :
    Filter.Tendsto w (Filter.atTop : Filter ℕ) (nhds 0) := by
  refine (Metric.tendsto_atTop (u := w) (a := (0 : ℝ))).2 ?_
  intro ε hε
  have hyε : ∀ᶠ k in (Filter.atTop : Filter ℕ), A + B / ε < y k :=
    hy (A + B / ε)
  rcases (Filter.eventually_atTop.1 hyε) with ⟨N, hN⟩
  rcases (Filter.eventually_atTop.1 hw_nonneg) with ⟨N₀, hN₀⟩
  refine ⟨max N N₀, ?_⟩
  intro k hk
  have hkN : N ≤ k := le_trans (Nat.le_max_left _ _) hk
  have hkN₀ : N₀ ≤ k := le_trans (Nat.le_max_right _ _) hk
  have hkheight := hN k hkN
  have hk_nonneg := hN₀ k hkN₀
  have hw := width_lt_of_height_gt hB hε (hbound k) hkheight
  simpa [Real.dist_eq, sub_zero, abs_of_nonneg hk_nonneg] using hw


/-- A uniformly bounded-cost sequence of feasible segments has vanishing width
    when its right endpoint height diverges. -/
theorem segment_width_tendsto_zero_of_right_height
    (f : ℝ → ℝ) {w y : ℕ → ℝ} {μ M C : ℝ}
    (hf : ∀ k, ContinuousOn f (Set.Icc 0 (w k)))
    (hμ : ∀ x, μ ≤ f x)
    (hM : ∀ x, f x ≤ M)
    (hfeas :
      ∀ k, segmentFeasible f
        0 μ (w k) (y k))
    (hcost :
      ∀ k, segmentCost f 0 μ (w k) (y k) ≤ C)
    (hw : ∀ k, 0 ≤ w k)
    (hy : ∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y k)
    (hC : 0 ≤ C) :
    Filter.Tendsto w (Filter.atTop : Filter ℕ) (nhds 0) := by
  have hbound : ∀ k, w k * (y k - (2 * M - μ)) ≤ 2 * C := by
    intro k
    by_cases hpos : 0 < w k
    · simpa only [sub_zero] using
        (segment_width_height_bound f hpos (hf k) (hfeas k)
          (by
            intro x hx
            exact hμ x)
          (by
            intro x hx
            exact hM x)
          (hcost k))
    · have hw0 : w k = 0 := le_antisymm (le_of_not_gt hpos) (hw k)
      simp [hw0, hC]
  exact tendsto_zero_of_mul_sub_le (A := 2 * M - μ) (B := 2 * C)
    (mul_nonneg (by norm_num) hC) hbound hy
    (Filter.Eventually.of_forall hw)

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

/-- Every nondegenerate feasible segment has nonnegative cost. -/
theorem segmentCost_nonneg_of_feasible
    (f : ℝ → ℝ) {x₀ y₀ x₁ y₁ : ℝ}
    (hxy : x₀ ≤ x₁)
    (hf : ContinuousOn f (Set.Icc x₀ x₁))
    (hfeas : segmentFeasible f x₀ y₀ x₁ y₁) :
    0 ≤ segmentCost f x₀ y₀ x₁ y₁ := by
  by_cases hlt : x₀ < x₁
  · exact segmentCost_nonneg f hlt hf hfeas
  · have hxeq : x₀ = x₁ := le_antisymm hxy (le_of_not_gt hlt)
    simp [segmentCost, hxeq]

/-- Every knot abscissa lies in the global interval. -/
theorem knotX_mem_Icc
    {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n) :
    ∀ i : Fin (n + 1), knotX P i ∈ Set.Icc a b := by
  intro i
  have hleft : ∀ j : Fin (n + 1), knotX P 0 ≤ knotX P j := by
    intro j
    exact Fin.induction le_rfl
      (fun k hk => le_trans hk (knotX_mono P k)) j
  have hright : ∀ j : Fin (n + 1),
      knotX P j ≤ knotX P (Fin.last n) := by
    intro j
    exact Fin.reverseInduction le_rfl
      (fun k hk => le_trans (knotX_mono P k) hk) j
  constructor
  · exact le_trans (le_of_eq P.2.1.symm) (hleft i)
  · exact le_trans (hright i) (le_of_eq P.2.2.1)

/-- A feasible relaxed configuration has nonnegative total objective. -/
theorem relaxedObjective_nonneg
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    (hf : ContinuousOn f (Set.Icc a b))
    (hfeas : RelaxedFeasible f P) :
    0 ≤ relaxedObjective f P := by
  unfold relaxedObjective
  apply Finset.sum_nonneg
  intro i hi
  have hxi := knotX_mono P i
  have hleft : Set.Icc (knotX P i.castSucc) (knotX P i.succ) ⊆ Set.Icc a b := by
    intro x hx
    exact ⟨le_trans (knotX_mem_Icc P i.castSucc).1 hx.1,
      le_trans hx.2 (knotX_mem_Icc P i.succ).2⟩
  have hfi : ContinuousOn f
      (Set.Icc (knotX P i.castSucc) (knotX P i.succ)) :=
    hf.mono hleft
  exact segmentCost_nonneg_of_feasible f hxi hfi (hfeas i)

/-- Every knot of a feasible relaxed configuration lies above a global lower
bound for the covered function. -/
theorem knotY_ge_of_global_lower_bound
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    (hfeas : RelaxedFeasible f P)
    {μ : ℝ}
    (hμ : ∀ x ∈ Set.Icc a b, μ ≤ f x)
    (hn : 1 ≤ n) :
    ∀ i : Fin (n + 1), μ ≤ knotY P i := by
  intro i
  refine Fin.cases ?_ (fun j => ?_) i
  · have hseg := hfeas ⟨0, hn⟩
    exact le_trans
      (hμ (knotX P 0) (knotX_mem_Icc P 0))
      (segmentFeasible_left f hseg)
  · have hseg := hfeas j
    exact le_trans
      (hμ (knotX P j.succ) (knotX_mem_Icc P j.succ))
      (segmentFeasible_right f hseg)

/-- The relaxed feasible set is nonempty. -/
theorem relaxed_feasible_nonempty
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    Set.Nonempty {P : OrderedKnots a b n | RelaxedFeasible f P } := by
  obtain ⟨P, hP⟩ := exists_relaxed_feasible f hn hab hf
  exact ⟨P, hP⟩


/-- Objective values attained by feasible relaxed configurations. -/
def relaxedObjectiveSet
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ} : Set ℝ :=
  {v | ∃ P : OrderedKnots a b n, RelaxedFeasible f P ∧
    v = relaxedObjective f P}

/-- The infimum of the relaxed finite-segment problem. -/
noncomputable def relaxedValue
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ} : ℝ :=
  sInf (relaxedObjectiveSet f (a := a) (b := b) (n := n))

/-- The relaxed objective set is nonempty. -/
theorem relaxedObjectiveSet_nonempty
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    (relaxedObjectiveSet f (a := a) (b := b) (n := n)).Nonempty := by
  obtain ⟨P, hP⟩ := exists_relaxed_feasible f hn hab hf
  exact ⟨relaxedObjective f P, P, hP, rfl⟩

/-- Zero is a lower bound for all relaxed objective values. -/
theorem relaxedObjectiveSet_bddBelow
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hf : ContinuousOn f (Set.Icc a b)) :
    BddBelow (relaxedObjectiveSet f (a := a) (b := b) (n := n)) := by
  refine ⟨0, ?_⟩
  rintro _ ⟨P, hP, rfl⟩
  exact relaxedObjective_nonneg f P hf hP

/-- The relaxed infimum is nonnegative. -/
theorem relaxedValue_nonneg
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hf : ContinuousOn f (Set.Icc a b)) :
    0 ≤ relaxedValue (f := f) (a := a) (b := b) (n := n) := by
  unfold relaxedValue
  exact Real.sInf_nonneg (fun v hv => by
    rcases hv with ⟨P, hP, rfl⟩
    exact relaxedObjective_nonneg f P hf hP)

/-- The relaxed infimum is attained below any chosen feasible objective value. -/
theorem relaxedValue_le_of_feasible
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hf : ContinuousOn f (Set.Icc a b))
    {P : OrderedKnots a b n}
    (hP : RelaxedFeasible f P) :
    relaxedValue (f := f) (a := a) (b := b) (n := n) ≤ relaxedObjective f P := by
  unfold relaxedValue
  exact csInf_le
    (relaxedObjectiveSet_bddBelow f (a := a) (b := b) (n := n) hf)
    ⟨P, hP, rfl⟩

/-- A feasible minimizing sequence exists for the relaxed problem. -/
theorem exists_relaxed_minimizing_sequence
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ P : ℕ → OrderedKnots a b n,
      ∀ k, RelaxedFeasible f (P k) ∧
        relaxedObjective f (P k) ≤
          relaxedValue (f := f) (a := a) (b := b) (n := n) +
            1 / ((k : ℝ) + 1) := by
  let S : Set ℝ := relaxedObjectiveSet f (a := a) (b := b) (n := n)
  have hS_nonempty : S.Nonempty := by
    exact relaxedObjectiveSet_nonempty f hn hab hf
  have hS_bddBelow : BddBelow S := by
    exact relaxedObjectiveSet_bddBelow f hf
  have hex : ∀ k : ℕ, ∃ P : OrderedKnots a b n,
      RelaxedFeasible f P ∧
        relaxedObjective f P <
          relaxedValue (f := f) (a := a) (b := b) (n := n) +
            1 / ((k : ℝ) + 1) := by
    intro k
    have hlt :
        relaxedValue (f := f) (a := a) (b := b) (n := n) <
          relaxedValue (f := f) (a := a) (b := b) (n := n) +
            1 / ((k : ℝ) + 1) := by
      have hpos : 0 < (1 : ℝ) / ((k : ℝ) + 1) := by
        positivity
      linarith
    have hlt' :
        sInf S <
          relaxedValue (f := f) (a := a) (b := b) (n := n) +
            1 / ((k : ℝ) + 1) := by
      simpa [S, relaxedValue] using hlt
    obtain ⟨v, hv, hvlt⟩ := (csInf_lt_iff hS_bddBelow hS_nonempty).1 hlt'
    rcases hv with ⟨P, hP, rfl⟩
    exact ⟨P, hP, hvlt⟩
  let P : ℕ → OrderedKnots a b n := fun k => Classical.choose (hex k)
  refine ⟨P, ?_⟩
  intro k
  rcases Classical.choose_spec (hex k) with ⟨hP, hobj⟩
  exact ⟨hP, hobj.le⟩



/-- The minimizing sequence has a uniform objective bound. -/
theorem relaxed_minimizing_sequence_bounded
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ P : ℕ → OrderedKnots a b n,
      (∀ k, RelaxedFeasible f (P k)) ∧
      (∀ k,
        relaxedObjective f (P k) ≤
          relaxedValue (f := f) (a := a) (b := b) (n := n) + 1) := by
  obtain ⟨P, hP⟩ := exists_relaxed_minimizing_sequence f hn hab hf
  refine ⟨P, fun k => (hP k).1, ?_⟩
  intro k
  have hk := (hP k).2
  have hfrac : (1 : ℝ) / ((k : ℝ) + 1) ≤ 1 := by
    have hk0 : 0 ≤ (k : ℝ) := by positivity
    have hden : 1 ≤ (k : ℝ) + 1 := by linarith
    have hpos : 0 < (k : ℝ) + 1 := by linarith
    exact (div_le_iff₀ hpos).2 (by linarith)
  linarith


/-- Each segment cost is bounded by the total objective of a feasible configuration. -/
theorem segmentCost_le_relaxedObjective
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    (hf : ContinuousOn f (Set.Icc a b))
    (hfeas : RelaxedFeasible f P)
    (i : Fin n) :
    segmentCost f
        (knotX P i.castSucc) (knotY P i.castSucc)
        (knotX P i.succ) (knotY P i.succ) ≤
      relaxedObjective f P := by
  unfold relaxedObjective
  refine Finset.single_le_sum
    (f := fun j : Fin n =>
      segmentCost f
        (knotX P j.castSucc) (knotY P j.castSucc)
        (knotX P j.succ) (knotY P j.succ))
    (s := Finset.univ) ?_ (Finset.mem_univ i)
  intro j hj
  have hxi := knotX_mono P j
  have hleft :
      Set.Icc (knotX P j.castSucc) (knotX P j.succ) ⊆ Set.Icc a b := by
    intro x hx
    exact ⟨le_trans (knotX_mem_Icc P j.castSucc).1 hx.1,
      le_trans hx.2 (knotX_mem_Icc P j.succ).2⟩
  have hfj : ContinuousOn f
      (Set.Icc (knotX P j.castSucc) (knotX P j.succ)) :=
    hf.mono hleft
  exact segmentCost_nonneg_of_feasible f hxi hfj (hfeas j)

/-- A minimizing sequence has a uniform upper bound on every individual
segment cost. -/
theorem relaxed_minimizing_sequence_segment_cost_bounded
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (hn : 1 ≤ n)
    (hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b)) :
    ∃ P : ℕ → OrderedKnots a b n,
      (∀ k, RelaxedFeasible f (P k)) ∧
      (∀ k, ∀ i : Fin n,
        segmentCost f
            (knotX (P k) i.castSucc) (knotY (P k) i.castSucc)
            (knotX (P k) i.succ) (knotY (P k) i.succ) ≤
          relaxedValue (f := f) (a := a) (b := b) (n := n) + 1) := by
  obtain ⟨P, hPfeas, hPbound⟩ :=
    relaxed_minimizing_sequence_bounded f hn hab hf
  refine ⟨P, hPfeas, ?_⟩
  intro k i
  exact le_trans
    (segmentCost_le_relaxedObjective f (P k) hf (hPfeas k) i)
    (hPbound k)


/-- For a sequence of feasible segments inside a fixed compact interval,
a diverging right endpoint height forces the segment widths to vanish. -/
theorem segment_width_tendsto_zero_of_right_height_global
    (f : ℝ → ℝ) {a b : ℝ} {x₀ x₁ y₀ y₁ : ℕ → ℝ}
    {μ M C : ℝ}
    (_hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b))
    (hμ : ∀ x ∈ Set.Icc a b, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M)
    (hinterval : ∀ k, a ≤ x₀ k ∧ x₁ k ≤ b)
    (hordered : ∀ k, x₀ k ≤ x₁ k)
    (hfeas : ∀ k, segmentFeasible f (x₀ k) (y₀ k) (x₁ k) (y₁ k))
    (hcost : ∀ k, segmentCost f (x₀ k) (y₀ k) (x₁ k) (y₁ k) ≤ C)
    (hC : 0 ≤ C)
    (hy : ∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y₁ k) :
    Filter.Tendsto (fun k => x₁ k - x₀ k)
      (Filter.atTop : Filter ℕ) (nhds 0) := by
  have hbound : ∀ k, (x₁ k - x₀ k) * (y₁ k - (2 * M - μ)) ≤ 2 * C := by
    intro k
    by_cases hpos : x₀ k < x₁ k
    · have hleft : Set.Icc (x₀ k) (x₁ k) ⊆ Set.Icc a b := by
        intro x hx
        exact ⟨le_trans (hinterval k).1 hx.1, le_trans hx.2 (hinterval k).2⟩
      exact segment_width_height_bound f hpos
        (hf.mono hleft) (hfeas k)
        (fun x hx => hμ x (hleft hx))
        (fun x hx => hM x (hleft hx))
        (hcost k)
    · have hz : x₁ k - x₀ k = 0 := by
        linarith [hordered k]
      simp [hz, hC]
  exact tendsto_zero_of_mul_sub_le
    (A := 2 * M - μ) (B := 2 * C)
    (mul_nonneg (by norm_num) hC) hbound hy
    (Filter.Eventually.of_forall (fun k => sub_nonneg.mpr (hordered k)))


/-- The symmetric version: a diverging left endpoint height forces the
segment widths to vanish. -/
theorem segment_width_tendsto_zero_of_left_height_global
    (f : ℝ → ℝ) {a b : ℝ} {x₀ x₁ y₀ y₁ : ℕ → ℝ}
    {μ M C : ℝ}
    (_hab : a ≤ b)
    (hf : ContinuousOn f (Set.Icc a b))
    (hμ : ∀ x ∈ Set.Icc a b, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M)
    (hinterval : ∀ k, a ≤ x₀ k ∧ x₁ k ≤ b)
    (hordered : ∀ k, x₀ k ≤ x₁ k)
    (hfeas : ∀ k, segmentFeasible f (x₀ k) (y₀ k) (x₁ k) (y₁ k))
    (hcost : ∀ k, segmentCost f (x₀ k) (y₀ k) (x₁ k) (y₁ k) ≤ C)
    (hC : 0 ≤ C)
    (hy : ∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y₀ k) :
    Filter.Tendsto (fun k => x₁ k - x₀ k)
      (Filter.atTop : Filter ℕ) (nhds 0) := by
  have hbound : ∀ k, (x₁ k - x₀ k) * (y₀ k - (2 * M - μ)) ≤ 2 * C := by
    intro k
    by_cases hpos : x₀ k < x₁ k
    · have hleft : Set.Icc (x₀ k) (x₁ k) ⊆ Set.Icc a b := by
        intro x hx
        exact ⟨le_trans (hinterval k).1 hx.1, le_trans hx.2 (hinterval k).2⟩
      have hMlocal : ∀ x ∈ Set.Icc (x₀ k) (x₁ k), f x ≤ M := by
        intro x hx
        exact hM x (hleft hx)
      have hlow :=
        segmentCost_lower_bound f (x₀ := x₀ k) (y₀ := y₀ k)
          (x₁ := x₁ k) (y₁ := y₁ k) (M := M) hpos
          (hf.mono hleft) hMlocal
      have hy0 : μ ≤ y₀ k := le_trans
        (hμ (x₀ k) ⟨(hinterval k).1,
          le_trans hpos.le (hinterval k).2⟩)
        (segmentFeasible_left f (hfeas k))
      have hy1 : μ ≤ y₁ k := le_trans
        (hμ (x₁ k) ⟨le_trans (hinterval k).1 hpos.le,
          (hinterval k).2⟩)
        (segmentFeasible_right f (hfeas k))
      have hw : 0 ≤ x₁ k - x₀ k := sub_nonneg.mpr (hordered k)
      have hy1' : 0 ≤ y₁ k - μ := sub_nonneg.mpr hy1
      have hprod : 0 ≤ (x₁ k - x₀ k) * (y₁ k - μ) :=
        mul_nonneg hw hy1'
      have htotal :
          (x₁ k - x₀ k) * (y₀ k + y₁ k - 2 * M) ≤ 2 * C := by
        nlinarith [hlow, hcost k]
      have hsplit :
          (x₁ k - x₀ k) * (y₀ k - (2 * M - μ)) =
            (x₁ k - x₀ k) * (y₀ k + y₁ k - 2 * M) -
              (x₁ k - x₀ k) * (y₁ k - μ) := by
        ring
      rw [hsplit]
      linarith
    · have hz : x₁ k - x₀ k = 0 := by
        linarith [hordered k]
      simp [hz, hC]
  exact tendsto_zero_of_mul_sub_le
    (A := 2 * M - μ) (B := 2 * C)
    (mul_nonneg (by norm_num) hC) hbound hy
    (Filter.Eventually.of_forall (fun k => sub_nonneg.mpr (hordered k)))


/-- The knot abscissae admit a convergent subsequence in the fixed compact box. -/
theorem exists_knotX_convergent_subsequence
    (_f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : ℕ → OrderedKnots a b n)
    (_hab : a ≤ b) :
    ∃ x : Fin (n + 1) → ℝ, ∃ φ : ℕ → ℕ,
      StrictMono φ ∧
        Filter.Tendsto
          (fun k => fun i => knotX (P (φ k)) i)
          (Filter.atTop : Filter ℕ) (nhds x) := by
  let K : Set (Fin (n + 1) → ℝ) :=
    Set.pi Set.univ (fun _ => Set.Icc a b)
  have hK : IsCompact K := by
    exact isCompact_univ_pi (fun _ => isCompact_Icc)
  have hPK : ∀ k : ℕ, (fun i => knotX (P k) i) ∈ K := by
    intro k i _
    exact knotX_mem_Icc (P k) i
  obtain ⟨x, hx, φ, hφ, hlim⟩ := hK.tendsto_subseq hPK
  exact ⟨x, φ, hφ, hlim⟩


/-- If every knot is at or above a global majorant level, replacing all
    knot heights by that level preserves feasibility. -/
theorem relaxed_feasible_constant_height_of_ge
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : OrderedKnots a b n)
    {M : ℝ}
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M)
    (_hY : ∀ i : Fin (n + 1), M ≤ knotY P i) :
    RelaxedFeasible f
      (⟨fun i => (knotX P i, M), P.2.1, P.2.2.1, P.2.2.2⟩ :
        OrderedKnots a b n) := by
  let Q : OrderedKnots a b n :=
    ⟨fun i => (knotX P i, M), P.2.1, P.2.2.1, P.2.2.2⟩
  change RelaxedFeasible f Q
  intro i
  have hxi : knotX P i.castSucc ≤ knotX P i.succ :=
    knotX_mono P i
  have hQx0 : knotX Q i.castSucc = knotX P i.castSucc := by
    rfl
  have hQx1 : knotX Q i.succ = knotX P i.succ := by
    rfl
  have hQy0 : knotY Q i.castSucc = M := by
    rfl
  have hQy1 : knotY Q i.succ = M := by
    rfl
  rw [segmentFeasible]
  rw [hQx0, hQx1, hQy0, hQy1]
  by_cases hlt : knotX P i.castSucc < knotX P i.succ
  · simp [hlt]
    intro x hx0 hx1
    exact hM x ⟨le_trans (knotX_mem_Icc P i.castSucc).1 hx0,
      le_trans hx1 (knotX_mem_Icc P i.succ).2⟩
  · have hxeq : knotX P i.castSucc = knotX P i.succ :=
      le_antisymm hxi (le_of_not_gt hlt)
    simp [hxeq]
    exact hM (knotX P i.succ) (knotX_mem_Icc P i.succ)




/-- A sequence which does not tend to +infinity has an infinite bounded
    subsequence. -/
theorem exists_bounded_subsequence_of_not_tendsto_top
    (y : ℕ → ℝ)
    (h : ¬ (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y k)) :
    ∃ R : ℝ, ∃ φ : ℕ → ℕ,
      StrictMono φ ∧ ∀ k, y (φ k) ≤ R := by
  obtain ⟨R, hR⟩ := not_forall.mp h
  have hfreq : ∃ᶠ k in (Filter.atTop : Filter ℕ), y k ≤ R := by
    rw [Filter.frequently_atTop]
    intro N
    by_contra hN
    have hev : ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y k := by
      rw [Filter.eventually_atTop]
      refine ⟨N, ?_⟩
      intro k hk
      have hnot : ¬ y k ≤ R := by
        intro hy
        exact hN ⟨k, hk, hy⟩
      exact lt_of_not_ge hnot
    exact hR hev
  obtain ⟨φ, hφ, hmem⟩ :=
    Filter.extraction_of_frequently_atTop hfreq
  refine ⟨R, φ, hφ, ?_⟩
  intro k
  exact hmem k

/-- Every real sequence either diverges to +infinity or has a bounded
    subsequence. -/
theorem height_subsequence_dichotomy
    (y : ℕ → ℝ) :
    (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y k) ∨
      ∃ R : ℝ, ∃ φ : ℕ → ℕ,
        StrictMono φ ∧ ∀ k, y (φ k) ≤ R := by
  by_cases h : ∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y k
  · exact Or.inl h
  · exact Or.inr (exists_bounded_subsequence_of_not_tendsto_top y h)

/-- A real sequence with a uniform lower and upper bound has a convergent
    subsequence. -/
theorem exists_convergent_subsequence_of_bounded
    (y : ℕ → ℝ) {μ R : ℝ}
    (hμ : ∀ k, μ ≤ y k)
    (hR : ∀ k, y k ≤ R) :
    ∃ y₀ : ℝ, ∃ φ : ℕ → ℕ,
      StrictMono φ ∧
        Filter.Tendsto (fun k => y (φ k))
          (Filter.atTop : Filter ℕ) (nhds y₀) := by
  let K : Set ℝ := Set.Icc μ R
  have hK : IsCompact K := isCompact_Icc
  have hYK : ∀ k : ℕ, y k ∈ K := by
    intro k
    exact ⟨hμ k, hR k⟩
  obtain ⟨y₀, hy₀, φ, hφ, hlim⟩ := hK.tendsto_subseq hYK
  exact ⟨y₀, φ, hφ, hlim⟩

/-- After passing to a subsequence, each member of a finite family of real
    sequences either tends to +infinity or converges to a finite real value. -/
theorem exists_height_classification_subsequence
    {m : ℕ} (Y : ℕ → Fin m → ℝ) {μ : ℝ}
    (hμ : ∀ k i, μ ≤ Y k i) :
    ∃ φ : ℕ → ℕ, StrictMono φ ∧
      ∀ i : Fin m,
        (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < Y (φ k) i) ∨
          ∃ y : ℝ,
            Filter.Tendsto (fun k => Y (φ k) i)
              (Filter.atTop : Filter ℕ) (nhds y) := by
  induction m with
  | zero =>
      refine ⟨fun k => k, strictMono_id, ?_⟩
      intro i
      exact Fin.elim0 i
  | succ m ih =>
      let y0 : ℕ → ℝ := fun k => Y k 0
      by_cases htop :
          ∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ), R < y0 k
      · obtain ⟨φ, hφ, htail⟩ :=
          ih (Y := fun k i => Y k i.succ)
            (by
              intro k i
              exact hμ k i.succ)
        refine ⟨φ, hφ, ?_⟩
        intro i
        refine Fin.cases ?_ (fun j => ?_) i
        · exact Or.inl (fun R => by
            simpa [y0] using hφ.tendsto_atTop.eventually (htop R))
        · exact htail j
      · obtain ⟨R, ψ, hψ, hR⟩ :=
          exists_bounded_subsequence_of_not_tendsto_top y0 htop
        obtain ⟨y, θ, hθ, hlim⟩ :=
          exists_convergent_subsequence_of_bounded
            (fun k => y0 (ψ k))
            (hμ := fun k => hμ (ψ k) 0)
            (hR := fun k => hR k)
        obtain ⟨φtail, hφtail, htail⟩ :=
          ih (Y := fun k i => Y (ψ (θ k)) i.succ)
            (by
              intro k i
              exact hμ (ψ (θ k)) i.succ)
        let φ : ℕ → ℕ := fun k => ψ (θ (φtail k))
        refine ⟨φ, ?_, ?_⟩
        · exact hψ.comp (hθ.comp hφtail)
        · intro i
          refine Fin.cases ?_ (fun j => ?_) i
          · exact Or.inr ⟨y, by
              dsimp [φ, y0]
              exact hlim.comp hφtail.tendsto_atTop⟩
          · exact htail j



/-- A convergent feasible knot remains above the covered function at its
    limiting abscissa. -/
theorem limit_knot_height_ge
    (f : ℝ → ℝ) {a b : ℝ}
    (hf : ContinuousOn f (Set.Icc a b))
    {x y : ℝ} {xseq yseq : ℕ → ℝ}
    (hx : Filter.Tendsto xseq (Filter.atTop : Filter ℕ) (nhds x))
    (hy : Filter.Tendsto yseq (Filter.atTop : Filter ℕ) (nhds y))
    (hxin : ∀ k, xseq k ∈ Set.Icc a b)
    (hfeas : ∀ k, f (xseq k) ≤ yseq k)
    (hxmem : x ∈ Set.Icc a b) :
    f x ≤ y := by
  have hxwithin :
      Filter.Tendsto xseq (Filter.atTop : Filter ℕ)
        (nhdsWithin x (Set.Icc a b)) :=
    tendsto_nhdsWithin_iff.mpr ⟨hx, Filter.Eventually.of_forall hxin⟩
  have hfx : Filter.Tendsto (fun k => f (xseq k))
      (Filter.atTop : Filter ℕ) (nhds (f x)) :=
    (hf x hxmem).tendsto.comp hxwithin
  exact le_of_tendsto_of_tendsto hfx hy (Filter.Eventually.of_forall hfeas)


/-- On a bounded-cost minimizing subsequence, a knot whose height diverges
    forces each adjacent segment width to vanish. -/
theorem classified_segment_widths_tendsto_zero
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : ℕ → OrderedKnots a b n)
    (hab : a < b)
    (hf : ContinuousOn f (Set.Icc a b))
    {μ M C : ℝ}
    (hμ : ∀ x ∈ Set.Icc a b, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M)
    (hfeas : ∀ k, RelaxedFeasible f (P k))
    (hcost : ∀ k, relaxedObjective f (P k) ≤ C)
    (hC : 0 ≤ C)
    (φ : ℕ → ℕ)
    (_hφ : StrictMono φ)
    (_hclass : ∀ i : Fin (n + 1),
      (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ),
        R < knotY (P (φ k)) i) ∨
      ∃ y : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) i)
          (Filter.atTop : Filter ℕ) (nhds y)) :
    ∀ i : Fin n,
      ((∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ),
          R < knotY (P (φ k)) i.castSucc) →
        Filter.Tendsto
          (fun k =>
            knotX (P (φ k)) i.succ -
              knotX (P (φ k)) i.castSucc)
          (Filter.atTop : Filter ℕ) (nhds 0)) ∧
      ((∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ),
          R < knotY (P (φ k)) i.succ) →
        Filter.Tendsto
          (fun k =>
            knotX (P (φ k)) i.succ -
              knotX (P (φ k)) i.castSucc)
          (Filter.atTop : Filter ℕ) (nhds 0)) := by
  intro i
  constructor
  · intro hy
    apply segment_width_tendsto_zero_of_left_height_global f hab.le hf hμ hM
      (fun k => ⟨
        (knotX_mem_Icc (P (φ k)) i.castSucc).1,
        (knotX_mem_Icc (P (φ k)) i.succ).2⟩)
      (fun k => knotX_mono (P (φ k)) i)
      (fun k => hfeas (φ k) i)
      (fun k =>
        le_trans
          (segmentCost_le_relaxedObjective f (P (φ k)) hf
            (hfeas (φ k)) i)
          (hcost (φ k)))
      hC hy
  · intro hy
    apply segment_width_tendsto_zero_of_right_height_global f hab.le hf hμ hM
      (fun k => ⟨
        (knotX_mem_Icc (P (φ k)) i.castSucc).1,
        (knotX_mem_Icc (P (φ k)) i.succ).2⟩)
      (fun k => knotX_mono (P (φ k)) i)
      (fun k => hfeas (φ k) i)
      (fun k =>
        le_trans
          (segmentCost_le_relaxedObjective f (P (φ k)) hf
            (hfeas (φ k)) i)
          (hcost (φ k)))
      hC hy


/-- A classified bounded-cost subsequence has at least two knots whose heights
    converge to finite real limits. -/
theorem exists_two_convergent_knot_heights
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : ℕ → OrderedKnots a b n)
    (hab : a < b)
    (hf : ContinuousOn f (Set.Icc a b))
    {μ M C : ℝ}
    (hμ : ∀ x ∈ Set.Icc a b, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M)
    (hfeas : ∀ k, RelaxedFeasible f (P k))
    (hcost : ∀ k, relaxedObjective f (P k) ≤ C)
    (hC : 0 ≤ C)
    (φ : ℕ → ℕ)
    (hφ : StrictMono φ)
    (hclass : ∀ i : Fin (n + 1),
      (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ),
        R < knotY (P (φ k)) i) ∨
      ∃ y : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) i)
          (Filter.atTop : Filter ℕ) (nhds y)) :
    ∃ i j : Fin (n + 1), i ≠ j ∧
      (∃ yi : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) i)
          (Filter.atTop : Filter ℕ) (nhds yi)) ∧
      (∃ yj : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) j)
          (Filter.atTop : Filter ℕ) (nhds yj)) := by
  by_contra htwo
  have hwidth : ∀ i : Fin n,
      Filter.Tendsto
        (fun k =>
          knotX (P (φ k)) i.succ -
            knotX (P (φ k)) i.castSucc)
        (Filter.atTop : Filter ℕ) (nhds 0) := by
    intro i
    have hnotboth :
        ¬ ((∃ y : ℝ,
            Filter.Tendsto
              (fun k => knotY (P (φ k)) i.castSucc)
              (Filter.atTop : Filter ℕ) (nhds y)) ∧
          (∃ y : ℝ,
            Filter.Tendsto
              (fun k => knotY (P (φ k)) i.succ)
              (Filter.atTop : Filter ℕ) (nhds y))) := by
      intro h
      exact htwo ⟨i.castSucc, i.succ,
        Fin.ne_of_lt Fin.castSucc_lt_succ, h.1, h.2⟩
    rcases hclass i.castSucc with hleft | hleft
    · exact (classified_segment_widths_tendsto_zero
        f P hab hf hμ hM hfeas hcost hC φ hφ hclass i).1 hleft
    · rcases hclass i.succ with hright | hright
      · exact (classified_segment_widths_tendsto_zero
          f P hab hf hμ hM hfeas hcost hC φ hφ hclass i).2 hright
      · exact False.elim (hnotboth ⟨hleft, hright⟩)
  let width : Fin n → ℕ → ℝ := fun i k =>
    knotX (P (φ k)) i.succ - knotX (P (φ k)) i.castSucc
  have hsum : Filter.Tendsto
      (fun k => ∑ i : Fin n, width i k)
      (Filter.atTop : Filter ℕ) (nhds 0) := by
    simpa [width] using
      (tendsto_finsetSum (s := Finset.univ)
        (f := width)
        (a := fun _ : Fin n => 0)
        (x := (Filter.atTop : Filter ℕ)) (by
          intro i hi
          simpa [width] using hwidth i))
  have heq :
      (fun k => ∑ i : Fin n, width i k) =
        (fun _ : ℕ => b - a) := by
    funext k
    let x : Fin (n + 1) → ℝ := knotX (P (φ k))
    let z : ℕ → ℝ := fun i =>
      if hi : i ≤ n then x ⟨i, Nat.lt_succ_of_le hi⟩ else 0
    let d : ℕ → ℝ := fun i =>
      if hi : i < n then z (i + 1) - z i else 0
    have hsumx :
        (∑ i : Fin n, (x i.succ - x i.castSucc)) =
          x (Fin.last n) - x 0 := by
      have hconvert :
          (∑ i : Fin n, (x i.succ - x i.castSucc)) =
            ∑ i : Fin n, d i.val := by
        apply Finset.sum_congr rfl
        intro i hi
        have hs : i.succ = ⟨i.val + 1, by omega⟩ := by
          apply Fin.ext
          simp
        have hc : i.castSucc = ⟨i.val, by omega⟩ := by
          apply Fin.ext
          rfl
        rw [hs, hc]
        simp [d, z, i.isLt]
      rw [hconvert, Fin.sum_univ_eq_sum_range]
      have hrewrite :
          (∑ i ∈ Finset.range n, d i) =
            ∑ i ∈ Finset.range n, (z (i + 1) - z i) := by
        apply Finset.sum_congr rfl
        intro i hi
        have hix : i < n := Finset.mem_range.mp hi
        simp [d, hix]
      rw [hrewrite, Finset.sum_range_sub]
      change z n - z 0 = x (Fin.last n) - x 0
      have hzn : z n = x (Fin.last n) := by
        simp [z]
        congr 1
      have hz0 : z 0 = x 0 := by
        simp [z]
      rw [hzn, hz0]
    have hx0 : x 0 = a := by
      simpa [x, knotX] using (P (φ k)).property.1
    have hxn : x (Fin.last n) = b := by
      simpa [x, knotX] using (P (φ k)).property.2.1
    rw [hx0, hxn] at hsumx
    change (∑ i : Fin n, (x i.succ - x i.castSucc)) = b - a
    exact hsumx
  have hconst : Filter.Tendsto
      (fun _ : ℕ => b - a)
      (Filter.atTop : Filter ℕ) (nhds 0) := by
    rw [← heq]
    exact hsum
  have hzero : b - a = 0 :=
    (tendsto_const_nhds_iff.mp hconst)
  linarith


/-- Some segment has two endpoint heights converging to finite limits
    along the classified bounded-cost subsequence. -/
theorem exists_adjacent_convergent_knot_heights
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : ℕ → OrderedKnots a b n)
    (hab : a < b)
    (hf : ContinuousOn f (Set.Icc a b))
    {μ M C : ℝ}
    (hμ : ∀ x ∈ Set.Icc a b, μ ≤ f x)
    (hM : ∀ x ∈ Set.Icc a b, f x ≤ M)
    (hfeas : ∀ k, RelaxedFeasible f (P k))
    (hcost : ∀ k, relaxedObjective f (P k) ≤ C)
    (hC : 0 ≤ C)
    (φ : ℕ → ℕ)
    (hφ : StrictMono φ)
    (hclass : ∀ i : Fin (n + 1),
      (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ),
        R < knotY (P (φ k)) i) ∨
      ∃ y : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) i)
          (Filter.atTop : Filter ℕ) (nhds y)) :
    ∃ i : Fin n,
      (∃ y₀ : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) i.castSucc)
          (Filter.atTop : Filter ℕ) (nhds y₀)) ∧
      (∃ y₁ : ℝ,
        Filter.Tendsto
          (fun k => knotY (P (φ k)) i.succ)
          (Filter.atTop : Filter ℕ) (nhds y₁)) := by
  by_contra hnone
  have hwidth : ∀ i : Fin n,
      Filter.Tendsto
        (fun k =>
          knotX (P (φ k)) i.succ -
            knotX (P (φ k)) i.castSucc)
        (Filter.atTop : Filter ℕ) (nhds 0) := by
    intro i
    rcases hclass i.castSucc with hleft | hleft
    · exact (classified_segment_widths_tendsto_zero
        f P hab hf hμ hM hfeas hcost hC φ hφ hclass i).1 hleft
    · rcases hclass i.succ with hright | hright
      · exact (classified_segment_widths_tendsto_zero
          f P hab hf hμ hM hfeas hcost hC φ hφ hclass i).2 hright
      · exact hnone ⟨i, hleft, hright⟩
  let width : Fin n → ℕ → ℝ := fun i k =>
    knotX (P (φ k)) i.succ - knotX (P (φ k)) i.castSucc
  have hsum : Filter.Tendsto
      (fun k => ∑ i : Fin n, width i k)
      (Filter.atTop : Filter ℕ) (nhds 0) := by
    simpa [width] using
      (tendsto_finsetSum (s := Finset.univ)
        (f := width)
        (a := fun _ : Fin n => 0)
        (x := (Filter.atTop : Filter ℕ)) (by
          intro i hi
          simpa [width] using hwidth i))
  have heq :
      (fun k => ∑ i : Fin n, width i k) =
        (fun _ : ℕ => b - a) := by
    funext k
    let x : Fin (n + 1) → ℝ := knotX (P (φ k))
    let z : ℕ → ℝ := fun i =>
      if hi : i ≤ n then x ⟨i, Nat.lt_succ_of_le hi⟩ else 0
    let d : ℕ → ℝ := fun i =>
      if hi : i < n then z (i + 1) - z i else 0
    have hsumx :
        (∑ i : Fin n, (x i.succ - x i.castSucc)) =
          x (Fin.last n) - x 0 := by
      have hconvert :
          (∑ i : Fin n, (x i.succ - x i.castSucc)) =
            ∑ i : Fin n, d i.val := by
        apply Finset.sum_congr rfl
        intro i hi
        have hs : i.succ = ⟨i.val + 1, by omega⟩ := by
          apply Fin.ext
          simp
        have hc : i.castSucc = ⟨i.val, by omega⟩ := by
          apply Fin.ext
          rfl
        rw [hs, hc]
        simp [d, z, i.isLt]
      rw [hconvert, Fin.sum_univ_eq_sum_range]
      have hrewrite :
          (∑ i ∈ Finset.range n, d i) =
            ∑ i ∈ Finset.range n, (z (i + 1) - z i) := by
        apply Finset.sum_congr rfl
        intro i hi
        have hix : i < n := Finset.mem_range.mp hi
        simp [d, hix]
      rw [hrewrite, Finset.sum_range_sub]
      change z n - z 0 = x (Fin.last n) - x 0
      have hzn : z n = x (Fin.last n) := by
        simp [z]
        congr 1
      have hz0 : z 0 = x 0 := by
        simp [z]
      rw [hzn, hz0]
    have hx0 : x 0 = a := by
      simpa [x, knotX] using (P (φ k)).property.1
    have hxn : x (Fin.last n) = b := by
      simpa [x, knotX] using (P (φ k)).property.2.1
    rw [hx0, hxn] at hsumx
    change (∑ i : Fin n, (x i.succ - x i.castSucc)) = b - a
    exact hsumx
  have hconst : Filter.Tendsto
      (fun _ : ℕ => b - a)
      (Filter.atTop : Filter ℕ) (nhds 0) := by
    rw [← heq]
    exact hsum
  have hzero : b - a = 0 :=
    (tendsto_const_nhds_iff.mp hconst)
  linarith


/-- A single subsequence can make all knot abscissae converge while
    classifying every knot height as either divergent to +∞ or convergent
    to a finite real limit. -/
theorem exists_joint_knot_subsequence
    (f : ℝ → ℝ) {a b : ℝ} {n : ℕ}
    (P : ℕ → OrderedKnots a b n)
    (hab : a ≤ b)
    {μ : ℝ}
    (hμ : ∀ k i, μ ≤ knotY (P k) i) :
    ∃ x : Fin (n + 1) → ℝ, ∃ φ : ℕ → ℕ,
      StrictMono φ ∧
      Filter.Tendsto
        (fun k => fun i => knotX (P (φ k)) i)
        (Filter.atTop : Filter ℕ) (nhds x) ∧
      ∀ i : Fin (n + 1),
        (∀ R : ℝ, ∀ᶠ k in (Filter.atTop : Filter ℕ),
          R < knotY (P (φ k)) i) ∨
        ∃ y : ℝ,
          Filter.Tendsto
            (fun k => knotY (P (φ k)) i)
            (Filter.atTop : Filter ℕ) (nhds y) := by
  obtain ⟨x, ψ, hψ, hx⟩ :=
    exists_knotX_convergent_subsequence f P hab
  obtain ⟨θ, hθ, hclass⟩ :=
    exists_height_classification_subsequence
      (Y := fun k i => knotY (P (ψ k)) i)
      (μ := μ)
      (by
        intro k i
        exact hμ (ψ k) i)
  refine ⟨x, ψ ∘ θ, hψ.comp hθ, ?_, ?_⟩
  · simpa [Function.comp_def] using hx.comp hθ.tendsto_atTop
  · intro i
    simpa [Function.comp_def] using hclass i

end
