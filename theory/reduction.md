# Reduction: no vertical segments

Assume \(f\in C^1([a,b])\).

An optimal boundary contains no vertical segment.

Suppose a boundary contains a vertical segment at \(x=c\), from height \(y_0\) to \(y_1>y_0\). Since the graph of \(f\) must lie below the boundary,

\[
f(c)\le y_0.
\]

Because \(f\in C^1\), it is locally Lipschitz. Hence for some \(M,\delta>0\),

\[
|f(x)-f(c)|\le M|x-c|
\]

whenever \(|x-c|<\delta\).

Choose \(m>M\) and tilt the upper endpoint slightly to the right:

\[
\varepsilon=\frac{y_1-y_0}{m}.
\]

For sufficiently small \(\varepsilon\), the tilted segment remains above \(f\) near \(c\), while the region under the boundary decreases by a positive triangular area

\[
\frac12(y_1-y_0)\varepsilon.
\]

If the right side is unavailable, tilt to the left instead.

Therefore a vertical segment cannot occur in an optimum.

Consequently an optimal boundary can be represented as a continuous piecewise-linear function with ordered breakpoints

\[
a=x_0<x_1<\cdots<x_n=b.
\]
