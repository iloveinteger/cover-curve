# Interpolation

The library accepts sampled data through interpolation functions that produce the same generic callable type used by the core solver.

## Piecewise-linear interpolation

Given ordered data points $(x_i,y_i)$, the linear interpolant uses the unique affine function on each consecutive interval $[x_i,x_{i+1}]$.

This is the most direct representation when the supplied data itself is intended to describe a piecewise-linear curve.

## Natural cubic spline

The natural cubic spline constructs a $C^2$ piecewise-cubic interpolant with zero second derivative at the two endpoints.

It provides a smoother callable when the sampled values are measurements of an underlying smooth curve.

## Domain requirements

Data points must be ordered in $x$ and have distinct $x$ coordinates. The interpolation domain must contain the interval requested by the cover-curve solver.

The core optimization code does not depend on which interpolation method is used: it only evaluates the resulting callable.

