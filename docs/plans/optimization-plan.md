# 4x4 Tent Build Optimization Plan

## Context

The vault docs describe a 2x2 budget build (~$170-270 CAD). The actual plan has scaled to a **4x4x6.5' tent** with **480W DIY LM301B light**, **3 plants in coco coir**, and a **mixing bucket dosing system**. The docs need significant updates, and every component should be evaluated for maximum DIY value.

---

## 1. DIY 480W LM301B Light Build

### Recommended Build

| Component | Recommendation | Est. Cost (CAD) |
|---|---|---|
| LED Boards | 2x Meijiu 240W QB, LM301B 3000K + 660nm (Alibaba) | $80-110 |
| Driver | Mean Well ELG-480-54A (AliExpress or Digi-Key) | $50-75 |
| Heatsink | Flat aluminum plate 4-5mm, local metal supplier or Princess Auto | $15-30 |
| 730nm far-red strip | OSRAM OSLON strip, separately switched (AliExpress) | $10-15 |
| UV-A 395nm strip | Small strip, separately switched (AliExpress) | $7-12 |
| Wiring + hardware | WAGO connectors, 18AWG wire, thermal paste | $15-20 |
| Ratchet hangers | Amazon or Princess Auto | $10-15 |
| Dimmer | 100K potentiometer | $4-7 |
| **Total** | | **$191-284** |

### Spectrum breakdown
- 3000K LM301B white (70%) + 660nm deep red (20%) + 730nm far red (5%) + 395nm UV (2-3%)
- Run 660nm with the main boards always. Run 730nm for 15-30 min after lights off (Emerson effect, speeds flowering). Run UV-A only in late flower (last 2-4 weeks) for trichome production.
- 3000K is better than 3500K for a flowering-focused build. If doing full-cycle (veg + flower in same tent), 3500K is the compromise.

### Why this over commercial
- Spider Farmer SF4000: ~$450-550 CAD, same LM301B diodes. You pay $200+ for frame, warranty, resale.
- DIY gives: spectrum customization, repairability (swap a board if it dies), exact knowledge of components.
- The ELG driver is $10-15 cheaper than HLG with negligible efficiency loss (~93% vs ~95%).

### Key gotchas
- **Verify diode authenticity**: Ask Meijiu/Kingbrite for close-up photos. Some sellers ship LM281B (cheaper, worse).
- **Counterfeit Mean Wells**: If a Mean Well driver is under $40 for 480W, it's fake. Buy from Digi-Key/Mouser if unsure.
- **Driver outside tent**: Mount the driver where it gets airflow. It's the hottest and most humidity-sensitive component.

### Wiring the boards
- Two 240W boards **in parallel** on the ELG-480-54A (54V output matches each board).
- Each supplemental strip (730nm, UV) on its own small driver or MOSFET-switched from the ESP32.

---

## 2. Tent & Environment (4x4x6.5')

### Tent
- **Mars Hydro 4x4 (48"x48"x80")** from Amazon.ca: ~$120-170 CAD
- VIVOSUN is cheaper ($80-130) but thinner canvas and zipper light leaks. The $20-40 difference is worth it for the Mars Hydro.
- Generic AliExpress tents ($60-100): inconsistent quality, flimsy frames. Skip.

### Ventilation (480W LED in 4x4x6.5')
- Tent volume: 104 cu ft. Target ~300+ CFM after filter restriction.
- **6" inline fan rated 400-450 CFM is the right size.** Not 4", not 8".
- **VIVOSUN Kinematic 6"**: ~$60-90 CAD (~400+ CFM). Best budget value.
- **Inkbird ITC-608T** temp controller: ~$35-45 CAD for automated temp-triggered fan control.
- Total fan + controller: ~$95-135 CAD vs AC Infinity Cloudline T6 at $170-200.

### Carbon filter
- **6" x 18" carbon filter**: ~$30-50 CAD. The 18" length gives more carbon mass (longer life) for $5-10 more than the 12".
- Pair naturally with the 6" fan (no reducers).
- **Delay purchase until early flower** when odor becomes relevant.

### Ducting
- 6" insulated flex duct + clamps: ~$15-25 CAD.
- Passive intake (bottom port with mesh screen) + active exhaust (top-rear).

### Circulation fans
- Two 6" oscillating clip fans: ~$15-20 each, $30-40 total.
- One at canopy level (horizontal airflow), one lower (under-canopy).

### Temperature management
- 480W LED produces ~100-145W of heat. Expect 3-10F above ambient during lights-on.
- **Cooling is the primary concern.** Run lights at night (8PM-8AM) for cooler ambient.
- Supplemental heat only needed if room drops below 16C during lights-off in winter. Cross that bridge when measurements prove it.

### Humidity control
- **DIY ultrasonic fogger**: Ultrasonic disc + float ($8-12) + 12V PSU + PC fan + tubing + Inkbird IHC-200 humidistat ($35-45) = ~$55-80 CAD.
- Simpler alternative: cheap ultrasonic humidifier from Canadian Tire ($25-50) piped into tent intake.

### Environment budget summary

| Item | Est. Cost (CAD) |
|---|---|
| Mars Hydro 4x4 tent | $120-170 |
| 6" inline fan (VIVOSUN) | $60-90 |
| Inkbird temp controller | $35-45 |
| 6x18 carbon filter (buy later) | $30-50 |
| 6" ducting + clamps | $15-25 |
| 2x clip fans | $30-40 |
| DIY humidifier (optional) | $55-80 |
| **Subtotal (basic)** | **$290-470** |

---

## 3. Nutrient Stack Assessment

### Your current stack: verdict on each item

| Item | Keep? | Reason |
|---|---|---|
| Mega Crop Part A (5-12-26) | **Yes** | Solid base PK + micros. Greenleaf Nutrients makes good stuff. |
| Mega Crop Part B (15.5-0-0) | **Yes** | Calcium nitrate. Required complement to Part A. |
| MKP (0-52-34) | **Yes** | Legitimate bloom booster (commercial growers use it). Not overpriced snake oil. Boosts P and K during flower. |
| K2SO4 | **Yes** | Serves a specific purpose: late flower K push without adding more P or N. Keep. |
| CalMag | **Yes** | Mandatory for coco. Even though Part B provides Ca, coco's CEC binds Ca and Mg. Without CalMag you'll get deficiencies. |
| Epsom Salt | **Yes** | $5 for a huge bag. Insurance for late flower when heavy K dosing competes with Mg uptake. May not need it every feeding, but keep it on hand. |
| pH Up/Down | **Yes** | Non-negotiable for coco (5.8-6.2 target). |

### What you do NOT need to add

- **Silica supplement** ($15-25): Nice to have for cell wall strength under 480W, but not critical for v1. Add later if you want.
- **Humic/Fulvic acid**: Minimal benefit in drain-to-waste coco. Skip.
- **Enzymes (Cannazym etc)**: More important for recirculating. Drain-to-waste doesn't accumulate dead roots the same way. Skip.
- **Beneficial microbes**: Nice but not essential. Skip for v1.
- **Any "bud booster" branded product**: Your MKP + K2SO4 already covers what these do at fraction of the cost.

### Mother liquor compatibility (critical for dosing automation)

**Rule: Never mix calcium with phosphorus or sulfur in concentrated form.**

Safe groupings:
- **Tank A**: Mega Crop Part A + MKP + K2SO4 + Epsom Salt (all PKS-based, no Ca conflict)
- **Tank B**: Mega Crop Part B + CalMag (both calcium-based; verify CalMag is nitrate-form, not sulfate-form. If sulfate-form, keep CalMag in its own tank to avoid CaSO4 precipitation)
- **pH Down**: Always separate (acid)

---

## 4. Irrigation & Dosing Automation

### Architecture

```
Water line → 12V solenoid → Mixing bucket
                              ↑↑↑↑↑ (peristaltic pumps from mother liquors)
                              pH probe + EC probe
                              ↓
                         12V submersible pump → split tubing → 3 x 5gal fabric pots
                              (drain-to-waste)
```

### Peristaltic pump channels: 3

| Channel | Doses | Why separate |
|---|---|---|
| 1 | Mega Crop Part A (+ MKP + K2SO4 + Epsom, pre-mixed for current stage) | Has P and S, cannot contact Ca |
| 2 | Mega Crop Part B | Has Ca, cannot contact P or S |
| 3 | pH Down | Acid, always separate |

**CalMag**: Added manually before automated dosing cycle. Infrequent enough that a dedicated pump isn't worth it. Add measured dose to the bucket before the auto-dose sequence runs.

**pH Up**: Rarely needed in this setup (nutrients + tap water trend alkaline). Use manually if needed.

**Stage management**: Pre-mix supplements (MKP, K2SO4, Epsom) into Tank A at ratios appropriate for the current growth stage (veg vs early flower vs late flower). Swap the Tank A bottle when transitioning stages.

### Peristaltic pump options

| Option | Price (CAD) | Notes |
|---|---|---|
| **Generic SJ-25 (AliExpress)** | $5-10 each | Cheapest. DC motor, fixed speed. Need MOSFET + timer for dose volume. Tubing is replaceable. Good enough for this. |
| **Kamoer NKP series (AliExpress)** | $15-25 each | Stepper-driven, more precise dosing. Worth the premium if you want accurate small doses. |
| **Kamoer KCP/KCM series** | $25-40 each | Higher end. Overkill for this application. |

**Recommendation**: 3x SJ-25 or similar DC peristaltic pumps at ~$5-10 each. Control dose volume by running the pump for a calibrated time (ESP32 timed GPIO). Calibrate once by measuring output over 10 seconds, then use that mL/second rate.

For 3 pumps: **$15-30 CAD** total.

### Pump control
- Each peristaltic pump is a DC motor. Switch with logic-level MOSFET from ESP32 GPIO.
- Already in the vault: "MOSFET module for DC pump control" at $3 each. Same approach.
- 3 MOSFET modules: ~$9 total.

### Auto-fill from water line
- **12V DC solenoid valve** (normally closed): ~$5-10 CAD on AliExpress. Match to your water line connection (1/4" or 1/2").
- **Level sensing options**:
  - **Float switch** (simplest): ~$2-5. Magnetic reed float switch. Position at target fill level. ESP32 reads the switch.
  - **Ultrasonic distance sensor** (HC-SR04): ~$3. Mount above bucket, measure distance to water surface. More flexible (can target any fill level via software).
- **Recommendation**: Float switch for simplicity and reliability. $2-5.

### pH/EC monitoring

Running both manual pens and automated sensor modules:
- **Combo TDS/pH pen**: ~$25 CAD. Quick manual checks, calibration reference for the automated sensors.
- **pH sensor module**: ~$25 CAD. ESP32 ADC input. Enables automated pH Down dosing.
- **TDS sensor module**: ~$30 CAD. ESP32 ADC input. Enables EC verification after dosing.
- **DS18B20 temperature probe**: ~$2. Required for temperature compensation on both pH and TDS readings.
- **Total**: ~$82 CAD for both manual + automated monitoring.

Key notes:
- Calibrate automated sensors against the pens regularly (pens are the ground truth).
- pH probe needs periodic calibration with buffer solutions (pH 4.0 and 7.0).
- TDS probe needs KCl calibration solution.
- Both probes will foul over time in nutrient solution. Rinse after each use, store properly.

### Mixing/stirring
- Small 12V submersible pump in the bucket recirculating back into the bucket (or air stone + small air pump). ~$5-10.
- Run for 30-60 seconds after dosing before checking pH/EC.

### Main feed pump
- 12V submersible pump in the mixing bucket: ~$8-15 CAD.
- 3mm or 4mm micro irrigation tubing split to 3 pots with drip emitters.
- Tubing + fittings set: ~$10-25 CAD.

### Drain-to-waste
- 3x saucers or a single large drain tray under the pots. ~$10-20.
- No recirculation. Simpler, no nutrient buildup, no root disease spread.
- **Coco drain-to-waste is the right call** for this setup. The nutrient cost of runoff is minimal compared to the complexity of recirculating.

### Irrigation/dosing budget summary

| Item | Est. Cost (CAD) |
|---|---|
| 3x peristaltic pumps (SJ-25) | $15-30 |
| 3x MOSFET modules | $9 |
| 12V solenoid valve | $5-10 |
| Float switch | $2-5 |
| 12V submersible pump (feed) | $8-15 |
| Mixing pump or air stone | $5-10 |
| Tubing + fittings + drip emitters | $10-25 |
| pH/EC monitoring (pens + sensor modules) | $82 |
| 5-gal fabric pots x3 | $9-15 |
| Drain trays/saucers | $10-20 |
| **Subtotal** | **$155-221** |

---

## 5. Coco Coir Setup

### Source
- Buy unbuffered bricks in bulk on Amazon.ca: ~$1.50-3/brick. 6-8 bricks for three 5-gallon pots: ~$12-24.
- Add perlite (~$15-25 for 100L bag). 70/30 coco/perlite mix.

### Buffering (cannot skip)
1. Hydrate bricks
2. Rinse with pH-adjusted water
3. Soak in CalMag solution (1 tsp/gal) for 4-8 hours
4. Rinse again, adjust to pH 5.5-6.0

Unbuffered coco will lock out Ca and Mg. This is not optional.

### Pot sizing
- **5-gallon fabric pots** are optimal for 3 plants in 4x4 coco. Fits comfortably with working room.
- 7-gallon gets tight in late flower. 3-gallon is too small for the canopy space available.

---

## 6. Power & Controller Setup

### Already owned
- ESP32 dev board (controller)
- Raspberry Pi 2 B (camera node)
- Home server (backend)
- 12V 2A adapters x2
- AHT20 + BMP280 sensors
- Circulation fan

### Power distribution
- **12V rail 1** (pump + solenoid): 12V 2A adapter. Submersible pump + solenoid valve + mixing pump.
- **12V rail 2** (electronics): 12V 2A adapter. Peristaltic pumps and MOSFETs.
- **ESP32**: USB power or buck converter from 12V rail.
- **Light driver**: Mean Well ELG-480-54A runs direct from AC. MOSFET-switched or RF outlet controlled from ESP32.

### ESP32 GPIO allocation (rough)
- 3x GPIO for peristaltic pump MOSFETs (Part A, Part B, pH Down)
- 1x GPIO for submersible feed pump MOSFET
- 1x GPIO for solenoid valve MOSFET
- 1x GPIO for float switch (input)
- 2x GPIO for AHT20 + BMP280 (I2C)
- 1x GPIO for light control (MOSFET or RF remote hack)
- 2x ADC for pH + TDS sensor modules
- 1x GPIO for DS18B20 temp probe (one-wire)
- Optional: 1x GPIO for fan control via Inkbird or direct MOSFET

---

## 7. What's Outdated in the Vault Docs

### Files that need updates

1. **Current Recommended Build** (`docs/vault/Planning/Current Recommended Build.md`)
   - References 2x2 tent → needs 4x4x6.5
   - References "clean water tank + mix tank" → now just mixing bucket with dosing
   - References "$50-90 budget grow light" → now 480W DIY LM301B
   - Sensor section mentions pH/TDS pens "for manual testing and programmatic modules for automation" → refine with actual sensor choices

2. **Budget Shopping Guide** (`docs/vault/Planning/Budget Shopping Guide.md`)
   - Entire price table is for 2x2 build
   - 6" inline fan listed at $120 → VIVOSUN Kinematic is $60-90
   - No mention of peristaltic pumps, solenoid valves, dosing hardware
   - No coco coir, perlite, CalMag costs
   - "Rough build totals" section completely wrong for 4x4

3. **Shopping List** (`docs/vault/Parts/Shopping List.md`)
   - Outdated items and pricing for 2x2 build
   - Needs full rewrite for 4x4 + dosing system

4. **Irrigation Architecture** (`docs/vault/Hardware/Irrigation Architecture.md`)
   - Describes simple pump + split tubing → needs dosing system architecture
   - "Keep a clean water tank" → remove, now using mixing bucket + auto-fill
   - No mention of peristaltic pumps, pH/EC management

5. **Tent Build and Safety** (`docs/vault/Growing/Tent Build and Safety.md`)
   - References 60x60x140cm tent → update for 4x4x6.5
   - Light guidance: "Single honest board cheapest for 2x2 or small 3x3" → update for 480W in 4x4

### Files that are still accurate
- Controller Architecture, Power and Protection, RF Outlet Remote Hack - mostly fine
- Camera and Network Options - fine
- Environment Targets - fine (stage-specific temp/RH values don't change)
- Odor, Drying, and Storage - fine

---

## 8. Total Build Cost Estimate

### Essential (no carbon filter, no humidifier)

| Category | Est. Cost (CAD) |
|---|---|
| Light (DIY 480W) | $191-284 |
| Tent + ventilation | $260-395 |
| Irrigation + dosing + monitoring + pots | $155-221 |
| Coco + perlite + CalMag | $35-55 |
| Nutrients (Mega Crop A+B, MKP, K2SO4, pH Up/Down) | ~$60-100 |
| **Total** | **~$701-1,055** |

### Add-ons

| Item | Est. Cost (CAD) |
|---|---|
| Carbon filter (buy when entering flower) | $30-50 |
| DIY humidifier | $55-80 |
| Pi camera module | $8-20 |

### For comparison
- A complete Mars Hydro/Spider Farmer 4x4 package (tent + light + fan + filter): $800-1,200 CAD. Same or worse components, no dosing automation, no custom controller.

---

## 9. Build Sequence

1. **Light** - Order boards + driver from Alibaba/AliExpress (2-4 week shipping). Start here since shipping is the bottleneck.
2. **Tent + ventilation** - Order tent, fan, ducting, clip fans. Can arrive in days from Amazon.
3. **Coco prep** - Buffer coco coir while waiting for other parts.
4. **Core irrigation** - Submersible pump, tubing, fittings, pots. Hand-mix nutrients initially.
5. **ESP32 controller** - Sensors, MOSFETs, pump switching. Already have the board.
6. **Dosing automation** - 3x peristaltic pumps, solenoid valve, float switch, pH/TDS sensor modules. Add once manual routine is dialed in.
7. **Camera** - Pi camera node. Already have the Pi. Low priority.

---

## 10. Files to Modify

After plan approval, update these vault docs:

1. `docs/vault/Planning/Current Recommended Build.md` - Rewrite for 4x4 build
2. `docs/vault/Planning/Budget Shopping Guide.md` - New price table for 4x4
3. `docs/vault/Parts/Shopping List.md` - Updated shopping list
4. `docs/vault/Hardware/Irrigation Architecture.md` - Add dosing system design
5. `docs/vault/Growing/Tent Build and Safety.md` - Update tent specs

## 11. New File: Coco Grow Guide

Create `docs/vault/Growing/Coco Coir Grow Guide.md` with:

### Content outline
- **Coco coir properties**: CEC, air-to-water ratio, why it needs buffering
- **Buffering procedure**: Step-by-step with CalMag
- **Transplanting into coco**: Seedling → 5-gal pot, wetting the medium
- **Feeding schedule by stage**:
  - Seedling (weeks 1-2): Light feed, ~250-400 ppm EC, pH 5.8-6.0
  - Early veg (weeks 2-4): Increase to 500-700 ppm, pH 5.8-6.0
  - Late veg (weeks 4-6): 700-900 ppm, pH 5.8-6.0
  - Early flower (weeks 1-3): Transition, 800-1000 ppm, pH 5.8-6.2
  - Mid flower (weeks 3-5): Peak feeding, 1000-1200 ppm + MKP, pH 5.8-6.2
  - Late flower (weeks 5-8): Reduced N, high K (K2SO4), 800-1000 ppm, pH 5.8-6.2
  - Flush (last 7-10 days): Plain pH'd water only
- **Feeding frequency**: 1x/day in early veg → 2-3x/day in late flower. Coco needs frequent feeding.
- **Runoff targets**: 10-20% runoff each feeding. Measure runoff EC to verify uptake.
- **Nutrient mixing order**: Always add Part A → mix → add Part B → mix → add CalMag → mix → check pH → adjust. Never mix A and B concentrates directly.
- **Common coco-specific issues**: Ca/Mg lockout, K toxicity symptoms, salt buildup, overwatering in coco (yes it's possible)
- **Mother liquor recipes**: Concentrated stock solutions for each growth stage, with mL-per-gallon dosing ratios for the peristaltic pump calibration
