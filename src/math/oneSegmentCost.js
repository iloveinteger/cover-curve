// Numerical baseline for the continuous one-segment problem.
//
// For a fixed slope beta, the smallest feasible intercept is
//
//   alpha(beta) = max_x (f(x) - beta*x).
//
// The maximum is approximated by sampling [u, v], and the resulting
// one-dimensional objective is minimized numerically.

export function simpson(values, h) {
  const n = values.length - 1;

  if (n < 2 || n % 2 !== 0) {
    throw new Error("Simpson integration requires an even number of intervals.");
  }

  let sum = values[0] + values[n];

  for (let i = 1; i < n; i++) {
    sum += (i % 2 === 0 ? 2 : 4) * values[i];
  }

  return sum * h / 3;
}

function sampledSupport(f, u, v, beta, samples) {
  let bestX = u;
  let bestValue = f(u) - beta * u;

  for (let i = 1; i <= samples; i++) {
    const x = u + (v - u) * i / samples;
    const value = f(x) - beta * x;

    if (value > bestValue) {
      bestValue = value;
      bestX = x;
    }
  }

  return {
    x: bestX,
    value: bestValue
  };
}

export function oneSegmentCost(f, u, v, options = {}) {
  if (v <= u) {
    return {
      cost: 0,
      slope: 0,
      intercept: f(u),
      contact: u
    };
  }

  const samples = options.samples ?? 128;
  const slopeIterations = options.slopeIterations ?? 50;

  let minSlope = Infinity;
  let maxSlope = -Infinity;

  for (let i = 0; i < samples; i++) {
    const x1 = u + (v - u) * i / samples;
    const x2 = u + (v - u) * (i + 1) / samples;
    const slope = (f(x2) - f(x1)) / (x2 - x1);

    minSlope = Math.min(minSlope, slope);
    maxSlope = Math.max(maxSlope, slope);
  }

  const span = Math.max(1, maxSlope - minSlope);
  const lo0 = minSlope - 2 * span;
  const hi0 = maxSlope + 2 * span;

  const integralSamples = samples % 2 === 0 ? samples : samples + 1;
  const h = (v - u) / integralSamples;

  const fValues = Array.from(
    { length: integralSamples + 1 },
    (_, i) => f(u + i * h)
  );

  const integralF = simpson(fValues, h);

  function objective(beta) {
    const support = sampledSupport(f, u, v, beta, samples);

    return (
      support.value * (v - u) +
      beta * (v * v - u * u) / 2 -
      integralF
    );
  }

  const phi = (1 + Math.sqrt(5)) / 2;

  let lo = lo0;
  let hi = hi0;

  let x1 = hi - (hi - lo) / phi;
  let x2 = lo + (hi - lo) / phi;

  let y1 = objective(x1);
  let y2 = objective(x2);

  for (let iter = 0; iter < slopeIterations; iter++) {
    if (y1 <= y2) {
      hi = x2;
      x2 = x1;
      y2 = y1;
      x1 = hi - (hi - lo) / phi;
      y1 = objective(x1);
    } else {
      lo = x1;
      x1 = x2;
      y1 = y2;
      x2 = lo + (hi - lo) / phi;
      y2 = objective(x2);
    }
  }

  const slope = (lo + hi) / 2;
  const support = sampledSupport(f, u, v, slope, samples);
  const intercept = support.value;
  const cost = objective(slope);

  return {
    cost: Math.max(0, cost),
    slope,
    intercept,
    contact: support.x
  };
}
