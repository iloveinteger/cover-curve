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

  await page.getByRole("button", {name: "Solve curve"}).click();
  await page.waitForFunction(
    () => document.getElementById("value").textContent !== "—",
    null,
    {timeout: 120000}
  );

  const status = await page.locator("#status").textContent();
  const value = await page.locator("#value").textContent();
  const details = await page.locator("#details").textContent();

  if (!status || !status.includes("Solved."))
    throw new Error("Unexpected solver status: " + status);
  if (!value || value === "—" || !details.includes("Breakpoints:"))
    throw new Error("Solver result was not rendered.");

  console.log("Web smoke test passed:", {status, value});
} finally {
  await browser.close();
}
