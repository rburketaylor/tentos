import { render, screen, within } from "@testing-library/react";
import { afterEach, describe, expect, it, vi } from "vitest";

import { App } from "./App";

const liveResponses = {
  "/api/v1/devices": [
    {
      id: "tent-controller-01",
      name: "Tent Controller",
      status: "online",
      temperature_c: 24.3,
      humidity_rh: 59.1,
      moisture_pct: 42.0,
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
} as Record<string, unknown>;

afterEach(() => {
  vi.restoreAllMocks();
});

describe("App", () => {
  it("renders key dashboard sections from live API data", async () => {
    vi.spyOn(globalThis, "fetch").mockImplementation(async (input) => {
      const url = typeof input === "string" ? input : input instanceof URL ? input.toString() : input.url;
      const pathname = new URL(url, "http://127.0.0.1").pathname;
      const payload = liveResponses[pathname];

      return new Response(JSON.stringify(payload ?? []), {
        status: payload ? 200 : 404,
        headers: {
          "Content-Type": "application/json",
        },
      });
    });

    render(<App />);

    expect(await screen.findByRole("heading", { name: /tent operations at a glance/i })).toBeInTheDocument();
    expect(screen.getByRole("heading", { name: /devices/i })).toBeInTheDocument();
    expect(screen.getByRole("heading", { name: /cameras/i })).toBeInTheDocument();
    expect(screen.getByRole("heading", { name: /alerts/i })).toBeInTheDocument();
    const sourceStrip = await screen.findByText(/validated backend responses are flowing into the dashboard/i);
    expect(within(sourceStrip.closest(".source-strip")!).getByText("Live API")).toBeInTheDocument();
  });

  it("surfaces API failures without falling back to seeded data", async () => {
    vi.spyOn(globalThis, "fetch").mockResolvedValue(
      new Response(JSON.stringify({ error: "unavailable" }), {
        status: 503,
        headers: {
          "Content-Type": "application/json",
        },
      }),
    );

    const { container } = render(<App />);

    const sourceStrip = container.querySelector(".source-strip") as HTMLElement | null;
    expect(sourceStrip).not.toBeNull();
    expect(await within(sourceStrip!).findByText("API Error")).toBeInTheDocument();
    expect(screen.getByText(/not masking that failure with seed data/i)).toBeInTheDocument();
    expect(screen.getAllByText(/no .* data loaded/i).length).toBeGreaterThan(0);
  });
});
