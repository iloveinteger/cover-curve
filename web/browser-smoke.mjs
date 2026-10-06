import { chromium } from "playwright";

const browser = await chromium.launch({headless: true});
const page = await browser.newPage();

try {
  await page.goto("http://127.0.0.1:8080/web/", {waitUntil: "networkidle"});
  await page.waitForFunction(
    () => document.getElementById("solve").disabled === false,
    null,
    {timeout: 30000}
  );

  const cases = [
  {expression: "x^2", a: 0, b: 1, n: 1},
  {expression: "x^2", a: -5, b: 5, n: 2},
  {expression: "sin(x)", a: 0, b: 6.283185307179586, n: 2},
  {expression: "sin(x)", a: -10, b: 10, n: 4},
  {expression: "ln(x)", a: 0.01, b: 0.1, n: 2},
  {expression: "ln(x)", a: 0.1, b: 2, n: 4},
  {expression: "x^4 - 2*x^2 + x", a: -1, b: 1, n: 3},
  {expression: "x^4 - 2*x^2 + x", a: -10, b: 10, n: 5},
  {expression: "x + sin(x)", a: 0, b: 3, n: 5},
  {expression: "x + sin(x)", a: 0, b: 5, n: 5}
];

  for (const test of cases) {
    await page.locator("#expression").fill(test.expression);
    await page.locator("#a").fill(String(test.a));
    await page.locator("#b").fill(String(test.b));
    await page.locator("#n").fill(String(test.n));

    await page.getByRole("button", {name: "Solve curve"}).click();
    await page.waitForFunction(
      () => document.getElementById("value").textContent !== "—" &&
            document.getElementById("status").textContent.includes("Solved."),
      null,
      {timeout: 120000}
    );

    const value = await page.locator("#value").textContent();
    const details = await page.locator("#details").textContent();

    if (!value || value === "—" || !details.includes("Breakpoints:"))
      throw new Error("Solver result was not rendered for " + test.expression);

    console.log("Web solver passed:", {expression: test.expression, value});
  }
} finally {
  await browser.close();
}
