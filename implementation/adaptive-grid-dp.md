# Continuous-Height Dynamic Programming

This implementation uses the mathematical breakpoint-only formulation.

## 1. State representation

Breakpoint locations are discretized by
$$
G_N=\{z_0<\cdots<z_m\}.
$$

Vertex heights are **not** stored on a finite grid. A DP state is evaluated at an arbitrary real height q:
$$
F_k(j,q).
$$

The recursive evaluator implements
$$
F_{k+1}(j,q)=\min_{i<j}\inf_{\substack{p\ge f(z_i)\q\ge T_{z_i,z_j}(p)}}\left[F_k(i,p)+\frac{z_j-z_i}{2}(p+q)\right].
$$

The implementation memoizes values at the real heights actually requested by the optimizer. These are adaptive evaluation points, not a uniform height grid and there is no heightLevels parameter.

## 2. Continuous height bound

The mathematical optimum can be searched inside
$$
[m_f,B_N],\qquad B_N=m_f+\frac{4(b-a)(M_f-m_f)}{\rho_N}.
$$

The implementation estimates m_f and M_f numerically and uses this bound. This bound controls the search domain; it does not discretize that domain.

## 3. Transition evaluation

For a candidate left height p, the implementation evaluates
$$
T_{u,v}(p)=\sup_{u<x\le v}\frac{(v-u)f(x)-(v-x)p}{x-u}
$$
using adaptive numerical sampling/support search.

This is still a numerical approximation. For arbitrary continuous black-box f, finite numerical sampling cannot certify the exact supremum.

## 4. Continuous-height minimization

For each state and predecessor breakpoint, the implementation searches the bounded feasible interval in p.

Because the full free-breakpoint value function is not generally convex, the implementation does not assume that one golden-section search over the whole interval is globally valid.

Instead it:

1. samples the bounded interval coarsely to locate promising regions;
2. locally refines several regions with golden-section search;
3. memoizes recursively evaluated value states.

This is an adaptive global-search heuristic over a continuous variable, not a mathematical height grid.

Consequently the implementation is intended to find high-quality candidates, but this search procedure does not itself constitute a global-optimality proof.

## 5. Outer final-height search

The final height q is optimized by the same bounded global-search mechanism. The terminal DP condition is q >= f(b).

## 6. Curvature-adaptive solver

The curvature-adaptive solver only changes how the breakpoint grid is generated. It delegates the actual optimization to the same continuous-height DP.

Therefore curvature remains a numerical breakpoint-placement heuristic and does not change the mathematical convergence theorem.

## 7. Feasibility status

The returned spline has shared vertex heights by construction.

However, because T is numerically approximated, the result is not a certified global majorant for arbitrary black-box continuous input. Certification requires an independently controlled upper bound on the transition supremum.

## 8. Convergence status

The theory proves convergence of the exact continuous-height breakpoint-grid optimum as
$$
\delta_N\to0.
$$

The current C++ implementation additionally has numerical errors from transition evaluation, continuous-height global search, numerical integration, and numerical estimation of the function range. Those errors are not yet covered by a full numerical convergence theorem.


## 8. Numerical performance optimizations

The implementation keeps the height state continuous, but avoids several sources of redundant work.

- Transition evaluations are memoized by the pair of breakpoint indices and the continuous left height (p).
- Feasible lower-height searches are memoized by the pair of breakpoint indices and the continuous terminal height (q).
- Repeated evaluations of the same objective point during one global one-dimensional search are cached.
- The numerical support search used inside a transition uses a smaller initial sample and refinement budget than the generic support routine.
- Continuous-height global searches use a small coarse sample followed by local golden-section refinement. This is still a heuristic numerical search; it is not a height grid.
- The binary search used to locate the feasible lower endpoint uses fewer iterations because the result is only a numerical starting point for the subsequent continuous search.

These changes reduce repeated function evaluations without changing the mathematical state space. They do not turn the numerical implementation into a certified global optimizer.

The exact mathematical recurrence and its (O(nm^2)) oracle-level predecessor complexity remain the same; the constants above affect only the cost of the numerical realization.

## 9. Fast grid DP solver

A separate `fast_grid_dp` implementation uses the same continuous-height recurrence, transition search, feasibility search, global-search settings, height bound, and stopping rule as `adaptive_grid_dp`.

Its numerical result is therefore intended to be equivalent to the baseline solver. The optimization is in the memoization layer:

- state values use `std::unordered_map`;
- transition values use `std::unordered_map`;
- feasible lower-height values use `std::unordered_map`;
- repeated objective evaluations in each one-dimensional search use a hash table as well.

The keys are still the exact `double` values requested by the numerical optimizer. No height quantization or tolerance-based key merging is introduced.

This changes the cache lookup/insert cost from ordered-map (O(log M)) to average-case (O(1)), without changing the mathematical search domain or numerical tolerances. `unordered_map` has average constant-time lookup, while `map` has logarithmic lookup. This is an implementation-level optimization, not a change to the mathematical algorithm.

The fast solver is exposed as `cover_curve::fastGridDP`. It is kept separate from `adaptiveGridDP) so the two implementations can be compared directly.
