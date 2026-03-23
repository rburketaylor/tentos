# Current State Assessment

Date: 2026-03-23

This note captures the current relationship between project planning and implementation so future planning changes can reference one stable assessment instead of rediscovering the same gaps.

## Executive Summary

The repo is not yet the full grow-control system described in the vault and roadmap. It is currently:

- a coherent seeded backend and web vertical slice
- a real ESP32 MQTT-connected single-relay light controller
- a placeholder Pi camera service and deployment path
- a contracts/fixtures layer that is ahead of the implementation in several areas

The main mismatch is that the planning docs and roadmap assume a schema-aligned grow controller for pump, fan, temperature, humidity, and camera integration, while the actual firmware is still centered on light toggling plus photoresistor-based verification.

## Intended Architecture

The current planning layer describes this target v1 architecture:

- home server or main host for backend API, orchestration, logging, scheduling, alerts, dashboard, MQTT broker, and eventually persistence
- ESP32 controller for direct IO, local safety behavior, pumps, valves, fans, and sensors
- Raspberry Pi camera node as a first-class v1 component because video monitoring is in scope
- MQTT as the default machine-to-machine boundary

Key references:

- [Software and Monorepo Layout](../vault/10%20Planning/Software%20and%20Monorepo%20Layout.md)
- [Current Recommended Build](../vault/10%20Planning/Current%20Recommended%20Build.md)
- [Controller Architecture](../vault/20%20Hardware/Controller%20Architecture.md)
- [Implementation Roadmap](./implementation-roadmap.md)

## What Is Already Implemented

### Backend and API

- FastAPI app with `/health` and read-only `/api/v1/devices`, `/schedules`, `/alerts`, `/cameras`, and `/media`
- In-memory repository loading seeded fixtures from `shared/fixtures`
- Response models for the seeded REST resources
- Minimal automation health summary, though it is still placeholder data

Relevant sources:

- [apps/backend/src/grow_backend/main.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/main.py)
- [apps/backend/src/grow_backend/api/routes.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/api/routes.py)
- [apps/backend/src/grow_backend/storage/repository.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/storage/repository.py)
- [apps/backend/src/grow_backend/models.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/models.py)
- [apps/backend/src/grow_backend/automation/service.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/automation/service.py)

### Web Dashboard

- Dashboard fetches live API collections for devices, cameras, alerts, and schedules
- Explicit seeded mode via `VITE_USE_SEEDED_DATA=true`
- Client-side response validation
- Honest API error handling instead of silent seeded fallback
- A presentational camera panel and manual watering button

Relevant sources:

- [apps/web/src/App.tsx](/home/burket/Git/tentos/apps/web/src/App.tsx)
- [apps/web/src/dashboardData.ts](/home/burket/Git/tentos/apps/web/src/dashboardData.ts)

### Firmware

- Real Wi-Fi and MQTT connectivity with reconnect logic
- Subscription to a command topic
- Retained availability and state publishes
- Periodic telemetry publishes
- Single relay pulse control
- Analog light-sensor sampling and photoresistor-based state verification

Relevant sources:

- [firmware/esp32-controller/src/main.cpp](/home/burket/Git/tentos/firmware/esp32-controller/src/main.cpp)
- [firmware/esp32-controller/include/light_control.h](/home/burket/Git/tentos/firmware/esp32-controller/include/light_control.h)
- [firmware/esp32-controller/include/controller_config.example.h](/home/burket/Git/tentos/firmware/esp32-controller/include/controller_config.example.h)

### Contracts and Fixtures

- JSON schemas for REST collection responses
- Schemas and fixtures for `device-command`, `device-telemetry`, `camera-state`, `media-record`, and `alert`
- Protocol document covering REST resources and MQTT topic naming

Relevant sources:

- [shared/contracts/protocol.yaml](/home/burket/Git/tentos/shared/contracts/protocol.yaml)
- [shared/contracts](/home/burket/Git/tentos/shared/contracts/README.md)
- [shared/fixtures](/home/burket/Git/tentos/shared/fixtures)

### Deployment and Packaging

- Minimal compose stack for `backend` + `web`
- Backend and web Dockerfiles
- Pi camera package, env example, and systemd unit

Relevant sources:

- [deploy/compose/home-server/docker-compose.yml](/home/burket/Git/tentos/deploy/compose/home-server/docker-compose.yml)
- [deploy/compose/home-server/backend.Dockerfile](/home/burket/Git/tentos/deploy/compose/home-server/backend.Dockerfile)
- [deploy/compose/home-server/web.Dockerfile](/home/burket/Git/tentos/deploy/compose/home-server/web.Dockerfile)
- [deploy/pi-camera/README.md](/home/burket/Git/tentos/deploy/pi-camera/README.md)
- [deploy/systemd/grow-pi-camera.service](/home/burket/Git/tentos/deploy/systemd/grow-pi-camera.service)

## What Is Partial or Placeholder

### Backend

- `automation` status is placeholder-only
- service helpers in `devices/service.py` and `camera/service.py` are thin seeded wrappers
- configuration includes MQTT and database settings, but there is no corresponding live integration

### Web

- "Trigger Manual Watering" has no handler and no backend route behind it
- camera panel links to `stream_url` but does not embed a live MJPEG stream
- dashboard ignores `/api/v1/media` and `/api/v1/health`

### Pi Camera

- `/stream` is explicitly a placeholder response
- `/state` is environment-backed rather than capture-backed
- no snapshot endpoint, no capture pipeline, no MQTT publishing

### Simulators

- current simulator scripts only print fixture JSON
- they do not publish MQTT, simulate state changes, or exercise backend/web flows

Relevant sources:

- [nodes/pi-camera/src/grow_camera/main.py](/home/burket/Git/tentos/nodes/pi-camera/src/grow_camera/main.py)
- [nodes/pi-camera/src/grow_camera/capture.py](/home/burket/Git/tentos/nodes/pi-camera/src/grow_camera/capture.py)
- [scripts/sim/fake_controller.py](/home/burket/Git/tentos/scripts/sim/fake_controller.py)
- [scripts/sim/fake_camera.py](/home/burket/Git/tentos/scripts/sim/fake_camera.py)

## What Is Still Missing

### Backend Runtime Features

- MQTT broker in the compose stack
- backend MQTT subscriber bridge
- live merge of device and camera state into repository responses
- mutation endpoints for operator actions
- auth, CORS, dependency injection, and structured error handling
- persistence and time-series storage

### Protocol and Schema Gaps

- `device-state.schema.json`
- `camera-event.schema.json`
- fixtures for those missing payloads

### Pi Camera Runtime

- real camera streaming implementation
- live state publishing over MQTT
- backend ingestion of live camera state
- dashboard embedding of the live stream

## Major Mismatches Between Plan and Implementation

### Firmware vs Shared Contracts

The biggest mismatch in the repo is the firmware boundary.

Planned and documented contract:

- JSON command payloads like `{node_id, command, issued_at}`
- climate-style telemetry like `{temperature_c, humidity_rh, moisture_pct, status, timestamp}`
- roadmap phases built on top of pump, fan, and sensor support

Actual firmware behavior:

- plain-text commands: `light_on`, `light_off`, and optional `:pulse_ms`
- telemetry fields focused on observed light state, desired light state, command status, and sensor average
- retained state payload with no matching schema in `shared/contracts`
- no pump control, no fan control, no AHT20/AHT21 support, no schedule refresh logic, no NTP-backed timestamps

Relevant sources:

- [shared/contracts/device-command.schema.json](/home/burket/Git/tentos/shared/contracts/device-command.schema.json)
- [shared/contracts/device-telemetry.schema.json](/home/burket/Git/tentos/shared/contracts/device-telemetry.schema.json)
- [firmware/esp32-controller/src/main.cpp](/home/burket/Git/tentos/firmware/esp32-controller/src/main.cpp)

### Planning Docs vs Current Runtime

- docs describe the home server as the backend, broker, database, and orchestration host
- current implementation is still explicitly seeded and read-only
- some notes still leave room for HTTP transport, while the rest of the plan assumes MQTT
- some notes still recommend timer-first irrigation, while the current build notes and shopping list say not to plan around a standalone timer

Relevant sources:

- [README.md](/home/burket/Git/tentos/README.md)
- [docs/vault/20 Hardware/Controller Architecture.md](/home/burket/Git/tentos/docs/vault/20%20Hardware/Controller%20Architecture.md)
- [docs/vault/10 Planning/Budget Shopping Guide.md](/home/burket/Git/tentos/docs/vault/10%20Planning/Budget%20Shopping%20Guide.md)
- [docs/vault/50 Parts/Shopping List.md](/home/burket/Git/tentos/docs/vault/50%20Parts/Shopping%20List.md)

## Recommended Cleanup and Planning Adjustments

### 1. Decide the actual controller direction

Pick one of these and align the roadmap to it:

- continue with the current light-controller-first firmware and update planning/contracts accordingly
- pivot the firmware immediately to the grow-controller schema in the roadmap and treat the current light controller as an experiment or precursor

Without this decision, the roadmap and firmware will keep diverging.

### 2. Separate "current state" from "target state" in planning

The docs should explicitly distinguish:

- what exists in the repo today
- what is planned next
- what is intentionally deferred

This would remove most of the current ambiguity.

### 3. Remove or rewrite stale planning guidance

Candidates:

- optional HTTP transport language in controller architecture
- standalone timer recommendations that conflict with current build guidance
- roadmap references whose rationale is not traceable to a vault note

### 4. Mark placeholder assets clearly

If they are not about to be implemented, consider marking them as placeholder or deferred in docs:

- [deploy/compose/home-server/mosquitto.conf](/home/burket/Git/tentos/deploy/compose/home-server/mosquitto.conf)
- [scripts/sim/fake_controller.py](/home/burket/Git/tentos/scripts/sim/fake_controller.py)
- [scripts/sim/fake_camera.py](/home/burket/Git/tentos/scripts/sim/fake_camera.py)
- [apps/backend/src/grow_backend/automation/service.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/automation/service.py)
- [apps/backend/src/grow_backend/devices/service.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/devices/service.py)
- [apps/backend/src/grow_backend/camera/service.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/camera/service.py)

## Verification Notes

Verified during this assessment:

- web unit tests passed
- contract tests passed
- firmware layout tests passed
- firmware native protocol tests were reported passing in subagent review

Not fully verified during this assessment:

- backend integration tests in [tests/integration/test_backend_api.py](/home/burket/Git/tentos/tests/integration/test_backend_api.py) hung in this environment
- Playwright E2E was not run
- real runtime behavior for MQTT, camera streaming, and simulators is not implemented enough to verify end-to-end

## Suggested Next Reference Use

Use this note when deciding:

- which existing plans should be removed or rewritten
- which placeholders should be kept versus deleted
- whether to align the implementation to the roadmap or revise the roadmap to match the working firmware direction
