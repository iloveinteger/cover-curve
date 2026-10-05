const FUNCTIONS = {
  sin: Math.sin, cos: Math.cos, tan: Math.tan,
  asin: Math.asin, acos: Math.acos, atan: Math.atan,
  exp: Math.exp, log: Math.log, sqrt: Math.sqrt,
  abs: Math.abs, floor: Math.floor, ceil: Math.ceil
};

const CONSTANTS = { pi: Math.PI, e: Math.E };

function makeFunction(expression) {
  const source = expression.replace(/\s+/g, "");
  if (!source) throw new Error("Enter a function expression.");

  let pos = 0;
  let currentX = 0;

  function peek() { return source[pos] || ""; }

  function consume(char) {
    if (source[pos] !== char)
      throw new Error("Expected '" + char + "' at position " + pos + ".");
    ++pos;
  }

  function parseNumber() {
    const match = source.slice(pos).match(/^(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][+-]?\d+)?/);
    if (!match) throw new Error("Expected a number at position " + pos + ".");
    pos += match[0].length;
    return Number(match[0]);
  }

  function parseIdentifier() {
    const match = source.slice(pos).match(/^[A-Za-z_][A-Za-z0-9_]*/);
    if (!match) throw new Error("Expected an identifier at position " + pos + ".");
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

    if (/[0-9.]/.test(peek())) return parseNumber();

    if (/[A-Za-z_]/.test(peek())) {
      const name = parseIdentifier();
      if (name === "x") return currentX;
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
    if (peek() === "+") { consume("+"); return parseUnary(); }
    if (peek() === "-") { consume("-"); return -parseUnary(); }
    return parsePrimary();
  }

  function parsePower() {
    const left = parseUnary();
    if (peek() === "^") { consume("^"); return Math.pow(left, parsePower()); }
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
