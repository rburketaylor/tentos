import { expect, test } from "@playwright/test";

test("dashboard renders the core sections", async ({ page }) => {
  await page.goto("/");

  await expect(page.getByRole("heading", { name: /home server, esp32, and pi camera/i })).toBeVisible();
  await expect(page.getByRole("heading", { name: "Devices" })).toBeVisible();
  await expect(page.getByRole("heading", { name: "Cameras" })).toBeVisible();
  await expect(page.getByRole("button", { name: /trigger manual watering/i })).toBeVisible();
});
