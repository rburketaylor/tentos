---
type: plan
status: current
updated: 2026-03-06
region: CA
---

# Software and Monorepo Layout

## Current decision

The repo root is now the software monorepo for the grow project.

- Home server stack: backend API, scheduling and alerts, MQTT broker, database, web dashboard
- ESP32 controller node: firmware for direct IO, local schedules, and failsafe behavior
- Pi camera node: Raspberry Pi OS Lite service for USB camera capture and camera status

The `vault/` directory remains the planning and documentation layer, while the repo root holds the actual code and deployment assets.

## Current repo shape

```text
apps/backend                  Python backend service
apps/web                      TypeScript dashboard
firmware/esp32-controller     PlatformIO firmware project
nodes/pi-camera               Pi camera service
shared/contracts              Protocol definitions and topic naming
shared/fixtures               Canonical payload samples for tests and simulation
deploy/compose/home-server    Main Docker Compose stack
deploy/pi-camera              Pi deployment notes and env example
deploy/systemd                Service units for Pi or host installs
scripts/sim                   Fake controller and fake camera publishers
tests                         Integration, contract, firmware, and E2E coverage
```

## Current defaults

- Keep one deployed backend service for v1, but keep its internal modules separated by concern.
- Use MQTT as the main machine-to-machine transport between the home server and edge nodes.
- Keep the ESP32 focused on hardware control and local safety behavior, not the full orchestration layer.
- Keep the Pi camera node as a first-class v1 component because video monitoring is part of this build.
- Keep protocol details in `shared/contracts` and reusable payload examples in `shared/fixtures`.
- Favor lightweight verification: integration tests, contract checks, firmware tests, and a small E2E smoke path.

## Operational notes

- The repo root [`README.md`](/home/burket/Git/tentos/README.md) is now the operator-facing quick-start for local setup and test commands.
- The home server deployment entry point is [`deploy/compose/home-server/docker-compose.yml`](/home/burket/Git/tentos/deploy/compose/home-server/docker-compose.yml).
- The Pi camera deployment entry point is [`deploy/pi-camera/README.md`](/home/burket/Git/tentos/deploy/pi-camera/README.md).

## Related notes

- [[01 Index/Home]]
- [[10 Planning/Current Recommended Build]]
- [[20 Hardware/Controller Architecture]]
- [[20 Hardware/Camera and Network Options]]
- [[50 Parts/Current Inventory]]
