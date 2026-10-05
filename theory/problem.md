# Problem

Let $f:[a,b]\to\mathbb R$ be continuous.

Find a continuous boundary made from exactly $n$ line segments, each of arbitrary direction, including vertical segments, such that the boundary lies everywhere above the graph of $f$, minimizing

```math
E(g)=\int_a^b (g(x)-f(x))\,dx.
```

After showing that vertical segments can be eliminated from an optimal solution, the boundary can be represented by breakpoints

```math
a=x_0<x_1<\cdots<x_n=b.
```

On each interval $[x_i,x_{i+1}]$, an affine upper majorant of $f$ is chosen optimally.
