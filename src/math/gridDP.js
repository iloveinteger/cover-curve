import { oneSegmentCost } from "./oneSegmentCost.js";

export function solveGridDP(f, a, b, n, options = {}) {
  const N = options.N ?? 80;
  const oneSegmentOptions = options.oneSegment ?? {};

  if (typeof f !== "function") {
    throw new Error("f must be a function.");
  }

  if (!Number.isInteger(n) || n < 1) {
    throw new Error("n must be a positive integer.");
  }

  if (!Number.isInteger(N) || N < n) {
    throw new Error("N must be an integer with N >= n.");
  }

  if (
    !Number.isFinite(a) ||
    !Number.isFinite(b) ||
    a >= b
  ) {
    throw new Error("Require finite a < b.");
  }

  const points = Array.from(
    { length: N + 1 },
    (_, i) => a + (b - a) * i / N
  );

  const costs = Array.from(
    { length: N + 1 },
    () => Array(N + 1).fill(Infinity)
  );

  const segmentData = Array.from(
    { length: N + 1 },
    () => Array(N + 1).fill(null)
  );

  for (let i = 0; i < N; i++) {
    for (let j = i + 1; j <= N; j++) {
      const result = oneSegmentCost(
        f,
        points[i],
        points[j],
        oneSegmentOptions
      );

      if (!Number.isFinite(result.cost)) {
        throw new Error(
          `Failed to compute segment cost for [${points[i]}, ${points[j]}].`
        );
      }

      costs[i][j] = result.cost;
      segmentData[i][j] = result;
    }
  }

  const dp = Array.from(
    { length: n + 1 },
    () => Array(N + 1).fill(Infinity)
  );

  const parent = Array.from(
    { length: n + 1 },
    () => Array(N + 1).fill(-1)
  );

  dp[0][0] = 0;

  for (let k = 1; k <= n; k++) {
    for (let j = k; j <= N; j++) {
      for (let i = k - 1; i < j; i++) {
        if (!Number.isFinite(dp[k - 1][i])) {
          continue;
        }

        const candidate =
          dp[k - 1][i] + costs[i][j];

        if (candidate < dp[k][j]) {
          dp[k][j] = candidate;
          parent[k][j] = i;
        }
      }
    }
  }

  if (!Number.isFinite(dp[n][N])) {
    throw new Error("No feasible DP solution was found.");
  }

  const breakpointIndices = new Array(n + 1);
  breakpointIndices[n] = N;

  let j = N;

  for (let k = n; k >= 1; k--) {
    const i = parent[k][j];

    if (i < 0) {
      throw new Error("Failed to reconstruct the optimal path.");
    }

    breakpointIndices[k - 1] = i;
    j = i;
  }

  if (breakpointIndices[0] !== 0) {
    throw new Error("Invalid reconstructed breakpoint path.");
  }

  const breakpoints =
    breakpointIndices.map(i => points[i]);

  const segments = [];

  for (let k = 0; k < n; k++) {
    const i = breakpointIndices[k];
    const j2 = breakpointIndices[k + 1];
    const data = segmentData[i][j2];

    if (data === null) {
      throw new Error("Missing segment data.");
    }

    segments.push({
      x0: points[i],
      x1: points[j2],
      ...data
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
