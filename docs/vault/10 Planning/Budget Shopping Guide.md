---
type: reference
status: current
updated: 2026-03-05
region: CA
---

# Budget Shopping Guide

These ranges are Canada-first and meant to stay realistic. Exact marketplace prices move too much to treat as stable specs, so commodity items are listed as low-cost ranges rather than fake-precise totals.

## Current best-value picks

| Item | Current recommendation | Target range (CAD) | Notes |
|---|---|---:|---|
| Tent | 60 x 60 x 140 cm tent or simple DIY enclosure | 55-95 | Buy if convenience matters; DIY if budget matters more. |
| Main controller | ESP32-S2 Mini or basic ESP32 dev board | 8-18 | Use ESP32-S2 Mini when Wi-Fi + GPIO is enough; use a fuller ESP32 board if you need more pins. |
| Temp/RH sensor | AHT20 or AHT21 | 3-6 | Cheaper than BME280 if pressure is not needed. |
| Temp/RH/pressure sensor | BME280 | 6-12 | Keep only if you explicitly want pressure data. |
| Soil moisture | Capacitive v1.2 style sensor | 3-8 | Optional. Add only if it will drive control logic. |
| Pump switching | Logic-level MOSFET module | 3-8 | Prefer over relay for DC pump loads. |
| 12V pump | Small irrigation or submersible pump | 8-20 | Pick head pressure over raw L/h marketing. |
| Tubing + fittings | 3 mm or 4 mm micro irrigation set | 10-25 | Buy extra line once; it is cheap insurance. |
| 3-port valve | Micro 5V switching valve | 8-18 each | Skip entirely if one schedule fits all pots. |
| Inline fan | 4 inch fan | 25-45 | Carbon filter is separate. |
| Carbon filter | 4 inch | 30-50 | Buy only if odor control matters. |
| Circulation fan | Clip fan | 10-20 | Required. |
| Ducting + clamps | 4 inch kit | 10-18 | Easy to under-buy; include it in the real total. |
| Pots | 5-gallon fabric pots | 8-15 | Buckets or nursery pots also work if already on hand. |
| Timer | Mechanical outlet timer | 6-12 | Cheap and still useful even with automation later. |
| Basic monitor | LCD hygrometer | 4-8 | Good fallback even if the main controller is down. |
| Budget grow light | Honest 100W full-spectrum LED board | 50-90 | Avoid fake wattage and old blurple fixtures. |
| Camera module | Pi Camera v1/v2 or cheap USB webcam | 8-20 | Only buy if video monitoring matters. |
| Electronics enclosure | Plastic IP-rated enclosure | 8-15 | Worth it if any board must be near the grow. |
| Optional Pi camera node | Raspberry Pi Zero 2 W | 21-25 official MSRP | Pi is now a role-specific add-on, not the default controller. |

## How to use this note

- Treat this page as the price and recommendation reference.
- Track what you already own in [[50 Parts/Current Inventory]].
- Track the remaining gaps in [[50 Parts/Shopping List]].
- When an owned item satisfies a recommendation, mark it there instead of deleting it from this guide.

## Explicit recommendation changes

- Old default: `BME280 everywhere`
- Current default: `AHT20/AHT21` unless pressure is required

- Old default: `Pi Zero 2 W as the automation brain near the tent`
- Current default: `main host + ESP32`, with Pi reserved for camera or local Linux-only tasks

- Old default: `relay module for pump control`
- Current default: `MOSFET module for DC pump control`

- Old default: `per-plant valves by default`
- Current default: `shared timed irrigation first, valves only when schedules diverge`

- Old default: `carbon filter treated as standard`
- Current default: `buy it only if odor control is actually required`

## Buying priorities

1. Spend first on light quality and airflow reliability.
2. Spend second on irrigation parts that match your actual plumbing layout.
3. Spend last on “smart” extras like cameras, pressure sensors, and independent valve routing.

## Rough build totals

### Cheapest workable 2x2 setup

- DIY enclosure path: about `170-230 CAD` before optional camera and odor control
- Bought tent path: about `190-270 CAD` before optional camera and odor control

### Add-ons that move the total

- Carbon filter: add `30-50 CAD`
- Camera hardware: add `8-25 CAD` depending on what you already own
- Valve-based independent irrigation: add `20-70 CAD+` depending on channel count

These ranges replace the older single total because the real cost swings mostly with enclosure choice, odor control, and whether irrigation is shared or independently routed.

## Price notes

- Raspberry Pi Zero 2 W official pricing remains around USD 15, which is roughly low-20s CAD depending on exchange and retailer markup: <https://www.raspberrypi.com/products/raspberry-pi-zero-2-w/>
- Example current Canada listings used to reset older assumptions:
  - AHT20 breakout at CAD 2.99: <https://universal-solder.ca/product/temperature-humidity-sensor-aht20/>
  - BME280 breakout at CAD 5.99: <https://universal-solder.ca/product/bme280-temperature-humidity-pressure-sensor/>
  - Capacitive moisture sensor at CAD 3.99: <https://universal-solder.ca/product/capacitive-soil-moisture-sensor-v1-2/>
  - ESP32-S2 Mini at CAD 7.84: <https://universal-solder.ca/product/lolin-s2-mini/>
  - ESP32-C6 dev board at CAD 17.90: <https://universal-solder.ca/product/waveshare-esp32-c6-dev-board/>

## Related notes

- [[10 Planning/Current Recommended Build]]
- [[20 Hardware/Controller Architecture]]
- [[20 Hardware/Camera and Network Options]]
- [[20 Hardware/Irrigation Architecture]]
- [[20 Hardware/Power and Protection]]
- [[50 Parts/Current Inventory]]
- [[50 Parts/Shopping List]]
- [[30 Growing/Tent Build and Safety]]
