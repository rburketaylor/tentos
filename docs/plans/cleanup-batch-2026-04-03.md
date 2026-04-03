# Repo Cleanup — Batched Fixes

## Context

Code review identified 24 cleanup items across the tentos monorepo. After verification and triage, we're implementing the quick wins plus the design-decision items the user approved. Media endpoint is intentional scaffolding — leave it alone.

---

## Phase 1: Quick wins (safe deletions and one-line fixes)

### 1a. Delete orphaned `__pycache__` from removed backend modules
- `rm -rf apps/backend/src/grow_backend/camera/`
- `rm -rf apps/backend/src/grow_backend/devices/`
- `rm -rf apps/backend/src/grow_backend/mqtt/`
- These directories contain only `__pycache__` — source files were already deleted.

### 1b. Remove `.badge-fallback` from CSS
- **File:** `apps/web/src/styles.css` (line 473)
- Remove `.badge-fallback,` from the selector, keeping `.badge-seeded, .status-degraded, .pill-warning`

### 1c. Remove unnecessary React default import
- **File:** `apps/web/src/main.tsx` (line 1)
- Delete `import React from "react";`

### 1d. Add `.claude/` to `.gitignore`
- **File:** `.gitignore`
- Add `.claude/` after the `.vscode/` line (line 46), in the "Local IDE state" section

### 1e. Remove stale `.cursor` from `.dockerignore`
- **File:** `.dockerignore` (line 4)
- Delete the `.cursor` line

### 1f. Simplify redundant firmware relay pin setup
- **File:** `firmware/esp32-controller/src/main.cpp` (lines 431-435)
- Replace the 5-line block with:
  ```cpp
  digitalWrite(grow::config::kRelayPin, relayIdleLevel());
  pinMode(grow::config::kRelayPin, OUTPUT);
  ```
- The first `pinMode(INPUT_PULLUP/PULLDOWN)` was immediately overwritten by `pinMode(OUTPUT)`.

---

## Phase 2: Schema and naming fixes

### 2a. Merge command sets in device-command schema
- **File:** `shared/contracts/device-command.schema.json` (line 8)
- Change enum from `["pump_on", "pump_off", "refresh_schedule"]` to `["light_on", "light_off", "pump_on", "pump_off", "refresh_schedule"]`
- Firmware handles `light_*` today; `pump_*` and `refresh_schedule` are for future controller versions.

### 2b. Normalize snapshot_url → last_snapshot_url in MQTT schema
- **File:** `shared/contracts/camera-state.schema.json` (line 10)
- Rename `"snapshot_url"` to `"last_snapshot_url"`
- Backend model, HTTP response, and frontend all already use `last_snapshot_url`.

### 2c. Remove bare `/health` endpoint, keep only `/api/v1/health`
- **File:** `apps/backend/src/grow_backend/main.py` (lines 11-17)
- Delete the `@app.get("/health")` route and `healthcheck()` function
- `/api/v1/health` in `routes.py` is a strict superset and becomes the only health endpoint.

---

## Phase 3: Config robustness

### 3a. Improve repository_mode validation error
- **File:** `apps/backend/src/grow_backend/config.py` (line 10)
- Keep `Literal["seeded"]` but add startup validation that catches bad env var values and prints a clear message like `"GROW_REPOSITORY_MODE must be 'seeded', got 'foo'"`.

---

## Files modified (summary)

| File | Change |
|------|--------|
| `apps/backend/src/grow_backend/camera/` | Delete directory |
| `apps/backend/src/grow_backend/devices/` | Delete directory |
| `apps/backend/src/grow_backend/mqtt/` | Delete directory |
| `apps/web/src/styles.css` | Remove `.badge-fallback` from selector |
| `apps/web/src/main.tsx` | Remove `import React from "react"` |
| `.gitignore` | Add `.claude/` |
| `.dockerignore` | Remove `.cursor` |
| `firmware/esp32-controller/src/main.cpp` | Simplify relay pin setup |
| `shared/contracts/device-command.schema.json` | Expand enum |
| `shared/contracts/camera-state.schema.json` | Rename field |
| `apps/backend/src/grow_backend/main.py` | Remove `/health` endpoint |
| `apps/backend/src/grow_backend/config.py` | Improve validation error |

## Verification

1. `just check` — run linting/type-checking across the repo
2. `just test` — run unit + integration tests
3. `just dev` — confirm backend starts without errors
4. Verify firmware still compiles: `cd firmware/esp32-controller && just compile`
