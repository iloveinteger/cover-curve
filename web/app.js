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
  if (!ys.length)
    throw new Error("Function produced no finite values on the interval.");

  const ymin = Math.min(...ys), ymax = Math.max(...ys);
  const pad = Math.max(1e-9, (ymax - ymin) * 0.08);
  const X = x => (x - a) / (b - a) * (w - 40) + 20;
  const Y = y => h - 20 - (y - (ymin - pad)) / (ymax - ymin + 2 * pad) * (h - 40);

  ctx.beginPath();
  values.slice(0, 501).forEach(([x, y], i) =>
    i ? ctx.lineTo(X(x), Y(y)) : ctx.moveTo(X(x), Y(y))
  );
  ctx.stroke();

  for (const s of result.segments) {
    ctx.beginPath();
    ctx.moveTo(X(s.x0), Y(s.slope * s.x0 + s.intercept));
    ctx.lineTo(X(s.x1), Y(s.slope * s.x1 + s.intercept));
    ctx.stroke();
  }

  for (const x of result.breakpoints) {
    const y = Math.max(
      ...[f(x), ...result.segments.map(s => s.slope * x + s.intercept)]
        .filter(Number.isFinite)
    );
    ctx.beginPath();
    ctx.arc(X(x), Y(y), 4, 0, 2 * Math.PI);
    ctx.fill();
  }
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
