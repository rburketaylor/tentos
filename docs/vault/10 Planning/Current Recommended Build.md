---
type: plan
status: current
updated: 2026-03-06
region: CA
---

# Current Recommended Build

## Decision summary

- Controller stack: application/webserver on the main host, ESP32-class board for pumps/valves/sensors, and a Pi 2 B camera node in v1 because video monitoring is part of this build.
- Timing and scheduling: use the ESP32 plus home-server stack rather than buying a separate timer.
- Tent control: cheap inline fan plus circulation fan, carbon filter only if odor matters.
- Irrigation: 12V pump plus MOSFET switching, with optional valves only if you need per-plant routing.
- Sensors: AHT20/AHT21 for temp/RH when pressure is not needed; capacitive moisture sensors only if they will actually drive decisions.

## Best current path

### 1. Brains vs device control

- Keep the higher-level app, dashboard, logging, and rules engine off the tent hardware.
- Use an ESP32 board for direct IO because it is cheaper and cleaner than dedicating a Pi to relay and sensor duty.
- In this build, use the Pi 2 B as the dedicated camera node and keep it off controller duty.
- Let the home server own the schedule source of truth and let the ESP32 execute the timing locally; do not plan around a separate outlet timer.

### 2. Environment control

- Ventilation first, humidification second, heating only if the room actually runs cold at lights-off.
- Start with timer-free environmental control logic driven by temp/RH thresholds and hysteresis.
- If odor is not a real problem, skip the carbon filter and preserve the budget for better light or more reliable irrigation parts.

### 3. Irrigation

- Use a single 12V pump and timed watering if all pots can share a schedule.
- Add branch valves only when plants need truly independent watering or nutrient routing.
- Use a MOSFET module for pump switching instead of a relay for DC loads unless you already own the relay.
- Keep the water reservoir plain and dose nutrients separately only if you outgrow manual mixing.

### 4. Price reductions with no useful loss

- Replace BME280 with AHT20/AHT21 when you only need temperature and humidity.
- Replace Pi-as-controller designs with ESP32-as-controller unless camera or local Linux services are required.
- Replace “smart irrigation” complexity with timed irrigation first; add soil feedback later if overwatering becomes a real problem.
- Replace multi-valve routing trees with simple split tubing if the plants will be watered together.

## What to build first

1. Core grow hardware: light, airflow, containers, and the already-owned circulation fan.
2. Simple irrigation: reservoir, 12V pump, tubing, MOSFET switching.
3. Sensor and control node: the already-owned temp/RH sensor plus optional soil moisture.
4. Monitoring stack: logging, dashboard, alerts, and the Pi camera node.

## Related notes

- [[10 Planning/Budget Shopping Guide]]
- [[10 Planning/Software and Monorepo Layout]]
- [[20 Hardware/Controller Architecture]]
- [[20 Hardware/Camera and Network Options]]
- [[20 Hardware/Irrigation Architecture]]
- [[30 Growing/Environment Targets]]
