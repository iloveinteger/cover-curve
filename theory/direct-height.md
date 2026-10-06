# Direct-height cutting-plane solver

## 1. Fixed-breakpoint problem

Fix
$$\na=x_0<\cdots<x_n=b.\n$$
With vertex heights $y_i=g(x_i)$, minimize
$$\nE_X(y)=c(X)^Ty-\int_a^b f\n$$
subject to
$$\nf(x)-w(x)^Ty\le0\n\\\qquad\n(x\in[x_i,x_{i+1}]).\n$$

This is a linear semi-infinite program: finitely many variables and a continuum of linear inequalities.

## 2. Cutting-plane relaxation

Let $S$ be a finite set of sampled contacts. The restricted LP keeps only
$$\nf(z)-w(z)^Ty\le0,\\\qquad z\in S.\n$$

Its feasible set contains the feasible set of the full problem. Therefore
$$\nLB(S)\\\le V(X).\n$$

After solving the restricted LP, define
$$\nv_i(x)=f(x)-L_i(x)\n$$
and use a separation oracle to find
$$\nM_i=\max_{x\in[x_i,x_{i+1}]}v_i(x).\n$$

If $M_i>0$, the maximizer is a violated constraint and is added to $S$.

Pseudocode:

    S <- one interior sample per segment

    repeat for at most the configured round budget:
        y <- solve finite LP(S)
        for each segment i:
            z_i <- approximate argmax of f(x)-L_i(x)
            if violation(z_i) > tolerance:
                S <- S union {z_i}

        if no segment has a violating point:
            stop

    perform one final separation pass
    return y

## 3. Exact-oracle correctness

Suppose the separation oracle returns a true global maximizer on every segment.

If an iteration terminates with
$$\n\max_i M_i\le0,\n$$
then the current $y$ is feasible for the full problem. Because $y$ was optimal for a relaxation,
$$\nLB(S)\\\le V(X)\\\le E_X(y).\n$$

If the finite LP is solved exactly, the restricted optimum equals $E_X(y)$, so equality holds:
$$\nLB(S)=V(X).\n$$

Thus finite termination with an exact separation oracle gives an exact fixed-breakpoint optimum.

Finite termination is not guaranteed for every semi-infinite LP; an exchange sequence can in principle generate infinitely many contacts. Therefore the finite round budget is not an exactness theorem.

## 4. Numerical tolerance certificate

Suppose the separation oracle establishes
$$\n\max_i M_i\\\le\varepsilon.\n$$
Then
$$\n\widetilde g(x)=g(x)+\varepsilon\n$$
is feasible and
$$\nE(\widetilde g)=E(g)+\varepsilon(b-a).\n$$

Hence, for an exact restricted LP,
$$\n\\\boxed{\nLB(S)\\\le V(X)\\\le E(g)+\varepsilon(b-a).\n}\n$$

The implementation uses a scale-relative tolerance. If
$$\nM_i\\\le\varepsilon s_i,\n$$
then the analogous bound is
$$\nV(X)\\\le E(g)+\varepsilon(b-a)\max_i s_i\n$$
provided the separation bound is genuinely global.

The current black-box support search is numerical rather than formally certified, so the displayed inequality is a certification theorem conditional on a certified global separation result.

## 5. Dual multipliers

For the finite restricted LP, dual multipliers are nonnegative and satisfy complementary slackness. The implementation stores positive multipliers for retained contacts.

These multipliers are used as sensitivity information by the outer breakpoint solver. They are not claimed to be the exact continuous dual measure unless the exchange process has captured an exact optimal active set.

## 6. Complexity

Let $d=n+1$ be the number of height variables, $m$ the number of retained constraints, $R$ the number of cutting-plane rounds, and $S_{\rm sep}(n,\tau)$ the cost of separating one segment.

Then
$$\nT=\nO\!\\left(\nR\,[T_{\rm LP}(d,m)+nS_{\rm sep}(n,\tau)]\n\right).\n$$

The current LP is a dense two-phase simplex implementation. Simplex has no polynomial worst-case complexity guarantee, so no polynomial worst-case bound is claimed.

## 7. Numerical error budget

The returned value is affected by LP floating-point error, incomplete separation, numerical integration, support-search tolerance, and the finite cutting-plane budget. These are implementation errors, distinct from the exact fixed-breakpoint optimization error.

## 8. Role in the full solver

Direct-height is an inner fixed-breakpoint oracle.

- fastGridDP / adaptiveGridDP: breakpoint-grid global/coarse methods;
- directHeight: fixed-breakpoint continuous-height oracle;
- coordinateSearch: coordinatewise nonconvex outer optimization;
- Envelope-SQP: envelope-sensitivity nonconvex outer optimization.
