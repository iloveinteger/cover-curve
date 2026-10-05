import { oneSegmentCost } from "./oneSegmentCost.js";

export function solveGridDP(f, a, b, n, options = {}) {
  const N = options.N ?? 80;
  if (!Number.isInteger(N) || N < n) {
    throw new Error("N must be an integer with N >= n.");
  }

  const points = Array.from(
    { length: N + 1 },
    (_, i) => a + (b - a) * i / N
  );

  const costs = Array.from({ length: N + 1 }, () =>
    Array(N + 1).fill(Infinity)
  );
  const segmentData = Array.from({ length: N + 1 }, () =>
    Array(N + 1).fill(null)
  );

  for (let i = 0; i < N; i++) {
    for (let j = i + 1; j <= N; j++) {
      const result = oneSegmentCost(
        f,
        points[i],
        points[j],
        options.oneSegment
      );
      costs[i][j] = result.cost;
      segmentData[i][j] = result;
    }
  }

  const dp = Array.from({ length: n + 1 }, () =>
    Array(N + 1).fill(Infinity)
  );
  const parent = Array.from({ length: n + 1 }, () =>
    Array(N + 1).fill(-1)
  );

  dp[0][0] = 0;

  for (let k = 1; k <= n; k++) {
    for (let j = k; j <= N; j++) {
      for (let i = k - 1; i < j; i++) {
        const candidate = dp[k - 1][i] + costs[i][j];
        if (candidate < dp[k][j]) {
          dp[k][j] = candidate;
          parent[k][j] = i;
        }
      }
    }
  }

  const breakpoints = Array(n + 1);
  let j = N;

  for (let k = n; k >= 1; k--) {
    breakpoints[k] = points[j];
    j = parent[k][j];
  }
  breakpoints[0] = a;

  const segments = [];
  for (let k = 0; k < n; k++) {
    const i = Math.round((breakpoints[k] - a) / (b - a) * N);
    const j2 = Math.round((breakpoints[k + 1] - a) / (b - a) * N);
    segments.push({
      x0: breakpoints[k],
      x1: breakpoints[k + 1],
      ...segmentData[i][j2]
    });
  }

  return {
    N,
    n,
    points,
    value: dp[n][N],
    breakpoints,
    segments,
    dp,
    parent
  };
}
