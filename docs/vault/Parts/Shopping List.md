---
type: reference
status: current
updated: 2026-04-03
region: CA
---

# Shopping List

This note tracks gaps, not every possible part. The recommendation reference stays in [[Planning/Budget Shopping Guide]], and the owned hardware lives in [[Parts/Current Inventory]].

## Already covered by inventory

| Item | Covered by | Notes |
|---|---|---|
| Main controller | ESP32 dev board | Existing preferred controller node. |
| Main application host | Home server | Use for dashboards, logging, and control app. |
| Camera host | Raspberry Pi 2 B | Already chosen for the v1 camera node. |
| USB camera | Existing USB camera | Covers the current camera-device requirement for the Pi node. |
| Circulation fan | Existing circulation fan | Already covers one fan. A second clip fan is on the buy list. |
| Temp/RH sensor | Existing temp/RH sensor | Already covers the first environmental-monitoring requirement. |
| Backup edge hardware | Raspberry Pi Zero v1.3 / Arduino | Keep as fallback, not default. |
| Basic power supplies | 12V adapters and USB-C charger | Confirm connectors and real quantities before buying duplicates. |
| Router bridge option | Spare router | Only useful if it supports client or bridge mode. |

## Need soon (priority order)

### Light (order first, 2-4 week shipping)

| Item | Qty | Priority | Target price | Notes |
|---|---|---|---|---|
| LM301B quantum boards (240W, 3000K + 660nm) | 2 | High | 80-110 CAD | Meijiu or Kingbrite on Alibaba. Verify genuine Samsung diodes. |
| Mean Well ELG-480-54A driver | 1 | High | 50-75 CAD | Digi-Key for guaranteed genuine, AliExpress for cheapest. |
| Aluminum plate 4-5mm (heatsink) | 1 | High | 15-30 CAD | Local metal supplier or Princess Auto. |
| 730nm far-red LED strip | 1 | Medium | 10-15 CAD | AliExpress. Separately switched. |
| 395nm UV-A LED strip | 1 | Medium | 7-12 CAD | AliExpress. Separately switched. |
| Ratchet hangers | 1 set | Medium | 10-15 CAD | |
| Wiring, WAGO connectors, thermal paste | 1 set | Medium | 15-20 CAD | |
| 100K potentiometer (dimmer) | 1 | Low | 4-7 CAD | |

### Tent and ventilation (order from Amazon, arrives in days)

| Item | Qty | Priority | Target price | Notes |
|---|---|---|---|---|
| Mars Hydro 4x4 tent (48x48x80 in) | 1 | High | 120-170 CAD | Amazon.ca. |
| VIVOSUN Kinematic 6 inch inline fan | 1 | High | 60-90 CAD | ~400+ CFM. Amazon.ca. |
| Inkbird ITC-608T temp controller | 1 | High | 35-45 CAD | Automated fan control. |
| 6 inch ducting + clamps | 1 set | High | 15-25 CAD | Insulated flex duct. |
| 6 inch oscillating clip fan | 1 | High | 15-20 CAD | Second circulation fan. |

### Irrigation and dosing

| Item | Qty | Priority | Target price | Notes |
|---|---|---|---|---|
| SJ-25 peristaltic pump | 3 | High | 15-30 CAD total | Part A, Part B, pH Down. AliExpress. |
| Logic-level MOSFET module | 6 | High | 18 CAD total | 3 for peristaltic pumps, 1 for feed pump, 1 for solenoid, 1 spare. |
| 12V DC solenoid valve (normally closed) | 1 | Medium | 5-10 CAD | Auto-fill from water line. Match fitting to line. |
| Float switch | 1 | Medium | 2-5 CAD | Level sensing for mixing bucket. |
| 12V submersible pump (feed) | 1 | High | 8-15 CAD | Feeds plants from mixing bucket. |
| 12V submersible pump (mixing) or air stone | 1 | Medium | 5-10 CAD | Stir bucket after dosing. |
| 3-4mm micro irrigation tubing + fittings + drip emitters | 1 set | High | 10-25 CAD | Split to 3 pots. |

### Monitoring

| Item | Qty | Priority | Target price | Notes |
|---|---|---|---|---|
| Combo pH/TDS pen | 1 | High | 25 CAD | Manual backup and calibration reference. |
| pH sensor module | 1 | Medium | 25 CAD | ESP32 ADC. Enables automated dosing. |
| TDS sensor module | 1 | Medium | 30 CAD | ESP32 ADC. EC verification. |
| DS18B20 temperature probe | 1 | Medium | 2 CAD | Temp compensation for pH/TDS readings. |

### Medium and containers

| Item | Qty | Priority | Target price | Notes |
|---|---|---|---|---|
| Coco coir bricks (unbuffered) | 6-8 | High | 12-24 CAD | Amazon.ca. ~$1.50-3/brick. |
| Perlite (100L bag) | 1 | High | 15-25 CAD | 70/30 coco/perlite mix. |
| 5-gallon fabric pots | 3 | High | 9-15 CAD | Optimal for 3 plants in 4x4 coco. |
| Drain trays or saucers | 3 | Medium | 10-20 CAD | Drain-to-waste. |
| CalMag (for buffering + supplementation) | 1 | High | 15-25 CAD | Mandatory for coco. |

## Conditional buys

| Item | Qty | Trigger | Target price | Notes |
|---|---|---|---|---|
| Carbon filter 6x18 inch | 1 | Buy when entering flower | 30-50 CAD | Skip until odor matters. |
| DIY humidifier parts | 1 set | Buy if RH is too low | 55-80 CAD | Ultrasonic disc + float + humidistat. |
| Soil moisture sensor | 1-2 | Buy only if feedback-driven watering | 3-8 CAD each | Optional at the start. |
| Pi camera module | 1 | Buy when camera node is ready | 8-20 CAD | Low priority. |

## Decision checks before ordering

- Confirm Alibaba order for LED boards: message Meijiu/Kingbrite for quote including shipping to Canada.
- Verify driver voltage matches boards (54V for parallel wiring of two 240W boards).
- Confirm whether odor control actually matters enough to justify a carbon filter.
- Confirm the water line fitting size before ordering the solenoid valve.
- Verify CalMag is nitrate-form before considering merging it with Part B in a shared mother liquor.

## Related notes

- [[Parts/Current Inventory]]
- [[Planning/Budget Shopping Guide]]
- [[Planning/Current Recommended Build]]
- [[Hardware/Irrigation Architecture]]
- [[Growing/Coco Coir Grow Guide]]
