---
type: reference
status: current
updated: 2026-04-03
region: CA
---

# Budget Shopping Guide

These ranges are Canada-first and meant to stay realistic. Exact marketplace prices move too much to treat as stable specs, so commodity items are listed as low-cost ranges rather than fake-precise totals.

## Current best-value picks

### Light (DIY 480W LM301B)

| Item | Current recommendation | Target range (CAD) | Notes |
|---|---|---|---|
| LED boards | 2x Meijiu 240W QB, LM301B 3000K + 660nm | 80-110 | Order from Alibaba. Verify genuine Samsung diodes. |
| Driver | Mean Well ELG-480-54A | 50-75 | AliExpress or Digi-Key. Avoid counterfeits (under $40 is fake). |
| Heatsink | Flat aluminum plate 4-5mm | 15-30 | Local metal supplier or Princess Auto. Passive cooling is sufficient. |
| 730nm far-red strip | OSRAM OSLON strip, separately switched | 10-15 | Run 15-30 min after lights off. AliExpress. |
| UV-A 395nm strip | Small strip, separately switched | 7-12 | Run only in late flower. AliExpress. |
| Wiring + hardware | WAGO connectors, 18AWG, thermal paste | 15-20 | |
| Ratchet hangers | | 10-15 | Amazon or Princess Auto. |
| Dimmer | 100K potentiometer | 4-7 | |

### Tent and environment (4x4x6.5 ft)

| Item | Current recommendation | Target range (CAD) | Notes |
|---|---|---|---|
| Tent | Mars Hydro 4x4 (48x48x80 in) | 120-170 | Better canvas than VIVOSUN. Amazon.ca. |
| Inline fan | VIVOSUN Kinematic 6 inch (~400+ CFM) | 60-90 | Best budget value. Amazon.ca. |
| Temp controller | Inkbird ITC-608T | 35-45 | Automated temp-triggered fan speed. |
| Carbon filter | 6x18 inch (buy when entering flower) | 30-50 | Delay purchase until odor matters. |
| Ducting + clamps | 6 inch insulated flex duct kit | 15-25 | |
| Circulation fans | 2x 6 inch oscillating clip fans | 30-40 | One at canopy, one under-canopy. |
| LCD hygrometer | Basic monitor | 4 | Fallback if controller is down. |

### Irrigation and dosing

| Item | Current recommendation | Target range (CAD) | Notes |
|---|---|---|---|
| Peristaltic pumps | 3x SJ-25 DC motor (AliExpress) | 15-30 | Part A, Part B, pH Down. CalMag manual. |
| MOSFET modules | 3x logic-level | 9 | Same as pump switching approach. |
| 12V solenoid valve | Normally closed, match water line | 5-10 | Auto-fill from water line. AliExpress. |
| Float switch | Magnetic reed | 2-5 | Level sensing for auto-fill. |
| 12V submersible pump | Feed pump for mixing bucket | 8-15 | |
| Mixing pump | Small submersible or air stone | 5-10 | Stir the bucket after dosing. |
| Tubing + fittings | 3-4mm micro irrigation set | 10-25 | Split to 3 pots with drip emitters. |
| 5-gal fabric pots | 3x | 9-15 | Optimal for 3 plants in 4x4 coco. |
| Drain trays | Saucers or single drain tray | 10-20 | Drain-to-waste. |

### Monitoring

| Item | Current recommendation | Target range (CAD) | Notes |
|---|---|---|---|
| Combo pH/TDS pen | Manual backup and calibration reference | 25 | Ground truth for automated sensors. |
| pH sensor module | ESP32 ADC input | 25 | Enables automated pH Down dosing. |
| TDS sensor module | ESP32 ADC input | 30 | Enables EC verification after dosing. |
| DS18B20 | Temperature probe | 2 | Temp compensation for pH and TDS. |
| Temp/RH sensor | AHT20 and BMP280 | N/A (OWN) | |
| Soil moisture | Capacitive v1.2 | 3 | Optional. Add only if it drives control logic. |
| Camera module | Pi Camera or USB webcam | 8-20 | Low priority. |

### Coco coir and medium

| Item | Current recommendation | Target range (CAD) | Notes |
|---|---|---|---|
| Coco coir bricks | Unbuffered, 6-8 bricks | 12-24 | Amazon.ca. ~$1.50-3/brick. |
| Perlite | 100L bag | 15-25 | 70/30 coco/perlite mix. |
| CalMag | For buffering and supplementation | 15-25 | Mandatory for coco. Also used in nutrient regimen. |

### Nutrients

| Item | Target range (CAD) | Notes |
|---|---|---|
| Mega Crop 2-Part A (5-12-26) | Included in stack | Base PK + micros. |
| Mega Crop 2-Part B (15.5-0-0) | Included in stack | Calcium nitrate. |
| MKP (0-52-34) | Included in stack | Bloom booster. |
| K2SO4 | Included in stack | Late flower K push. |
| Epsom Salt | ~5 | Huge bag. Mg insurance for heavy K. |
| pH Up | Included in stack | Rarely needed. Manual only. |
| pH Down | Included in stack | Essential. Automated via peristaltic pump. |

## Explicit recommendation changes

- Old default: `relay module for pump control`
- Current default: `MOSFET module for DC pump control`

- Old default: `per-plant valves by default`
- Current default: `shared timed irrigation first, valves only when schedules diverge`

- Old default: `carbon filter treated as standard`
- Current default: `buy it only if odor control is actually required`

- Old default: `2x2 tent with $50-90 budget grow light`
- Current default: `4x4x6.5 tent with DIY 480W LM301B light`

- Old default: `clean water tank + mix tank`
- Current default: `single mixing bucket with auto-fill and dosing`

- Old default: `4+ peristaltic pump channels`
- Current default: `3 pumps (Part A, Part B, pH Down), CalMag manual`

- Old default: `HLG driver series`
- Current default: `ELG driver series (similar performance, $10-15 less)`

## Buying priorities

1. Spend first on light quality and airflow reliability.
2. Spend second on irrigation parts that match your actual plumbing layout.
3. Spend last on "smart" extras like cameras, pressure sensors, and independent valve routing.

## Rough build totals

### Core 4x4 setup

- Light (DIY 480W): `191-284 CAD`
- Tent + ventilation: `260-395 CAD`
- Irrigation + dosing + monitoring + pots: `155-221 CAD`
- Coco + perlite + CalMag: `35-55 CAD`
- Nutrients (Mega Crop stack): `~60-100 CAD`
- **Total: ~701-1,055 CAD**

### Add-ons that move the total

- Carbon filter: add `30-50 CAD` (buy when entering flower)
- DIY humidifier: add `55-80 CAD`
- Camera hardware: add `8-25 CAD` depending on what you already own

### For comparison

- A complete Mars Hydro or Spider Farmer 4x4 package (tent + light + fan + filter): `800-1,200 CAD`. Same or worse components, no dosing automation, no custom controller.
- Spider Farmer SF4000 alone: `450-550 CAD`. Same LM301B diodes as the DIY build at ~$200+ more.

## Price notes

- Example current Canada listings used to reset older assumptions:
  - AHT20 breakout at CAD 2.99: <https://universal-solder.ca/product/temperature-humidity-sensor-aht20/>
  - BME280 breakout at CAD 5.99: <https://universal-solder.ca/product/bme280-temperature-humidity-pressure-sensor/>
  - Capacitive moisture sensor at CAD 3.99: <https://universal-solder.ca/product/capacitive-soil-moisture-sensor-v1-2/>
  - ESP32-S2 Mini at CAD 7.84: <https://universal-solder.ca/product/lolin-s2-mini/>
  - ESP32-C6 dev board at CAD 17.90: <https://universal-solder.ca/product/waveshare-esp32-c6-dev-board/>

## Related notes

- [[Planning/Current Recommended Build]]
- [[Hardware/Controller Architecture]]
- [[Hardware/Camera and Network Options]]
- [[Hardware/Irrigation Architecture]]
- [[Hardware/Power and Protection]]
- [[Parts/Current Inventory]]
- [[Parts/Shopping List]]
- [[Growing/Tent Build and Safety]]
- [[Growing/Coco Coir Grow Guide]]
