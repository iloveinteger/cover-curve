# Fixed-breakpoint formulation

Fix breakpoints $X=(x_0,\ldots,x_n)$ with

$a=x_0<x_1<\cdots<x_n=b.$

Write $y_i=g(x_i)$. For fixed $X$, the piecewise-affine majorant is determined by the vertex heights $y_0,\ldots,y_n$.

Define the feasible set $\mathcal F_X$ to consist of all $y\in\mathbb R^{n+1}$ such that, for every $i\in\{0,\ldots,n-1\}$ and every $x\in[x_i,x_{i+1}]$,

$\frac{x_{i+1}-x}{x_{i+1}-x_i}y_i+\frac{x-x_i}{x_{i+1}-x_i}y_{i+1}\ge f(x).$

The fixed-breakpoint objective is

$E_X(y)=\sum_{i=0}^{n-1}[\frac{x_{i+1}-x_i}{2}(y_i+y_{i+1})-\int_{x_i}^{x_{i+1}}f(x)\,dx].$

The fixed-breakpoint value is

$V(X)=\min_{y\in\mathcal F_X}E_X(y).$

## Theorem 1 — Existence for fixed breakpoints

For every strict breakpoint sequence $a=x_0<x_1<\cdots<x_n=b$, the set $\mathcal F_X$ is nonempty, closed, and convex, and $V(X)$ is attained.

### Proof

By continuity of $f$ on $[a,b]$, $M=\max_{x\in[a,b]}f(x)$ exists. The vector $y=(M,\ldots,M)$ is feasible, so $\mathcal F_X$ is nonempty.

Each defining inequality of $\mathcal F_X$ is a closed affine half-space in $y$. Hence $\mathcal F_X$ is closed and convex.

Let $h_i=x_{i+1}-x_i>0$. Then

$E_X(y)=\sum_{i=0}^n c_i y_i-\int_a^b f(x)\,dx,$

where $c_0=\frac{h_0}{2}$, $c_i=\frac{h_{i-1}+h_i}{2}$ for $1\le i\le n-1$, and $c_n=\frac{h_{n-1}}{2}$. Thus every $c_i>0$.

Taking $x=x_i$ in the feasibility constraint gives $y_i\ge f(x_i)$. Therefore every sublevel set of $E_X$ inside $\mathcal F_X$ is bounded above and below in every coordinate. It is closed, hence compact.

Since $\mathcal F_X$ is nonempty and $E_X$ is continuous, the minimum is attained.

## Theorem 2 — Fixed-breakpoint problem as a linear semi-infinite program

For fixed $X$, the optimization defining $V(X)$ is a linear semi-infinite program.

### Proof

The objective $E_X$ is affine in $y$, and each feasibility constraint is affine in $y$. There is one constraint for every $x$ in each segment interval.