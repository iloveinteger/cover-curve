# Problem

Let $f:[a,b]\to\mathbb R$ be continuous, with $a<b$, and let $n\ge1$.

An admissible $n$-segment majorant is a continuous piecewise-affine function $g:[a,b]\to\mathbb R$ with a representation

$$
a=x_0<x_1<\cdots<x_n=b
$$

such that $g$ is affine on each $[x_i,x_{i+1}]$ and

$$
g(x)\ge f(x)
\qquad (x\in[a,b]).
$$

## Vertex representation

Put

$$
y_i=g(x_i).
$$

Then the restriction of $g$ to $[x_i,x_{i+1}]$ is uniquely determined as

$$
L_i(x)
=
\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}.
$$

Hence the majorant condition is

$$
\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}
\ge f(x),
\qquad x\in[x_i,x_{i+1}].
$$

The error is

$$
E(g)
=
\int_a^b (g(x)-f(x))\,dx.
$$

## Fixed breakpoints

For fixed

$$
X=(x_0,\ldots,x_n),
$$

define

$$
E_X(y)
=
\sum_{i=0}^{n-1}
\left[
\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})
-
\int_{x_i}^{x_{i+1}} f(x)\,dx
\right].
$$

Define

$$
\mathcal F_X
=
\left\{
y\in\mathbb R^{n+1}:
\begin{array}{l}
\displaystyle
\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i
+
\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}
\ge f(x),\\[0.75em]
\displaystyle
i\in\{0,\ldots,n-1\},\quad
x\in[x_i,x_{i+1}]
\end{array}
\right\}.
$$

The fixed-breakpoint value is

$$
V(X)
=
\min_{y\in\mathcal F_X} E_X(y).
$$

The global value is

$$
E_n^*
=
\inf
\left\{
E(g):
g\text{ is an admissible }n\text{-segment majorant}
\right\}.
$$

## Theorem 1 — Existence for fixed breakpoints

For every

$$
a=x_0<x_1<\cdots<x_n=b,
$$

the set $\mathcal F_X$ is nonempty, closed, and convex, and $V(X)$ is attained.

### Proof

By continuity of $f$ on $[a,b]$,

$$
M=\max_{x\in[a,b]} f(x)
$$

exists. The vector

$$
y=(M,\ldots,M)
$$

is feasible, so $\mathcal F_X\ne\varnothing$.

Each defining inequality of $\mathcal F_X$ is a closed affine half-space in $y$. Hence $\mathcal F_X$ is closed and convex.

Let

$$
h_i=x_{i+1}-x_i>0.
$$

Then

$$
E_X(y)
=
\sum_{i=0}^n c_i y_i
-
\int_a^b f(x)\,dx,
$$

where

$$
c_0=\frac{h_0}{2},
$$

$$
c_i=\frac{h_{i-1}+h_i}{2}
\qquad (1\le i\le n-1),
$$

and

$$
c_n=\frac{h_{n-1}}{2}.
$$

Thus $c_i>0$ for every $i$.

Taking $x=x_i$ in the feasibility constraint gives

$$
y_i\ge f(x_i).
$$

Therefore every sublevel set

$$
\left\{
y\in\mathcal F_X:E_X(y)\le C
\right\}
$$

is bounded below coordinatewise. Since every $c_i>0$, the same inequality also bounds every coordinate above. Hence every sublevel set is compact.

The feasible set is nonempty and $E_X$ is continuous, so $E_X$ attains its minimum on $\mathcal F_X$. Thus $V(X)$ is attained. ∎

## Theorem 2 — Fixed-breakpoint formulation

For fixed $X$, the optimization defining $V(X)$ is a linear semi-infinite program.

### Proof

The objective $E_X$ is affine in $y$, and every feasibility constraint is affine in $y$. There is one such constraint for every $x$ in each segment interval. ∎
