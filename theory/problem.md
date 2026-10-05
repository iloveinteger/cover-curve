# Problem

Let $f:[a,b]\to\mathbb R$ be sufficiently regular.

We want a continuous boundary made from exactly $n$ line segments that lies everywhere above the graph of $f$, while minimizing the enclosed vertical area:

```math
E(g)=\int_a^b (g(x)-f(x))\,dx
```

The important point is that the line segments are not restricted to a predefined set of slopes or heights.

After eliminating vertical segments, the boundary is represented by ordered breakpoints

```math
a=x_0<x_1<\cdots<x_n=b
```

On each interval $[x_i,x_{i+1}]$, one affine function is chosen optimally subject to being an upper majorant of $f$.

This separates the problem into:

1. a continuous one-segment optimization problem.
2. a continuous optimization over breakpoint locations.
