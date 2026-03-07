import { render, screen } from "@testing-library/react";
import { describe, expect, it } from "vitest";

import { App } from "./App";

describe("App", () => {
  it("renders key dashboard sections", async () => {
    render(<App />);

    expect(await screen.findByRole("heading", { name: /home server, esp32, and pi camera/i })).toBeInTheDocument();
    expect(screen.getByRole("heading", { name: /devices/i })).toBeInTheDocument();
    expect(screen.getByRole("heading", { name: /cameras/i })).toBeInTheDocument();
  });
});
