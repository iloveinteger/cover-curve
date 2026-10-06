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

  const constant = value => ({type: "constant", value});
  const variable = () => ({type: "variable"});
  const unary = (op, child) => ({type: op, child});
  const binary = (op, left, right) => ({type: op, left, right});
  const fn = (name, child) => ({type: "function", fn: FUNCTIONS[name], name, child});

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
    return constant(Number(match[0]));
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

    if (/[0-9.]/.test(peek()))
      return parseNumber();

    if (/[A-Za-z_]/.test(peek())) {
      const name = parseIdentifier();

      if (name === "x") return variable();
      if (Object.prototype.hasOwnProperty.call(CONSTANTS, name))
        return constant(CONSTANTS[name]);

      if (Object.prototype.hasOwnProperty.call(FUNCTIONS, name)) {
        consume("(");
        const argument = parseAdditive();
        consume(")");
        return fn(name, argument);
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
      return unary("negate", parseUnary());
    }
    return parsePrimary();
  }

  function parsePower() {
    const left = parseUnary();
    if (peek() === "^") {
      consume("^");
      return binary("power", left, parsePower());
    }
    return left;
  }

  function parseMultiplicative() {
    let value = parsePower();
    while (peek() === "*" || peek() === "/") {
      const op = peek();
      ++pos;
      const rhs = parsePower();
      value = binary(op === "*" ? "multiply" : "divide", value, rhs);
    }
    return value;
  }

  function parseAdditive() {
    let value = parseMultiplicative();
    while (peek() === "+" || peek() === "-") {
      const op = peek();
      ++pos;
      const rhs = parseMultiplicative();
      value = binary(op === "+" ? "add" : "subtract", value, rhs);
    }
    return value;
  }

  const root = parseAdditive();
  if (pos !== source.length)
    throw new Error("Unexpected token at position " + pos + ".");

  function evaluate(node, x) {
    switch (node.type) {
      case "constant": return node.value;
      case "variable": return x;
      case "negate": return -evaluate(node.child, x);
      case "add": return evaluate(node.left, x) + evaluate(node.right, x);
      case "subtract": return evaluate(node.left, x) - evaluate(node.right, x);
      case "multiply": return evaluate(node.left, x) * evaluate(node.right, x);
      case "divide": return evaluate(node.left, x) / evaluate(node.right, x);
      case "power": return Math.pow(evaluate(node.left, x), evaluate(node.right, x));
      case "function": {
        const value = node.fn(evaluate(node.child, x));
        if (!Number.isFinite(value))
          throw new Error("Function '" + node.name + "' returned a non-finite value.");
        return value;
      }
      default:
        throw new Error("Invalid expression node.");
    }
  }

  return function(x) {
    const value = evaluate(root, x);
    if (!Number.isFinite(value))
      throw new Error("Function returned a non-finite value.");
    return value;
  };
}
