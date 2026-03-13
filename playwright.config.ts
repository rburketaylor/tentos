import { defineConfig, devices } from "@playwright/test";

export default defineConfig({
  testDir: "./tests/e2e",
  use: {
    baseURL: "http://127.0.0.1:4173",
    trace: "on-first-retry"
  },
  projects: [
    {
      name: "desktop",
      use: {
        browserName: "chromium",
        viewport: { width: 1920, height: 1080 }
      }
    },
    {
      name: "iphone-14-pro",
      use: {
        browserName: "chromium",
        ...devices["iPhone 14 Pro"]
      }
    }
  ],
  webServer: {
    command: "npm --workspace apps/web run dev -- --host 127.0.0.1 --port 4173",
    port: 4173,
    reuseExistingServer: !process.env.CI,
    timeout: 120000
  }
});
