# Envelope-SQP implementation

## 1. Architecture

The solver has two numerical layers.

1. directHeightSolveDetailed
   - solves the fixed-breakpoint continuous-height problem by a cutting-plane LP;
   - searches continuously for violated constraints on each segment;
   - returns endpoint heights and positive finite-LP dual multipliers.

2. envelopeSQPSolve
   - optimizes the interior breakpoints;
   - computes an envelope-based sensitivity vector;
   - uses an L-BFGS-style direction;
   - preserves strict breakpoint ordering;
   - backtracks using complete inner solves.

Despite the historical name, the current outer implementation does not solve a classical SQP quadratic-program subproblem. It is an envelope-gradient + safeguarded L-BFGS method.

## 2. Outer pseudocode

    for each seed:
        x <- seed
        current <- DirectHeight(x)

        repeat:
            g <- envelopeSensitivity(x, current)
            if ||g||_infinity <= tolerance:
                stop

            p <- L-BFGS(g)
            if g dot p >= 0:
                p <- -g

            alpha <- largest ordering-preserving step

            while line-search budget remains:
                trial <- x + alpha*p
                if trial is invalid:
                    alpha <- alpha/2
                    continue

                candidate <- DirectHeight(trial)

                if sufficient decrease holds:
                    accept
                    break

                alpha <- alpha/2

            if no step accepted:
                stop

    return best seed result

## 3. Inner oracle

The finite cutting-plane LP is a relaxation of the continuous fixed-breakpoint problem. After each LP solve, the implementation searches
\[
v_i(x)=f(x)-L_i(x)
\]
on every segment and adds a violating contact.

If a certified separation pass establishes
\[
\max_i\max_{x\in[x_i,x_{i+1}]}v_i(x)\le\varepsilon,
\]
then shifting the majorant upward by \(\varepsilon\) gives
\[
LB\le V(X)\le LB+\varepsilon(b-a)
\]
for an exact finite LP. With the current scale-relative stopping rule, use the corresponding scaled epsilon from theory/direct-height.md.

The present support search is numerical, so this is a conditional certificate, not an unconditional black-box theorem.

## 4. Sensitivity formula

The implementation uses the constraint convention
\[
f(z)-L(z)\le0.
\]

For \(z\in[x_i,x_{i+1}]\), \(h=x_{i+1}-x_i\), the contact contribution is
\[
-\lambda\frac{(y_{i+1}-y_i)(x_{i+1}-z)}{h^2}
\]
to \(x_i\), and
\[
-\lambda\frac{(y_{i+1}-y_i)(z-x_i)}{h^2}
\]
to \(x_{i+1}\).

The direct objective contributes
\[
\frac{y_{j-1}-y_{j+1}}2
\]
to interior breakpoint \(x_j\).

Endpoint constraints contribute \(\mu_j f'(x_j)\). Since Function exposes values only, the implementation estimates \(f'\) by a centered finite difference.

The contact signs are regression-tested against finite differences at a nonsingular active set.

## 5. Guarantees

The implementation guarantees only conditional numerical properties:

- accepted steps pass the configured sufficient-decrease test;
- breakpoint ordering is preserved;
- each accepted point is re-solved by the fixed-breakpoint oracle;
- objective values along accepted steps are non-increasing;
- the outer method has no general global-optimality guarantee.

## 6. Complexity

With \(S\) seeds, at most \(K\) accepted outer iterations per seed, and \(L\) line-search trials, the number of inner solves is \(O(SKL)\). If one inner solve costs \(C_{\rm DH}\), total dominant work is \(O(SKLC_{\rm DH})\).
