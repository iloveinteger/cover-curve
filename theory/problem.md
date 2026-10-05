# Problem

Let $f:[a,b]\to\mathbb R$ be continuous.

Find a continuous piecewise-linear function $g:[a,b]\to\mathbb R$ with exactly $n$ line segments such that $g(x)\ge f(x)$ for all $x\in[a,b]$, minimizing

```math
E(g)=\int_a^b (g(x)-f(x))\,dx.
```

The function $g$ is determined by breakpoints

```math
a=x_0<x_1<\cdots<x_n=b.
```

On each interval $[x_i,x_{i+1}]$, $g$ is affine and satisfies

```math
g(x)\ge f(x)\qquad\text{for all }x\in[a,b].
```

The objective is to minimize $E(g)$ over all such $n$-segment piecewise-linear majorants.
