# Grow Monorepo

This repository holds the software side of the grow project:

- `apps/backend`: home-server API, automation, alerts, and MQTT bridge
- `apps/web`: operator dashboard
- `firmware/esp32-controller`: PlatformIO firmware for the controller node
- `nodes/pi-camera`: Raspberry Pi USB camera service
- `shared/contracts`: protocol definitions shared across services
- `shared/fixtures`: canonical payloads for tests and simulators
- `deploy/compose/home-server`: Docker Compose stack for the home server

The `vault/` directory remains the planning and documentation layer.

## Quick Start

### Python services

```bash
python -m venv .venv
.venv/bin/python -m pip install -e '.[dev]' -e apps/backend -e nodes/pi-camera
.venv/bin/uvicorn grow_backend.main:app --app-dir apps/backend/src --reload
```

In another shell:

```bash
.venv/bin/uvicorn grow_camera.main:app --app-dir nodes/pi-camera/src --reload --port 8100
```

### Web app

```bash
npm install
npm --workspace apps/web run dev
```

### Firmware

```bash
pio run -d firmware/esp32-controller
pio test -d firmware/esp32-controller -e native
```

Firmware configuration lives under `firmware/esp32-controller/include/`:

- `controller_config.example.h` is the tracked template
- `controller_config.local.h` is your ignored local file for Wi-Fi and MQTT settings
- `controller_config.h` includes the local file when present and otherwise falls back to the example

For local hardware work, copy the example to the local file and edit your real values there:

```bash
cp firmware/esp32-controller/include/controller_config.example.h \
  firmware/esp32-controller/include/controller_config.local.h
```

The production controller firmware now uses the outlet indicator light on `GPIO34`
for verification and accepts desired-state MQTT commands on
`grow/tent-controller-01/command`:

- `light_on`
- `light_off`
- optional pulse override forms such as `light_on:750`

### Tests

```bash
python -m venv .venv
.venv/bin/pytest
npm --workspace apps/web run test
npm run test:e2e
```

## Repo Tasks

The root [`justfile`](/home/burket/Git/tentos/justfile) contains the common local commands for backend, web, firmware, simulators, and Docker Compose.
