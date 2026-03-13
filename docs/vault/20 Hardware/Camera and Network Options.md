---
type: reference
status: current
updated: 2026-03-05
region: CA
---

# Camera and Network Options

## Current default

- Do not add a camera node unless video monitoring actually matters.
- If you do add one, prefer a Pi 2 B or similar full Linux node over an older Pi Zero v1.3 unless size is the main constraint.

## Current project decision

- Video monitoring does matter for this build, so the Pi 2 B camera node is part of v1.
- The repo now includes a dedicated `nodes/pi-camera` service plus Pi deployment assets.
- Keep the camera node focused on capture and camera state; keep the main host responsible for viewing, metadata, and orchestration.

## Best low-cost camera layout

- Main host handles storage and viewing.
- Pi camera node handles capture and streaming.
- ESP32 stays focused on controls and sensors.

## Pi choice

### Prefer Pi 2 B for webcam use

- More CPU and RAM headroom than a Pi Zero v1.3
- Full-size USB ports
- Easier deployment as a dedicated camera endpoint

### When Pi Zero v1.3 is still acceptable

- You already have the right OTG and Wi-Fi accessories
- You can tolerate lower resolution or frame rate
- Small physical size matters more than convenience

## Network options for older Pis

1. Spare router in client or bridge mode
2. USB Wi-Fi dongle plus OTG
3. USB Ethernet adapter
4. ESP32 serial bridge only as an experiment, not the normal recommendation

## Spare router bridge strategy

If the spare router supports client mode, media bridge, WDS, or repeater bridge:

- join it to the main Wi-Fi
- plug the Pi camera node into the router LAN port
- avoid buying a separate Wi-Fi adapter for the Pi

This is the cheapest clean option when you already own the router.

## ESP32 bridge warning

- It works as a serial networking hack, not as the best practical camera path.
- Throughput is fine for telemetry and commands, but poor for video.
- Complexity is much higher than a simple Wi-Fi dongle or router bridge.

## Streaming software

- `libcamera` for native Pi camera support
- `motion` or `mjpg-streamer` for lightweight streaming

## Related notes

- [[10 Planning/Software and Monorepo Layout]]
- [[20 Hardware/Controller Architecture]]
- [[10 Planning/Budget Shopping Guide]]
