# Implementation Roadmap

Phased plan to complete the grow automation system. Tasks ordered by dependency and user priority.

See also: [Current State Assessment (2026-03-23)](./current-state-assessment-2026-03-23.md) for a snapshot of what is already implemented, what is still placeholder-level, and where the plan currently diverges from the codebase.

---

## Phase 1 — Protocol & Schema Alignment (Critical Path)

**Goal:** Eliminate the mismatch between firmware payloads and shared contracts so every MQTT message is schema-valid and bidirectionally parseable.

### 1.1 Firmware: Accept JSON commands

- Add `bblanchon/ArduinoJson` to `platformio.ini` `[env:esp32dev]` lib_deps
- Update `handleCommand()` in `main.cpp` to parse incoming JSON matching `device-command.schema.json`:
  ```json
  {"node_id":"tent-controller-01","command":"light_on","duration_seconds":10,"issued_at":"2026-03-20T12:00:00Z"}
  ```
- Map `command` enum to existing logic: `light_on`/`light_off` → relay pulse; `pump_on`/`pump_off` → stub handler (returns `ignored` for now); `refresh_schedule` → `ignored`
- Convert `duration_seconds` to `pulse_ms` for light relay
- Reject malformed JSON gracefully (log, don't crash)
- Backward compatibility: if payload is not valid JSON, attempt legacy string parse (deprecation path)

### 1.2 Firmware: Align telemetry payload to schema

- Attempt NTP time sync in `setup()` (`configTime(0, 0, "pool.ntp.org")`) with a bounded startup wait (for example 30s), then retry in the background
- Do not block command handling or relay safety on time sync; if time is still unavailable after the startup window, mark the node `degraded` and defer schema-validated telemetry/state publishes until UTC is available
- Restructure `publishTelemetryTick()` JSON to match `device-telemetry.schema.json`:
  - `node_id`, `temperature_c` (placeholder `0.0` until Phase 3), `humidity_rh` (placeholder `0.0`), `status` (online/offline/degraded), `moisture_pct` (null), `timestamp`
- Move `sensor_avg` to state topic only (or add `additionalProperties` to telemetry schema)

### 1.3 Firmware: Add timestamps to state payload

- Add `timestamp` and `node_id` fields to state topic payload

### 1.4 Schemas: Create missing schemas

- Create `shared/contracts/device-state.schema.json` for `grow/{node_id}/state` payload
- Create `shared/contracts/camera-event.schema.json` for `grow/camera/{camera_id}/event` topic
- Create corresponding fixture files in `shared/fixtures/`

### 1.5 Backend: MQTT bridge receives telemetry

- Add MQTT subscriber to consume `grow/+/telemetry` and `grow/+/state`
- Deserialize against schemas, store latest values per device
- Wire into in-memory repository so `GET /api/v1/devices` returns live data

### 1.6 Contracts: Add light commands to command schema

- Update `device-command.schema.json` enum to include `light_on` and `light_off`
- Update fixture `device-command.json`

### 1.7 Tests

- Firmware Unity tests: JSON command parsing (valid, malformed, unknown, light_on with duration, legacy string fallback)
- Contract tests for new schemas and fixtures
- Update integration tests if backend serves live MQTT data

**Acceptance:**
- [ ] Firmware accepts JSON commands matching `device-command.schema.json`; legacy strings still work
- [ ] Telemetry payload validates against `device-telemetry.schema.json`
- [ ] Time sync uses a bounded startup wait and never blocks core control/safety loops indefinitely
- [ ] All published telemetry/state payloads include valid UTC timestamps once sync is available
- [ ] `device-state.schema.json` and `camera-event.schema.json` exist with fixtures
- [ ] `device-command.schema.json` includes `light_on`/`light_off`
- [ ] All existing tests pass; new tests cover JSON parsing

---

## Phase 2 — Camera Live Stream

**Goal:** Replace placeholder `/stream` with real video via mjpg-streamer. *Independent of Phase 1 — can run in parallel.*

### 2.1 Pi camera: Install mjpg-streamer

- Add mjpg-streamer to Pi deployment (install script or instructions in `deploy/pi-camera/`)
- Start as background process (subprocess managed by FastAPI or separate systemd unit)
- Default output: MJPEG on `localhost:8080`

### 2.2 Pi camera: Implement /stream endpoint

- Replace placeholder in `nodes/pi-camera/src/grow_camera/main.py` with a stable public `/stream` endpoint that proxies or redirects to local mjpg-streamer
- Treat the FastAPI `/stream` route as the canonical stream URL exposed to the rest of the system; mjpg-streamer stays an internal implementation detail on `localhost:8080`
- Update `/state` so `stream_url` points to the Pi camera service URL (for example `http://<pi-ip>:8100/stream`)

### 2.3 Pi camera: Publish camera state via MQTT

- Add `paho-mqtt` to Pi camera service
- Publish to `grow/camera/{camera_id}/state` with `stream_url`, `status`, `timestamp`
- Validate against `camera-state.schema.json`

### 2.4 Backend: Ingest camera state and surface it to the dashboard

- Add MQTT subscriber support for `grow/camera/+/state`
- Deserialize against `camera-state.schema.json`, store the latest values per camera, and merge with seeded repository data
- Wire `GET /api/v1/cameras` to return live camera state when available so the web app can stay API-driven

### 2.5 Web dashboard: Embed camera stream

- Add camera panel to React dashboard
- Display MJPEG via `<img src="stream_url">` (native browser support)
- Camera status indicator (online/offline)

### 2.6 Tests

- Playwright E2E: camera section renders
- Python tests: Pi camera `/state` and `/stream` endpoints
- Backend tests: camera MQTT state updates `/api/v1/cameras`

**Acceptance:**
- [ ] Pi camera serves live MJPEG via `/stream`
- [ ] Camera state published to MQTT, matches `camera-state.schema.json`
- [ ] Backend `/api/v1/cameras` reflects live camera MQTT state
- [ ] Web dashboard displays live camera feed
- [ ] `/stream` is no longer a placeholder

---

## Phase 3 — Temperature & Humidity Sensor

**Goal:** Wire AHT20/AHT21 so firmware publishes real `temperature_c` and `humidity_rh`.

### 3.1 Firmware: I2C + AHT20/AHT21 driver

- Wire sensor to ESP32 I2C bus (GPIO 21/22 SDA/SCL, configurable)
- Add `adafruit/Adafruit AHTX0` to `platformio.ini` lib_deps
- Sensor init in `setup()` with error handling (log + `degraded` status if not found)
- Periodic read alongside telemetry interval

### 3.2 Firmware: Populate telemetry fields

- Replace `0.0` placeholders from Phase 1 with real readings
- Set `status` to `degraded` on I2C failure, NaN, or out-of-range

### 3.3 Firmware: Add to state payload

- Include `temperature_c` and `humidity_rh` in state topic

### 3.4 Contracts

- Update `device-state.schema.json` if adding temp/humidity to state
- Add fixture with representative values

### 3.5 Tests

- Unity test: sensor value serialization (mock reading, verify JSON)
- Update integration tests for real sensor data

**Acceptance:**
- [ ] Firmware reads temperature and humidity from AHT20/AHT21
- [ ] Telemetry has real `temperature_c` and `humidity_rh`
- [ ] `degraded` status on sensor failure
- [ ] All tests pass

---

## Phase 4 — Irrigation Pump Control

**Goal:** `pump_on`/`pump_off` commands end-to-end with PWM-driven pump.

### 4.1 Firmware: PWM pump output

- Select PWM-capable GPIO for pump MOSFET gate (e.g., GPIO 19)
- Configure LEDC PWM channel (25 kHz or suitable frequency)
- State tracking: `pump_running`, `pump_speed` (0–255), `pump_end_time`

### 4.2 Firmware: Pump command handlers

- `pump_on`: require `duration_seconds`, set PWM duty to configured default (or full 255), start timer, and auto-stop on expiry
- Reject or ignore `pump_on` commands that omit `duration_seconds` rather than allowing indefinite runtime
- `pump_off`: set PWM to 0, clear state
- State payload fields: `pump` (idle/running), `pump_speed` (int), `pump_remaining_s` (int)

### 4.3 Firmware: Safety

- `kPumpMaxRunMs` constant (e.g., 10 min) — auto-shutoff
- Pump off on firmware restart (GPIO defaults off)

### 4.4 Contracts and backend

- Update `device-state.schema.json` with pump fields
- Update `device-command.schema.json` and fixture(s) so `pump_on` requires `duration_seconds` while `pump_off` does not
- Backend: pass pump commands through MQTT

### 4.5 Web dashboard

- Pump on/off button, status display, remaining time
- Optional: speed slider

### 4.6 Tests

- Unity tests: pump command parsing, PWM clamping, max-run safety
- Contract tests for updated schemas
- Playwright E2E for pump controls

**Acceptance:**
- [ ] `pump_on`/`pump_off` JSON commands drive PWM output
- [ ] Auto-stops after duration or max-run-time
- [ ] Pump state in MQTT state topic and dashboard
- [ ] `pump_on` without an explicit duration is rejected

---

## Phase 5 — Backend Hardening

**Goal:** Production-ready backend with auth, structured errors, DI.

### 5.1 API authentication

- API key middleware for non-browser clients (`X-API-Key` header or `Authorization: Bearer <key>`)
- Configurable via `GROW_API_KEY` env var
- `/health` stays unauthenticated
- Do not ship a long-lived backend API key to the browser via `VITE_*`
- For the web dashboard, use either a backend-issued session cookie/login flow or trusted reverse-proxy auth on the home server; reserve raw API keys for non-browser clients and service-to-service access

### 5.2 Structured error handling

- Global exception handler returning consistent JSON:
  `{"error": {"code": "...", "message": "...", "details": ...}}`
- Handle: 422, 404, 500, 401/403
- Request logging middleware with correlation IDs

### 5.3 Dependency injection

- Replace module-level `repository` singleton with `Depends()` injection
- Define `Repository` protocol/interface
- Enables future swap to persistent storage

### 5.4 CORS

- Add CORS middleware
- Allow origins from `GROW_CORS_ORIGINS` env var (default: `http://localhost:4173,http://localhost:5173`)

### 5.5 Dependency version policy

- Widen FastAPI pin from `>=0.135,<0.136` to `>=0.135,<1.0`
- Review other overly narrow pins

### 5.6 Tests

- Auth middleware tests (valid/missing/invalid key)
- Browser-facing auth tests for the chosen dashboard mechanism (session cookie or reverse-proxy integration boundary)
- Error format tests (404, 422, 500)
- DI repository tests (mock repository)
- Update existing integration tests to pass credentials through the supported non-browser path

**Acceptance:**
- [ ] All non-health endpoints require authenticated access through the supported client path
- [ ] Consistent JSON error responses
- [ ] Repository injected via `Depends()`
- [ ] CORS configured
- [ ] Tests updated and passing

---

## Phase 6 — Fan & Exhaust Control

**Goal:** Fan relay with environment-driven automation.

### 6.1 Contracts

- Add `fan_on`/`fan_off` to `device-command.schema.json` enum
- Update state schema with `fan` field (idle/running/auto)
- Update fixtures

### 6.2 Firmware: Fan GPIO and commands

- GPIO pin for fan relay or MOSFET (e.g., GPIO 17)
- `fan_on`/`fan_off` handlers (same pattern as light)
- Fan state in MQTT state payload
- Optional: variable speed via PWM

### 6.3 Backend: Environment automation

- Automation module: read temp/RH thresholds from config (initial targets from `docs/vault/30 Growing/Environment Targets.md`)
- Compare against live MQTT telemetry
- Publish `fan_on`/`fan_off` on threshold crossings
- Hysteresis (fan on at 28°C, off at 26°C)
- Stage-based profiles: seedling, veg, flower

### 6.4 Web dashboard

- Fan toggle, auto/manual mode selector
- Show state and automation status
- Display threshold settings

### 6.5 Tests

- Firmware Unity tests: fan command parsing
- Backend tests: automation thresholds and hysteresis
- Contract tests for updated schemas
- Playwright tests for fan controls

**Acceptance:**
- [ ] `fan_on`/`fan_off` work end-to-end (MQTT → ESP32 → GPIO)
- [ ] Backend automation triggers fan with hysteresis
- [ ] Fan state in MQTT and dashboard
- [ ] Manual override works

---

## Phase 7 — Soil Moisture (Conditional)

**Goal:** Wire capacitive probe if irrigation feedback becomes needed. *Implement only if overwatering is observed.*

### 7.1 Firmware

- Capacitive probe on ADC pin (e.g., GPIO 35)
- Read alongside photoresistor in sampling loop
- Calibrate dry/wet thresholds (experiment sketch)
- Publish `moisture_pct` in telemetry (calibrated 0–100%)

### 7.2 Backend

- Extend automation to optionally use `moisture_pct` as irrigation trigger
- Hysteresis: water below X%, stop at Y%

### 7.3 Dashboard

- Moisture gauge/trend on device panel

---

## Phase 8 — Deferred & Future Work

Explicitly deferred per vault docs. Implement only when the trigger condition is met.

| Feature | Trigger | Source |
|---|---|---|
| Per-plant valve routing | Plants need independent schedules | Irrigation Architecture |
| Nutrient dosing pumps | Manual mixing is the main pain | Irrigation Architecture |
| VPD computation | Basic temp/RH targets insufficient | Environment Targets |
| Supplemental heat | Lights-off room runs cold | Environment Targets |
| Carbon filter | Odor actually matters | Current Recommended Build |
| Multiple cameras | >1 camera node deployed | code-review-issues |
| OTA firmware updates | USB access impractical | (new requirement) |
| Persistent storage | Seeded data insufficient | code-review-issues |
| Time-series storage | Historical trends needed | (new requirement) |
| Alert notifications | Push/email/SMS needed | (new requirement) |
| Arduino IO expansion | ESP32 pins exhausted | Controller Architecture |

---

## Dependency Graph

```
Phase 1 (Protocol) ──┬──> Phase 3 (Temp/Humidity)
                     ├──> Phase 4 (Pump) ──> Phase 7 (Soil Moisture)
                     └──> Phase 6 (Fan)

Phase 2 (Camera)     [independent, parallel with Phase 1]
Phase 5 (Backend)    [mostly parallel; auth rollout must account for active API consumers]
```

Phase 1 is the main protocol critical path for Phases 3, 4, and 6. Phase 2 can proceed in parallel. Most of Phase 5 can also proceed in parallel, with auth changes sequenced carefully around the web client and any operator workflows.

---

## Estimated Effort

| Phase | Size | Key risk |
|---|---|---|
| 1 — Protocol alignment | Large | Firmware JSON refactor; backward compat adds complexity |
| 2 — Camera stream | Medium | Pi hardware dependencies; mjpg-streamer setup varies by model |
| 3 — Temp/Humidity | Small | Straightforward I2C integration |
| 4 — Pump control | Medium | PWM + safety logic; need MOSFET hardware to test |
| 5 — Backend hardening | Medium | Need a real browser auth model, not a bundled frontend API key |
| 6 — Fan control | Medium | Automation adds backend state |
| 7 — Soil moisture | Small | Calibration-dependent |
