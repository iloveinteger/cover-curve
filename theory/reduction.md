# Reduction: no vertical segments

Assume $f\in C^1([a,b])$.

An optimal boundary contains no vertical segment.

Suppose a boundary contains a vertical segment at $x=c$, from height $y_0$ to $y_1>y_0$. Since the boundary lies above $f$,

```math
f(c)\le y_0.
```

Because $f\in C^1$, there exist $M,\delta>0$ such that

```math
|f(x)-f(c)|\le M|x-c|
```

for $|x-c|<\delta$.

Choose $m>M$ and replace the vertical segment by a segment of slope $-m$ or $m$, tilted toward a side where the boundary continues. For sufficiently small displacement, the new segment remains above $f$ and strictly lowers the boundary on a set of positive measure.

Thus the enclosed area decreases, contradicting optimality.

Therefore an optimal boundary contains no vertical segments.

Consequently, it can be represented as a continuous piecewise-linear function with breakpoints

```math
a=x_0<x_1<\cdots<x_n=b.
```
