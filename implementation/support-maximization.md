# Support Maximization

For one segment $[u,v]$ and slope $\beta$, define

```math
h_\beta(x)=f(x)-\beta x.
```

The smallest intercept making the line feasible is

```math
\alpha=\max_{x\in[u,v]}h_\beta(x).
```

## Search strategy

The implementation treats $h_\beta$ as a black-box continuous function.

1. Evaluate the interval at an initial set of sample points.
2. Form candidate subintervals from neighboring samples.
3. Score intervals using observed endpoint/midpoint values together with a variation indicator.
4. Refine several high-scoring intervals.
5. Keep the remaining intervals in the active set so that unexplored regions are not permanently lost.
6. Repeat until the numerical stopping criterion is reached.

The algorithm deliberately does **not** assume that $h_\beta$ is unimodal. A continuous input function may have arbitrarily many local maxima.

## Why this is only a heuristic

Continuity guarantees that a maximum exists on a compact interval, but it does not provide a computable bound on how rapidly the function can vary. Consequently, no finite collection of black-box samples can prove that a larger value does not occur between samples for every continuous function.

Thus the returned support value should be understood as a numerical approximation.

A certified variant would require additional assumptions, for example a known modulus of continuity or Lipschitz constant, allowing an upper bound for each unsampled interval.

## Interaction with slope optimization

The support search is called repeatedly while minimizing the one-segment objective over $\beta$. Accuracy of the support value directly affects the slope objective, so support-search tolerance is part of the overall numerical error budget.

