let wasmModule = null;

CoverCurve().then((module) => {
  wasmModule = module;
  document.getElementById("status").textContent = "Ready.";
}).catch((error) => {
  document.getElementById("status").textContent = "Failed to load WebAssembly: " + error;
});

function makeFunction(expression) {
  if (!/^[0-9a-zA-Z_+*/%()., ?:[\\]-]+$/.test(expression))
    throw new Error("Function expression contains unsupported characters.");
  return new Function("x", "return (" + expression + ");");
}

function draw(result, f, a, b) {
  const canvas = document.getElementById("plot");
  const ctx = canvas.getContext("2d");
  const w = canvas.width, h = canvas.height;
  ctx.clearRect(0, 0, w, h);

  const values = [];
  for (let i = 0; i <= 500; ++i) {
    const x = a + (b - a) * i / 500;
    values.push([x, Number(f(x))]);
  }
  for (const s of result.segments) {
    for (let i = 0; i <= 40; ++i) {
      const x = s.x0 + (s.x1 - s.x0) * i / 40;
      values.push([x, s.slope * x + s.intercept]);
    }
  }
  const ys = values.map(v => v[1]).filter(Number.isFinite);
  const ymin = Math.min(...ys), ymax = Math.max(...ys);
  const pad = Math.max(1e-9, (ymax - ymin) * 0.08);
  const X = x => (x - a) / (b - a) * (w - 40) + 20;
  const Y = y => h - 20 - (y - (ymin - pad)) / (ymax - ymin + 2 * pad) * (h - 40);

  ctx.beginPath();
  values.slice(0, 501).forEach(([x,y],i) => i ? ctx.lineTo(X(x),Y(y)) : ctx.moveTo(X(x),Y(y)));
  ctx.stroke();

  // Breakpoints and the upper approximation are drawn in the same coordinate system.
  for (const s of result.segments) {
    ctx.beginPath();
    ctx.moveTo(X(s.x0), Y(s.slope*s.x0+s.intercept));
    ctx.lineTo(X(s.x1), Y(s.slope*s.x1+s.intercept));
    ctx.stroke();
  }

  for (const x of result.breakpoints) {
    const y = Math.max(...[f(x), ...result.segments.map(s => s.slope * x + s.intercept)].filter(Number.isFinite));
    ctx.beginPath();
    ctx.arc(X(x), Y(y), 4, 0, 2 * Math.PI);
    ctx.fill();
  }
}

document.getElementById("solve").addEventListener("click", () => {
  const status = document.getElementById("status");
  if (!wasmModule) {
    status.textContent = "WebAssembly is still loading.";
    return;
  }

  try {
    const expression = document.getElementById("expression").value.trim();
    const a = Number(document.getElementById("a").value);
    const b = Number(document.getElementById("b").value);
    const n = Number(document.getElementById("n").value);
    if (!Number.isFinite(a) || !Number.isFinite(b) || !(b > a))
      throw new Error("Require finite a < b.");
    if (!Number.isInteger(n) || n < 1)
      throw new Error("n must be a positive integer.");

    const f = makeFunction(expression);
    const result = wasmModule.solve(f, a, b, n);
    if (result.error) throw new Error(result.error);

    document.getElementById("value").textContent = Number(result.value).toPrecision(10);
    document.getElementById("details").textContent =
      "Breakpoints: " + JSON.stringify(result.breakpoints) + "\n\n" +
      JSON.stringify(result.segments, null, 2);
    draw(result, f, a, b);
    status.textContent = "Solved.";
  } catch (error) {
    status.textContent = error.message || String(error);
  }
});
