# Support Maximization

This document specifies the numerical search primitive for maxima of continuous black-box functions.

## 1. General problem

Given a continuous callable (h) on a compact interval ([u,v]), compute a numerical approximation to
$$
\max_{x\in[u,v]}h(x).
$$

The exact maximum exists by continuity and compactness. A finite black-box sampling procedure does not, in general, certify that its returned value is the exact maximum.

## 2. Current search strategy

The numerical search may use:

1. an initial sample set;
2. candidate subintervals between neighboring samples;
3. a score based on observed values and variation;
4. refinement of several promising intervals;
5. retention of unrefined intervals in an active set;
6. termination at the requested numerical tolerance.

The search must not assume unimodality.

## 3. Use in Cover Curve

The primitive is used for functions such as
$$
h_\beta(x)=f(x)-\beta x
$$
in the independent one-segment calculation.

The shared-height solver instead needs the transition supremum
$$
T_{u,v}(p)
=
\sup_{u<x\\\le v}
\\left[
p+\frac{v-u}{x-u}(f(x)-p)
\right].
$$

A generic support maximizer can be reused only after the target function has been transformed appropriately and its behavior near (u) has been handled.

## 4. Certification boundary

For arbitrary continuous (f), finite samples cannot establish a global upper bound on an unsampled region. A certified transition evaluator therefore requires additional information, such as:

- a known modulus of continuity;
- a Lipschitz bound;
- an analytic representation allowing interval bounds;
- or another rigorous enclosure method.

Absent such information, the result is numerical evidence, not a certificate of global feasibility.

## 5. Error propagation

Support-search error affects transition feasibility and, through the DP, the final objective. The implementation should therefore expose the support/transition tolerance separately from integration and grid-refinement tolerances.
