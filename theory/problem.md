# Problem

Let $f:[a,b]\to\mathbb R$ be continuous, with $a<b$, and let $n\ge1$.

An admissible $n$-segment majorant is a continuous piecewise-affine function $g:[a,b]\to\mathbb R$ with a representation

$a=x_0<x_1<\cdots<x_n=b$

such that $g$ is affine on each $[x_i,x_{i+1}]$ and

$g(x)\ge f(x) \qquad (x\in[a,b]).$

## Vertex representation

Put

$y_i=g(x_i).$

Then the restriction of $g$ to $[x_i,x_{i+1}]$ is uniquely determined as

$L_i(x) = \frac{x_{i+1}-x}{x_{i+1}-x_i}y_i + \frac{x-x_i}{x_{i+1}-x_i}y_{i+1}.$

Hence the majorant condition is

$\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i + \frac{x-x_i}{x_{i+1}-x_i}y_{i+1} \ge f(x), \qquad x\in[x_i,x_{i+1}].$

The error is

$E(g) = \int_a^b (g(x)-f(x))\,dx.$

## Objective

The approximation error is

$E(g)=\int_a^b(g(x)-f(x))\,dx.$

The goal is to determine the minimum error

$E_n^*=\inf E(g)$

over all admissible $n$-segment majorants $g$.