---
type: reference
status: current
updated: 2026-03-06
region: CA
---

# Controller Architecture

## Current recommendation

Use a split architecture:

- Main host: webserver, database, logging, scheduling, alerting, dashboards
- ESP32 controller node: GPIO, sensor reads, pumps, valves, fans, heaters
- Optional Pi node: camera or local Linux-only service near the grow

## Why this replaced the older Pi-centered design

- ESP32 boards are cheaper than keeping a Pi on hardware duty.
- ESP32 is better suited to GPIO-heavy and timing-sensitive work.
- The main host is already the natural place for the monitoring and control application.
- A Pi still makes sense for camera streaming, but not as the default controller if the same job can be done with ESP32 plus the main host.

## Default topology

```text
Main host / webserver
        |
        | Wi-Fi or LAN
        v
ESP32 controller node ---- sensors / MOSFETs / valves / pump / fan control

Optional:
Pi camera node ---------- camera stream / snapshots
```

## One job per box

- Main host: orchestration, storage, UI, and long-running services
- ESP32: direct hardware control and local sensor reads
- Pi node: camera capture or Linux-only edge tasks
- Router bridge: only if it saves buying separate Wi-Fi hardware for an older Pi

## Communication defaults

- Use MQTT or a simple HTTP API between the main host and ESP32.
- Keep the controller node state machine simple: threshold logic, scheduled runs, failsafe defaults.
- Do not push full decision logic into the ESP32 unless offline operation is a real requirement.
- WebSockets are optional for live dashboards, not the default control path.

## Hardware defaults

- For DC loads, use logic-level MOSFET switching.
- For mains loads, use properly rated relays or smart plugs outside the tent.
- Keep the control boards and power conversion outside the humid area whenever possible.
- Bring only sensor leads and low-risk wiring into the tent.

## Controller board guidance

- Cheapest sufficient default: ESP32-S2 Mini class board.
- Use a larger ESP32 dev board only if you need more pins, ADC channels, or easier prototyping.
- Use Raspberry Pi Zero 2 W only when you need camera support or Linux userspace at the edge.
- An Arduino is still usable as sensor or IO expansion, but not the first board to buy for this build.

## Related notes

- [[10 Planning/Current Recommended Build]]
- [[10 Planning/Software and Monorepo Layout]]
- [[10 Planning/Budget Shopping Guide]]
- [[20 Hardware/Camera and Network Options]]
- [[20 Hardware/Power and Protection]]
- [[20 Hardware/Irrigation Architecture]]
- [[20 Hardware/RF Outlet Remote Hack]]
