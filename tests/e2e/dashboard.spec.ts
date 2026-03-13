import { expect, test } from "@playwright/test";

test("dashboard renders live API data", async ({ page }) => {
  await page.route("**/api/v1/**", async (route) => {
    const pathname = new URL(route.request().url()).pathname;
    const payloads: Record<string, unknown> = {
      "/api/v1/devices": [
        {
          id: "tent-controller-01",
          name: "Tent Controller",
          status: "online",
          temperature_c: 24.3,
          humidity_rh: 59.1,
          moisture_pct: 42,
          active_schedule_id: "feed-veg-am",
          last_seen: "2026-03-06T12:00:00Z",
        },
      ],
      "/api/v1/cameras": [
        {
          id: "pi-camera-01",
          name: "Tent Canopy Cam",
          status: "online",
          stream_url: "http://127.0.0.1:8100/stream",
          last_snapshot_url: null,
          last_seen: "2026-03-06T12:00:00Z",
        },
      ],
      "/api/v1/alerts": [
        {
          id: "camera-signal-lag",
          source: "pi-camera-01",
          level: "warning",
          message: "Camera node reported delayed snapshot upload.",
          active: true,
          created_at: "2026-03-06T12:00:00Z",
        },
      ],
      "/api/v1/schedules": [
        {
          id: "feed-veg-am",
          device_id: "tent-controller-01",
          name: "Morning Feed",
          enabled: true,
          start_time: "08:00",
          duration_seconds: 18,
          description: "Primary irrigation pulse for vegetative schedule.",
        },
      ],
    };

    await route.fulfill({
      status: pathname in payloads ? 200 : 404,
      contentType: "application/json",
      body: JSON.stringify(payloads[pathname] ?? []),
    });
  });

  await page.goto("/");

  await expect(page.getByRole("heading", { name: /tent operations at a glance/i })).toBeVisible();
  await expect(page.getByRole("heading", { name: "Devices" })).toBeVisible();
  await expect(page.getByRole("heading", { name: "Cameras" })).toBeVisible();
  await expect(page.getByRole("heading", { name: "Alerts" })).toBeVisible();
  await expect(page.locator(".source-strip .badge")).toHaveText("Live API");
  await expect(page.getByRole("button", { name: /trigger manual watering/i })).toBeVisible();
});

test("dashboard surfaces API failures instead of silently showing seed data", async ({ page }) => {
  await page.route("**/api/v1/**", async (route) => {
    await route.fulfill({
      status: 503,
      contentType: "application/json",
      body: JSON.stringify({ error: "unavailable" }),
    });
  });

  await page.goto("/");

  await expect(page.locator(".source-strip .badge")).toHaveText("API Error");
  await expect(page.getByText(/not masking that failure with seed data/i)).toBeVisible();
  await expect(page.getByText("No device data loaded.")).toBeVisible();
});
