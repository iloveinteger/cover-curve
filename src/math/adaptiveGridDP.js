import { solveGridDP } from "./gridDP.js";

export function solveAdaptiveGridDP(
  f,
  a,
  b,
  n,
  options = {}
) {
  const tolerance =
    options.tolerance ?? 1e-6;

  const initialN =
    options.initialN ?? Math.max(32, n);

  const maxN =
    options.maxN ?? 1024;

  if (!Number.isFinite(tolerance) || tolerance <= 0) {
    throw new Error(
      "tolerance must be a positive finite number."
    );
  }

  if (!Number.isInteger(initialN) || initialN < n) {
    throw new Error(
      "initialN must be an integer with initialN >= n."
    );
  }

  if (!Number.isInteger(maxN) || maxN < initialN) {
    throw new Error(
      "maxN must be an integer with maxN >= initialN."
    );
  }

  let N = initialN;
  let previous = null;
  let result = null;
  let relativeChange = Infinity;
  let converged = false;

  while (true) {
    result = solveGridDP(
      f,
      a,
      b,
      n,
      {
        ...options,
        N
      }
    );

    if (previous !== null) {
      relativeChange =
        Math.abs(result.value - previous.value) /
        Math.max(
          1,
          Math.abs(result.value),
          Math.abs(previous.value)
        );

      if (relativeChange < tolerance) {
        converged = true;
        break;
      }
    }

    if (N >= maxN) {
      break;
    }

    const nextN = Math.min(
      N * 2,
      maxN
    );

    if (nextN === N) {
      break;
    }

    previous = result;
    N = nextN;
  }

  return {
    ...result,
    converged,
    relativeChange
  };
}
