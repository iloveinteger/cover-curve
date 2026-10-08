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
    (hab : a ≤ b)
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
    have hleft : Set.Icc (x₀ k) (x₁ k) ⊆ Set.Icc a b := by
      intro x hx
      exact ⟨le_trans (hinterval k).1 hx.1, le_trans hx.2 (hinterval k).2⟩
    exact segment_width_height_bound f (hordered k)
      (hf.mono hleft) (hfeas k)
      (fun x hx => hμ x (hleft hx))
      (fun x hx => hM x (hleft hx))
      (hcost k)
  exact tendsto_zero_of_mul_sub_le
    (A := 2 * M - μ) (B := 2 * C)
    (mul_nonneg (by norm_num) hC) hbound hy
    (Filter.Eventually.of_forall (fun k => sub_nonneg.mpr (hordered k)))


/-- The symmetric version: a diverging left endpoint height forces the
segment widths to vanish. -/
theorem segment_width_tendsto_zero_of_left_height_global
    (f : ℝ → ℝ) {a b : ℝ} {x₀ x₁ y₀ y₁ : ℕ → ℝ}
    {μ M C : ℝ}
    (hab : a ≤ b)
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
    have hleft : Set.Icc (x₀ k) (x₁ k) ⊆ Set.Icc a b := by
      intro x hx
      exact ⟨le_trans (hinterval k).1 hx.1, le_trans hx.2 (hinterval k).2⟩
    have hbound' := segment_width_height_bound f (hordered k)
      (hf.mono hleft) (hfeas k)
      (fun x hx => hμ x (hleft hx))
      (fun x hx => hM x (hleft hx))
      (hcost k)
    have hy0 : μ ≤ y₀ k := le_trans
      (hμ (x₀ k) ⟨(hinterval k).1, hordered k⟩)
      (segmentFeasible_left f (hfeas k))
    have hy1 : μ ≤ y₁ k := le_trans
      (hμ (x₁ k) ⟨hordered k, (hinterval k).2⟩)
      (segmentFeasible_right f (hfeas k))
    nlinarith [hbound']
  exact tendsto_zero_of_mul_sub_le
    (A := 2 * M - μ) (B := 2 * C)
    (mul_nonneg (by norm_num) hC) hbound hy
    (Filter.Eventually.of_forall (fun k => sub_nonneg.mpr (hordered k)))

end