---
type: plan
status: current
updated: 2026-04-03
region: CA
---

# Current Recommended Build

## Decision summary

- Controller stack: application/webserver on the main host, ESP32-class board for pumps/valves/sensors, and a Pi 2 B camera node in v1 because video monitoring is part of this build.
- Timing and scheduling: use the ESP32 plus home-server stack rather than buying a separate timer.
- Tent: 4x4x6.5 ft (48 x 48 x 80 in) bought tent, Mars Hydro preferred over VIVOSUN for canvas quality.
- Tent control: 6 inch inline fan (~400-450 CFM) plus circulation fan, carbon filter only when odor matters (delay purchase until early flower).
- Light: DIY 480W LM301B build with 3000K white + 660nm deep red + 730nm far red + 395nm UV-A.
- Irrigation: mixing bucket with auto-fill from water line, 3 peristaltic pumps for nutrient dosing (Part A, Part B, pH Down), 12V submersible pump to feed plants, drain-to-waste in coco coir.
- Sensors: AHT20/BMP280 for temp/RH/pressure, combo pH/TDS pen for manual checks, pH and TDS sensor modules for ESP32 automation, DS18B20 for temperature compensation.

## Best current path

### 1. Brains vs device control

- Keep the higher-level app, dashboard, logging, and rules engine off the tent hardware.
- Use an ESP32 board for direct IO because it is cheaper and cleaner than dedicating a Pi to relay and sensor duty.
- In this build, use the Pi 2 B as the dedicated camera node and keep it off controller duty.
- Let the home server own the schedule source of truth and let the ESP32 execute the timing locally with the option for a local fallback plan to make sure the watering doesn't just stop if the home server goes offline; do not plan around a separate outlet timer.

### 2. Environment control

- Ventilation first, humidification second, heating only if the room actually runs cold at lights-off.
- Start with timer-free environmental control logic driven by temp/RH thresholds and hysteresis.
- If odor is not a real problem, skip the carbon filter and preserve the budget for better light or more reliable irrigation parts.
- 480W LED produces ~100-145W of heat. Cooling is the primary concern. Run lights at night for cooler ambient.
- Expect 3-10F above ambient during lights-on with a 6 inch exhaust running.

### 3. Light

- DIY 480W LM301B build for ~$191-284 CAD vs $450-550 for a commercial equivalent (Spider Farmer SF4000).
- Two 240W quantum boards (Meijiu or Kingbrite from Alibaba) with LM301B 3000K + 660nm on a Mean Well ELG-480-54A driver.
- Supplemental 730nm far-red strip (run 15-30 min after lights off for Emerson effect) and 395nm UV-A strip (run only in late flower for trichome production).
- Heatsink: flat aluminum plate 4-5mm from local metal supplier. Passive cooling is sufficient for quantum boards.
- Driver mounted outside the tent for airflow and humidity protection.

### 4. Irrigation and dosing

- Single mixing bucket with auto-fill from water line (12V solenoid + float switch).
- 3 peristaltic pumps (SJ-25 style) for automated dosing: Part A tank, Part B tank, pH Down.
- CalMag added manually to the bucket before the auto-dose cycle (infrequent enough to not warrant a dedicated pump).
- Pre-mix supplements (MKP, K2SO4, Epsom Salt) into the Part A mother liquor at ratios for the current growth stage. Swap the Part A bottle when transitioning between veg and flower.
- pH and TDS sensor modules on ESP32 ADC for automated monitoring. Combo pen as manual backup and calibration reference.
- 12V submersible pump feeds from mixing bucket through split tubing to 3 x 5-gallon fabric pots with drip emitters.
- Drain-to-waste: no recirculation. 10-20% runoff each feeding.

### 5. Price reductions with no useful loss

- Replace Pi-as-controller designs with ESP32-as-controller unless camera or local Linux services are required.
- Replace multi-valve routing trees with simple split tubing if the plants will be watered together.
- Replace commercial grow lights with DIY quantum board builds for the same diodes at 40-55% less cost.
- Use ELG driver series over HLG for ~$10-15 savings with negligible efficiency loss.
- CalMag manual dosing instead of a 4th peristaltic pump channel.

## What to build first

1. Light: order boards + driver from Alibaba/AliExpress first (2-4 week shipping is the bottleneck).
2. Tent and ventilation: tent, inline fan, ducting, clip fans. Arrives in days from Amazon.
3. Coco prep: buffer coco coir while waiting for other parts.
4. Core irrigation: submersible pump, tubing, fittings, pots. Hand-mix nutrients initially.
5. ESP32 controller: sensors, MOSFETs, pump switching. Already have the board.
6. Dosing automation: 3 peristaltic pumps, solenoid valve, float switch, pH/TDS sensor modules.
7. Camera: Pi camera node. Already have the Pi. Low priority.

## Related notes

- [[Planning/Budget Shopping Guide]]
- [[Planning/Software and Monorepo Layout]]
- [[Hardware/Controller Architecture]]
- [[Hardware/Camera and Network Options]]
- [[Hardware/Irrigation Architecture]]
- [[Growing/Environment Targets]]
- [[Growing/Coco Coir Grow Guide]]
