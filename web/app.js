let solverReady = false;
const worker = new Worker("solver-worker.js");

const status = document.getElementById("status");
const solveButton = document.getElementById("solve");

status.textContent = "Loading solver...";
solveButton.disabled = true;

worker.onmessage = (event) => {
  const data = event.data;

  if (data.type === "ready") {
    solverReady = true;
    solveButton.disabled = false;
    status.textContent = "Ready.";
    return;
  }

  if (data.type === "load-error") {
    solverReady = false;
    solveButton.disabled = true;
    status.textContent = "Failed to load WebAssembly: " + data.error;
    return;
  }

  if (data.type === "error") {
    solveButton.disabled = false;
    status.textContent = data.error;
    return;
  }

  if (data.type === "result") {
    solveButton.disabled = false;

    if (data.error) {
      status.textContent = data.error;
      return;
    }

    const a = Number(document.getElementById("a").value);
    const b = Number(document.getElementById("b").value);
    const expression = document.getElementById("expression").value.trim();

    document.getElementById("value").textContent =
      Number(data.value).toPrecision(10);
    document.getElementById("details").textContent =
      "Breakpoints: " + JSON.stringify(data.breakpoints) + "\n\n" +
      JSON.stringify(data.segments, null, 2);

    try {
      const f = makeFunction(expression);
      draw(data, f, a, b);
      status.textContent = "Solved.";
    } catch (error) {
      status.textContent = error.message || String(error);
    }
  }
};

function makeFunction(expression) {
  const source = expression.replace(/\s+/g, "");
  if (!source)
    throw new Error("Enter a function expression.");

  let pos = 0;

  function peek() {
    return source[pos] || "";
  }

  function consume(char) {
    if (source[pos] !== char)
      throw new Error("Expected '" + char + "' at position " + pos + ".");
    ++pos;
  }

  function parseNumber() {
    const match = source.slice(pos).match(/^(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?/);
    if (!match)
      throw new Error("Expected a number at position " + pos + ".");
    pos += match[0].length;
    return Number(match[0]);
  }

  function parseIdentifier() {
    const match = source.slice(pos).match(/^[A-Za-z_][A-Za-z0-9_]*/);
    if (!match)
      throw new Error("Expected an identifier at position " + pos + ".");
    pos += match[0].length;
    return match[0];
  }

  function parsePrimary() {
    if (peek() === "(") {
      consume("(");
      const value = parseAdditive();
      consume(")");
      return value;
    }

    if (/[0-9.]/.test(peek()))
      return parseNumber();

    if (/[A-Za-z_]/.test(peek())) {
      const name = parseIdentifier();

      if (name === "x")
        return currentX;

      if (Object.prototype.hasOwnProperty.call(CONSTANTS, name))
        return CONSTANTS[name];

      if (Object.prototype.hasOwnProperty.call(FUNCTIONS, name)) {
        consume("(");
        const argument = parseAdditive();
        consume(")");
        const value = FUNCTIONS[name](argument);
        if (!Number.isFinite(value))
          throw new Error("Function '" + name + "' returned a non-finite value.");
        return value;
      }

      throw new Error("Unknown identifier '" + name + "'.");
    }

    throw new Error("Unexpected token at position " + pos + ".");
  }

  function parseUnary() {
    if (peek() === "+") {
      consume("+");
      return parseUnary();
    }
    if (peek() === "-") {
      consume("-");
      return -parseUnary();
    }
    return parsePrimary();
  }

  function parsePower() {
    const left = parseUnary();
    if (peek() === "^") {
      consume("^");
      const right = parsePower();
      return Math.pow(left, right);
    }
    return left;
  }

  function parseMultiplicative() {
    let value = parsePower();
    while (peek() === "*" || peek() === "/") {
      const op = peek();
      ++pos;
      const rhs = parsePower();
      value = op === "*" ? value * rhs : value / rhs;
    }
    return value;
  }

  function parseAdditive() {
    let value = parseMultiplicative();
    while (peek() === "+" || peek() === "-") {
      const op = peek();
      ++pos;
      const rhs = parseMultiplicative();
      value = op === "+" ? value + rhs : value - rhs;
    }
    return value;
  }

  let currentX = 0;

  return function(x) {
    currentX = x;
    pos = 0;
    const value = parseAdditive();
    if (pos !== source.length)
      throw new Error("Unexpected token at position " + pos + ".");
    if (!Number.isFinite(value))
      throw new Error("Function returned a non-finite value.");
    return value;
  };
}

function draw(result, f, a, b) {
  const canvas = document.getElementById("plot");
  const ctx = canvas.getContext("2d");
  const dpr = window.devicePixelRatio || 1;
  const cssWidth = canvas.clientWidth || 900;
  const cssHeight = Math.max(420, Math.min(560, cssWidth * 0.56));
  canvas.width = Math.round(cssWidth * dpr);
  canvas.height = Math.round(cssHeight * dpr);
  ctx.setTransform(dpr, 0, 0, dpr, 0, 0);

  const w = cssWidth;
  const h = cssHeight;
  ctx.clearRect(0, 0, w, h);

  const samples = [];
  for (let i = 0; i <= 600; ++i) {
    const x = a + (b - a) * i / 600;
    const y = Number(f(x));
    if (Number.isFinite(y)) samples.push([x, y]);
  }
  if (samples.length < 2)
    throw new Error("Function produced too few finite values.");

  const curveValues = samples.map(p => p[1]);
  const upperValues = [];
  for (const seg of result.segments) {
    for (let i = 0; i <= 50; ++i) {
      const x = seg.x0 + (seg.x1 - seg.x0) * i / 50;
      upperValues.push(seg.slope * x + seg.intercept);
    }
  }

  const allY = curveValues.concat(upperValues).filter(Number.isFinite);
  let ymin = Math.min(...allY);
  let ymax = Math.max(...allY);
  if (ymax - ymin < 1e-12) {
    ymin -= 1;
    ymax += 1;
  }

  const yPad = (ymax - ymin) * 0.12;
  ymin -= yPad;
  ymax += yPad;

  const margin = {left: 58, right: 20, top: 28, bottom: 44};
  const plotW = w - margin.left - margin.right;
  const plotH = h - margin.top - margin.bottom;
  const X = x => margin.left + (x - a) / (b - a) * plotW;
  const Y = y => margin.top + (ymax - y) / (ymax - ymin) * plotH;

  function niceStep(range, targetTicks) {
    const raw = range / targetTicks;
    const power = Math.pow(10, Math.floor(Math.log10(raw)));
    const normalized = raw / power;
    let factor = 1;
    if (normalized >= 5) factor = 5;
    else if (normalized >= 2) factor = 2;
    return factor * power;
  }

  function decimals(step) {
    return Math.max(0, Math.min(8, Math.ceil(-Math.log10(step))));
  }

  const xStep = niceStep(b - a, 7);
  const yStep = niceStep(ymax - ymin, 6);
  const xDigits = decimals(xStep);
  const yDigits = decimals(yStep);
  const xStart = Math.ceil(a / xStep) * xStep;
  const yStart = Math.ceil(ymin / yStep) * yStep;

  ctx.font = '12px system-ui, sans-serif';
  ctx.lineWidth = 1;

  // Plot background.
  ctx.fillStyle = "#ffffff";
  ctx.fillRect(0, 0, w, h);

  // Grid and tick labels.
  ctx.strokeStyle = "#edf0f5";
  ctx.fillStyle = "#737c8d";
  ctx.textAlign = "center";
  ctx.textBaseline = "top";

  for (let x = xStart; x <= b + xStep * 0.001; x += xStep) {
    const px = X(x);
    ctx.beginPath();
    ctx.moveTo(px, margin.top);
    ctx.lineTo(px, h - margin.bottom);
    ctx.stroke();
    ctx.fillText(x.toFixed(xDigits), px, h - margin.bottom + 9);
  }

  ctx.textAlign = "right";
  ctx.textBaseline = "middle";
  for (let y = yStart; y <= ymax + yStep * 0.001; y += yStep) {
    const py = Y(y);
    ctx.beginPath();
    ctx.moveTo(margin.left, py);
    ctx.lineTo(w - margin.right, py);
    ctx.stroke();
    ctx.fillText(y.toFixed(yDigits), margin.left - 9, py);
  }

  // Axes when zero is in view.
  ctx.strokeStyle = "#aeb6c5";
  ctx.lineWidth = 1.2;
  if (a <= 0 && b >= 0) {
    const px = X(0);
    ctx.beginPath();
    ctx.moveTo(px, margin.top);
    ctx.lineTo(px, h - margin.bottom);
    ctx.stroke();
  }
  if (ymin <= 0 && ymax >= 0) {
    const py = Y(0);
    ctx.beginPath();
    ctx.moveTo(margin.left, py);
    ctx.lineTo(w - margin.right, py);
    ctx.stroke();
  }

  // f(x)
  ctx.strokeStyle = "#315efb";
  ctx.lineWidth = 2.4;
  ctx.beginPath();
  samples.forEach(([x, y], i) => {
    const px = X(x), py = Y(y);
    if (i === 0) ctx.moveTo(px, py);
    else ctx.lineTo(px, py);
  });
  ctx.stroke();

  // Upper piecewise-linear cover.
  ctx.strokeStyle = "#e06b2f";
  ctx.lineWidth = 2.6;
  for (const seg of result.segments) {
    ctx.beginPath();
    ctx.moveTo(X(seg.x0), Y(seg.slope * seg.x0 + seg.intercept));
    ctx.lineTo(X(seg.x1), Y(seg.slope * seg.x1 + seg.intercept));
    ctx.stroke();
  }

  // Breakpoints.
  ctx.fillStyle = "#e06b2f";
  ctx.strokeStyle = "#ffffff";
  ctx.lineWidth = 2;
  for (const x of result.breakpoints) {
    const candidates = [
      Number(f(x)),
      ...result.segments
        .filter(seg => x >= seg.x0 - 1e-10 && x <= seg.x1 + 1e-10)
        .map(seg => seg.slope * x + seg.intercept)
    ].filter(Number.isFinite);
    if (!candidates.length) continue;

    const y = Math.max(...candidates);
    ctx.beginPath();
    ctx.arc(X(x), Y(y), 4.5, 0, 2 * Math.PI);
    ctx.fill();
    ctx.stroke();
  }

  // Axis labels.
  ctx.fillStyle = "#626b7b";
  ctx.font = '600 12px system-ui, sans-serif';
  ctx.textAlign = "center";
  ctx.textBaseline = "bottom";
  ctx.fillText("x", margin.left + plotW / 2, h - 5);

  ctx.save();
  ctx.translate(15, margin.top + plotH / 2);
  ctx.rotate(-Math.PI / 2);
  ctx.fillText("f(x)", 0, 0);
  ctx.restore();

  // Legend.
  const legendY = 12;
  ctx.font = '600 12px system-ui, sans-serif';
  ctx.textAlign = "left";
  ctx.textBaseline = "middle";

  ctx.strokeStyle = "#315efb";
  ctx.lineWidth = 2.4;
  ctx.beginPath();
  ctx.moveTo(margin.left, legendY);
  ctx.lineTo(margin.left + 24, legendY);
  ctx.stroke();
  ctx.fillStyle = "#4b5565";
  ctx.fillText("f(x)", margin.left + 31, legendY);

  const secondX = margin.left + 90;
  ctx.strokeStyle = "#e06b2f";
  ctx.lineWidth = 2.6;
  ctx.beginPath();
  ctx.moveTo(secondX, legendY);
  ctx.lineTo(secondX + 24, legendY);
  ctx.stroke();
  ctx.fillStyle = "#4b5565";
  ctx.fillText("upper cover", secondX + 31, legendY);
}

solveButton.addEventListener("click", () => {
  if (!solverReady) {
    status.textContent = "Solver is still loading.";
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

    makeFunction(expression)(0);
    solveButton.disabled = true;
    status.textContent = "Solving…";
    worker.postMessage({expression, a, b, n});
  } catch (error) {
    status.textContent = error.message || String(error);
  }
});
