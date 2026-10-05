function validatePoints(points) {
  if (!Array.isArray(points) || points.length < 2) {
    throw new Error(
      "At least two data points are required."
    );
  }

  for (const point of points) {
    if (
      !Array.isArray(point) ||
      point.length !== 2 ||
      !Number.isFinite(point[0]) ||
      !Number.isFinite(point[1])
    ) {
      throw new Error(
        "Each data point must have the form [x, y] with finite numbers."
      );
    }
  }

  for (let i = 1; i < points.length; i++) {
    if (points[i][0] <= points[i - 1][0]) {
      throw new Error(
        "Data-point x values must be strictly increasing."
      );
    }
  }
}

export function parseDataPoints(text) {
  const lines = text
    .split(/\r?\n/)
    .map(line => line.trim())
    .filter(line => line.length > 0);

  if (lines.length < 2) {
    throw new Error(
      "Enter at least two data points, one point per line."
    );
  }

  const points = lines.map((line, index) => {
    const parts =
      line.split(/[\s,]+/).filter(Boolean);

    if (parts.length !== 2) {
      throw new Error(
        `Invalid data point on line ${index + 1}. Use: x, y`
      );
    }

    const x = Number(parts[0]);
    const y = Number(parts[1]);

    if (
      !Number.isFinite(x) ||
      !Number.isFinite(y)
    ) {
      throw new Error(
        `Invalid numbers on line ${index + 1}.`
      );
    }

    return [x, y];
  });

  validatePoints(points);

  return points;
}

export function createLinearInterpolation(points) {
  validatePoints(points);

  const xs = points.map(point => point[0]);
  const ys = points.map(point => point[1]);

  return function linearInterpolation(x) {
    if (
      x < xs[0] ||
      x > xs[xs.length - 1]
    ) {
      throw new Error(
        `x = ${x} is outside the interpolation interval.`
      );
    }

    if (x === xs[xs.length - 1]) {
      return ys[ys.length - 1];
    }

    let lo = 0;
    let hi = xs.length - 1;

    while (lo + 1 < hi) {
      const mid =
        Math.floor((lo + hi) / 2);

      if (xs[mid] <= x) {
        lo = mid;
      } else {
        hi = mid;
      }
    }

    const t =
      (x - xs[lo]) /
      (xs[lo + 1] - xs[lo]);

    return (
      ys[lo] +
      t * (ys[lo + 1] - ys[lo])
    );
  };
}

export function createNaturalCubicSpline(points) {
  validatePoints(points);

  const n = points.length;

  const xs =
    points.map(point => point[0]);

  const ys =
    points.map(point => point[1]);

  const h = new Array(n - 1);

  for (let i = 0; i < n - 1; i++) {
    h[i] =
      xs[i + 1] - xs[i];
  }

  const lower =
    new Array(n).fill(0);

  const diagonal =
    new Array(n).fill(0);

  const upper =
    new Array(n).fill(0);

  const rhs =
    new Array(n).fill(0);

  diagonal[0] = 1;
  diagonal[n - 1] = 1;

  for (let i = 1; i < n - 1; i++) {
    lower[i] = h[i - 1];

    diagonal[i] =
      2 * (h[i - 1] + h[i]);

    upper[i] = h[i];

    rhs[i] =
      6 * (
        (ys[i + 1] - ys[i]) / h[i] -
        (ys[i] - ys[i - 1]) / h[i - 1]
      );
  }

  const cPrime =
    new Array(n).fill(0);

  const dPrime =
    new Array(n).fill(0);

  cPrime[0] =
    upper[0] / diagonal[0];

  dPrime[0] =
    rhs[0] / diagonal[0];

  for (let i = 1; i < n; i++) {
    const denominator =
      diagonal[i] -
      lower[i] * cPrime[i - 1];

    if (
      !Number.isFinite(denominator) ||
      denominator === 0
    ) {
      throw new Error(
        "Failed to construct the cubic spline."
      );
    }

    cPrime[i] =
      i < n - 1
        ? upper[i] / denominator
        : 0;

    dPrime[i] =
      (
        rhs[i] -
        lower[i] * dPrime[i - 1]
      ) / denominator;
  }

  const secondDerivative =
    new Array(n);

  secondDerivative[n - 1] =
    dPrime[n - 1];

  for (let i = n - 2; i >= 0; i--) {
    secondDerivative[i] =
      dPrime[i] -
      cPrime[i] *
      secondDerivative[i + 1];
  }

  return function naturalCubicSpline(x) {
    if (
      x < xs[0] ||
      x > xs[n - 1]
    ) {
      throw new Error(
        `x = ${x} is outside the interpolation interval.`
      );
    }

    if (x === xs[n - 1]) {
      return ys[n - 1];
    }

    let lo = 0;
    let hi = n - 1;

    while (lo + 1 < hi) {
      const mid =
        Math.floor((lo + hi) / 2);

      if (xs[mid] <= x) {
        lo = mid;
      } else {
        hi = mid;
      }
    }

    const interval = h[lo];
    const t = x - xs[lo];

    const A =
      (xs[lo + 1] - x) / interval;

    const B =
      (x - xs[lo]) / interval;

    return (
      A * ys[lo] +
      B * ys[lo + 1] +
      (
        (A * A * A - A) *
          secondDerivative[lo] +
        (B * B * B - B) *
          secondDerivative[lo + 1]
      ) *
        interval *
        interval /
        6
    );
  };
}
