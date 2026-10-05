```javascript id="r8k2qm"
function simpson(f, a, b, fa, fm, fb) {
  return (b - a) * (fa + 4 * fm + fb) / 6;
}

function adaptiveSimpson(
  f,
  a,
  b,
  fa,
  fm,
  fb,
  whole,
  tolerance,
  depth
) {
  const m = (a + b) / 2;
  const lm = (a + m) / 2;
  const rm = (m + b) / 2;

  const flm = f(lm);
  const frm = f(rm);

  const left = simpson(f, a, m, fa, flm, fm);
  const right = simpson(f, m, b, fm, frm, fb);
  const refined = left + right;

  const error = refined - whole;

  if (
    depth <= 0 ||
    Math.abs(error) <= 15 * tolerance
  ) {
    return refined + error / 15;
  }

  return (
    adaptiveSimpson(
      f,
      a,
      m,
      fa,
      flm,
      fm,
      left,
      tolerance / 2,
      depth - 1
    ) +
    adaptiveSimpson(
      f,
      m,
      b,
      fm,
      frm,
      fb,
      right,
      tolerance / 2,
      depth - 1
    )
  );
}

function adaptiveIntegral(
  f,
  a,
  b,
  tolerance,
  maxDepth
) {
  if (a === b) return 0;

  const m = (a + b) / 2;
  const fa = f(a);
  const fm = f(m);
  const fb = f(b);

  return adaptiveSimpson(
    f,
    a,
    b,
    fa,
    fm,
    fb,
    simpson(f, a, b, fa, fm, fb),
    tolerance,
    maxDepth
  );
}

function adaptiveSupport(
  f,
  u,
  v,
  beta,
  options
) {
  const initialSamples =
    options.supportSamples ?? 32;

  const maxDepth =
    options.supportMaxDepth ?? 8;

  const refinementCount =
    options.supportRefinementCount ?? 32;

  const g = x => f(x) - beta * x;

  let best = {
    x: u,
    value: g(u)
  };

  const rightValue = g(v);

  if (rightValue > best.value) {
    best = {
      x: v,
      value: rightValue
    };
  }

  let intervals = [];

  for (let i = 0; i < initialSamples; i++) {
    const a =
      u + (v - u) * i / initialSamples;

    const b =
      u + (v - u) * (i + 1) / initialSamples;

    const m = (a + b) / 2;

    const fa = g(a);
    const fm = g(m);
    const fb = g(b);

    if (fa > best.value) {
      best = { x: a, value: fa };
    }

    if (fm > best.value) {
      best = { x: m, value: fm };
    }

    if (fb > best.value) {
      best = { x: b, value: fb };
    }

    intervals.push({
      a,
      b,
      fa,
      fm,
      fb,
      depth: 0
    });
  }

  for (let depth = 0; depth < maxDepth; depth++) {
    const scored = intervals.map(interval => {
      const {
        fa,
        fm,
        fb,
        a,
        b
      } = interval;

      const variation =
        Math.abs(fa - 2 * fm + fb);

      const endpointMaximum =
        Math.max(fa, fm, fb);

      return {
        ...interval,
        score:
          endpointMaximum +
          variation +
          (b - a) * 1e-12
      };
    });

    scored.sort((x, y) => y.score - x.score);

    const selected =
      scored.slice(0, refinementCount);

    const next = [];

    for (const interval of selected) {
      const {
        a,
        b,
        fa,
        fm,
        fb
      } = interval;

      const m = (a + b) / 2;
      const lm = (a + m) / 2;
      const rm = (m + b) / 2;

      const flm = g(lm);
      const frm = g(rm);

      if (flm > best.value) {
        best = {
          x: lm,
          value: flm
        };
      }

      if (frm > best.value) {
        best = {
          x: rm,
          value: frm
        };
      }

      next.push(
        {
          a,
          b: m,
          fa,
          fm: flm,
          fb: fm,
          depth: interval.depth + 1
        },
        {
          a: m,
          b,
          fa: fm,
          fm: frm,
          fb,
          depth: interval.depth + 1
        }
      );
    }

    intervals = next;

    if (intervals.length === 0) {
      break;
    }
  }

  for (const interval of intervals) {
    const candidates = [
      {
        x: interval.a,
        value: interval.fa
      },
      {
        x: (interval.a + interval.b) / 2,
        value: interval.fm
      },
      {
        x: interval.b,
        value: interval.fb
      }
    ];

    for (const candidate of candidates) {
      if (candidate.value > best.value) {
        best = candidate;
      }
    }
  }

  return best;
}

function bracketMinimum(
  objective,
  initial,
  options
) {
  const growth =
    options.bracketGrowth ?? 2;

  const initialStep =
    options.initialSlopeStep ?? 1;

  const maxIterations =
    options.bracketIterations ?? 32;

  const centerValue =
    objective(initial);

  let left = initial - initialStep;
  let right = initial + initialStep;

  let leftValue = objective(left);
  let rightValue = objective(right);

  if (
    leftValue >= centerValue &&
    rightValue >= centerValue
  ) {
    return {
      lo: left,
      hi: right
    };
  }

  const direction =
    rightValue < leftValue ? 1 : -1;

  let previous = initial;
  let current =
    direction === 1 ? right : left;

  let currentValue =
    direction === 1
      ? rightValue
      : leftValue;

  let step = initialStep;

  for (let i = 0; i < maxIterations; i++) {
    step *= growth;

    const next =
      current + direction * step;

    const nextValue =
      objective(next);

    if (nextValue >= currentValue) {
      return direction === 1
        ? {
            lo: previous,
            hi: next
          }
        : {
            lo: next,
            hi: previous
          };
    }

    previous = current;
    current = next;
    currentValue = nextValue;
  }

  return direction === 1
    ? {
        lo: previous,
        hi: current
      }
    : {
        lo: current,
        hi: previous
      };
}

function goldenSectionMinimum(
  objective,
  lo,
  hi,
  tolerance,
  maxIterations
) {
  const phi =
    (1 + Math.sqrt(5)) / 2;

  let x1 =
    hi - (hi - lo) / phi;

  let x2 =
    lo + (hi - lo) / phi;

  let y1 = objective(x1);
  let y2 = objective(x2);

  for (let i = 0; i < maxIterations; i++) {
    if (hi - lo <= tolerance) {
      break;
    }

    if (y1 <= y2) {
      hi = x2;
      x2 = x1;
      y2 = y1;
      x1 =
        hi - (hi - lo) / phi;
      y1 = objective(x1);
    } else {
      lo = x1;
      x1 = x2;
      y1 = y2;
      x2 =
        lo + (hi - lo) / phi;
      y2 = objective(x2);
    }
  }

  return (lo + hi) / 2;
}

export function oneSegmentCost(
  f,
  u,
  v,
  options = {}
) {
  if (v <= u) {
    return {
      cost: 0,
      slope: 0,
      intercept: f(u),
      contact: u
    };
  }

  const integralTolerance =
    options.integralTolerance ?? 1e-8;

  const integralMaxDepth =
    options.integralMaxDepth ?? 14;

  const slopeTolerance =
    options.slopeTolerance ?? 1e-8;

  const slopeIterations =
    options.slopeIterations ?? 50;

  const integralF = adaptiveIntegral(
    f,
    u,
    v,
    integralTolerance,
    integralMaxDepth
  );

  function objective(beta) {
    const support =
      adaptiveSupport(
        f,
        u,
        v,
        beta,
        options
      );

    return (
      support.value * (v - u) +
      beta * (v * v - u * u) / 2 -
      integralF
    );
  }

  const initialSlope =
    (f(v) - f(u)) / (v - u);

  const bracket =
    bracketMinimum(
      objective,
      initialSlope,
      options
    );

  const slope =
    goldenSectionMinimum(
      objective,
      bracket.lo,
      bracket.hi,
      slopeTolerance,
      slopeIterations
    );

  const support =
    adaptiveSupport(
      f,
      u,
      v,
      slope,
      options
    );

  return {
    cost: Math.max(
      0,
      objective(slope)
    ),
    slope,
    intercept: support.value,
    contact: support.x
  };
}
```
