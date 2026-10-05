# Problem

Let $f:[a,b]\to\mathbb R$ be continuous.

Find a continuous piecewise-linear function $g:[a,b]\to\mathbb R$ with exactly $n$ nondegenerate line segments such that
\[
g(x)\ge f(x)\qquad\forall x\in[a,b],
\]
while minimizing
\[
E(g)=\int_a^b(g(x)-f(x))\,dx.
\]

The breakpoints are
\[
a=x_0<x_1<\cdots<x_n=b.
\]

For each breakpoint introduce the shared vertex height
\[
y_i=g(x_i).
\]

Then the segment on $[x_i,x_{i+1}]$ is not an independent affine function. It is uniquely determined by its two shared endpoint values:
\[
L_i(x)
=
y_i+
\frac{y_{i+1}-y_i}{x_{i+1}-x_i}(x-x_i).
\]

The continuity condition is therefore built into the representation:
\[
L_i(x_i)=y_i=L_{i-1}(x_i).
\]

The feasibility condition is
\[
L_i(x)\ge f(x)
\qquad\forall x\in[x_i,x_{i+1}],
\]
for every segment.

For fixed breakpoints $X=(x_0,\ldots,x_n)$, the conditional optimization problem is
\[
\min_{y_0,\ldots,y_n}
\sum_{i=0}^{n-1}
\left[
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
\int_{x_i}^{x_{i+1}}f(x)\,dx
\right]
\]
subject to
\[
\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}
\ge f(x)
\]
for every $x\in[x_i,x_{i+1}]$.

For fixed breakpoints this is a linear semi-infinite program: the objective is linear in the shared heights and every constraint is linear in those heights.

This distinction is fundamental. Optimizing each interval independently does not solve the original problem, because adjacent segments must share the same value at every breakpoint.

## Degenerate breakpoints

For analysis it is convenient to allow
\[
a=x_0\le x_1\le\cdots\le x_n=b
\]
and interpret zero-length segments as absent. The nondegenerate formulation is recovered by removing zero-length segments and, when necessary, splitting positive-length segments without changing the represented function.

The primary mathematical problem, however, is the exactly-$n$ nondegenerate-segment problem stated above.
