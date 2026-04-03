---
type: reference
status: current
updated: 2026-04-03
region: CA
---

# Irrigation Architecture

## Current recommendation

Mixing bucket with automated dosing, auto-fill from water line, and drain-to-waste feeding to 3 plants in coco coir.

## Architecture

```text
Water line --> 12V solenoid valve --> Mixing bucket
                                          |  |  |  (3 peristaltic pumps from mother liquors)
                                          |  |  |   Tank A (Mega Crop A + supplements)
                                          |  |  |   Tank B (Mega Crop B)
                                          |  |  |   pH Down
                                          |
                                     pH probe + TDS probe + DS18B20
                                     Mixing pump (stir after dosing)
                                     Float switch (level sensing)
                                          |
                                     12V submersible pump
                                          |
                                     Main line --> T fittings --> 3 x drip emitters --> pots
                                                                          (drain-to-waste)
```

## Dosing system

### Peristaltic pumps: 3 channels

| Channel | Contents | Why separate |
|---|---|---|
| 1 | Mega Crop Part A + MKP + K2SO4 + Epsom Salt (pre-mixed per stage) | Has P and S, cannot contact Ca |
| 2 | Mega Crop Part B (15.5-0-0) | Has Ca, cannot contact P or S |
| 3 | pH Down (phosphoric acid or nitric acid) | Acid, always separate |

**CalMag**: Added manually to the bucket before the auto-dose cycle. Infrequent enough that a 4th pump channel is not justified.

### Mother liquor rules

**Never mix calcium with phosphorus or sulfur in concentrated form.**

- **Tank A** (safe to combine): Mega Crop Part A + MKP + K2SO4 + Epsom Salt. All are PKS-based with no calcium.
- **Tank B** (standalone): Mega Crop Part B (calcium nitrate). Do not mix anything into Tank B unless you verify it contains no P or S.
- **pH Down**: Always its own container.

### Stage management

Pre-mix supplements into Tank A at ratios for the current growth stage:

- **Veg**: Part A only (no MKP, no K2SO4, light Epsom)
- **Early flower**: Part A + MKP (moderate P and K boost)
- **Late flower**: Part A + MKP + K2SO4 (heavy K push, reduced P)

Swap the Tank A bottle when transitioning stages. The pump and calibration stay the same.

### Pump selection

- **SJ-25 DC peristaltic pumps**: ~$5-10 CAD each on AliExpress. Fixed speed, MOSFET-switched.
- Control dose volume by running the pump for a calibrated time (ESP32 timed GPIO).
- Calibrate once: measure output over 10 seconds, derive mL/second rate.

## Auto-fill from water line

- **12V DC solenoid valve** (normally closed): Opens to fill bucket to target level.
- **Float switch**: Magnetic reed switch at target water level. ESP32 reads the switch to know when to close the solenoid.
- Alternative: ultrasonic distance sensor (HC-SR04) above the bucket for software-defined fill levels. More flexible but more complex.

## pH/EC monitoring

- **pH sensor module**: ESP32 ADC input. Enables automated pH Down dosing.
- **TDS sensor module**: ESP32 ADC input. Verifies nutrient concentration after dosing.
- **DS18B20**: Temperature compensation for both pH and TDS readings.
- **Combo pH/TDS pen**: Manual backup and calibration reference. Calibrate automated sensors against the pen regularly.

## Automated dosing cycle

1. Open solenoid valve, fill bucket to target level (float switch triggers close).
2. Add CalMag manually (if needed for this feeding).
3. Dose Tank A (peristaltic pump 1 runs for calibrated time).
4. Dose Tank B (peristaltic pump 2 runs for calibrated time).
5. Run mixing pump for 30-60 seconds.
6. Check pH (sensor module). If above target, dose pH Down (peristaltic pump 3).
7. Run mixing pump again, re-check pH.
8. Check TDS (sensor module). Verify EC is in target range.
9. Run feed pump to deliver to plants through split tubing.
10. Stop when target volume delivered (timed or float-triggered).

## Feed delivery

- 12V submersible pump in the mixing bucket feeds through main line.
- Split via T fittings to 3 drip emitters, one per 5-gallon fabric pot.
- Target 10-20% runoff each feeding (drain-to-waste).
- No recirculation. Simpler, no nutrient buildup, no root disease spread.

## Drain-to-waste rationale

- Coco drain-to-waste is the right call for this setup.
- Nutrient cost of runoff is minimal compared to the complexity of recirculating.
- Recirculating coco requires more pH/EC management, sterilization, and monitoring.
- If a recirculating system is wanted later, it requires a collection tray, return pump, and much more aggressive pH/EC control logic.

## Stage 1 fallback: simplest layout

If dosing is not ready yet, start with hand-mixing:

```text
Mixing bucket (hand-mix nutrients) -> 12V pump -> main line -> T fittings -> pot lines
```

- Control the pump with a MOSFET module.
- Meter water with runtime and emitter flow rates.
- This is the operational baseline. Dosing automation layers on top.

## Pump guidance

- Prefer 12V pumps over 5V pumps for irrigation duty.
- Judge pumps by head pressure and restriction tolerance, not just zero-head flow numbers.
- Use submersible pumps for cheap and quiet if the line is simple.
- Use diaphragm pumps if emitters, long runs, or routing valves need more pressure.
- A 12V 4.8W pump only draws about `0.4A`, so a 12V 2A supply is already comfortable.
- Use a reducer if the pump outlet is much larger than the tubing ID.

## Tubing and emitters

- For micro valve barbs, `3 mm ID x 5 mm OD` silicone is the best fit.
- `4 mm ID` tubing is usually too loose on a `3.2-3.6 mm` barb unless clamped.
- Buy about `2-3 m` of tubing per plant; `20 m` is a good starter roll.
- Drip emitters stabilize flow and help equalize plants.
- Typical emitter ratings are `1 L/h`, `2 L/h`, and `4 L/h`.
- `2 L/h` is about `33 mL/min`.
- Many emitters target `4 mm` tubing, so verify fit or plan adapters.

## Sensor guidance

- Soil moisture sensing is optional, not mandatory.
- If used, use capacitive probes and treat them as advisory unless calibrated in the actual medium.
- Time-based watering with observation is still the safest starting point.

## Failure prevention

- Keep mother liquors in separate sealed containers away from light and heat.
- Add a short water-only flush after dosing if nutrients are injected inline.
- Fuse the pump supply.
- Keep power conversion and controller boards outside the wet zone.
- Consider an inline filter before the feed pump if the bucket collects debris.
- If the ESP32 loses connection to the home server, it should still be able to execute a local watering schedule (fallback plan).

## Valve drive basics

- Never drive a solenoid from GPIO directly.
- Use a logic-level MOSFET per valve or a multi-channel MOSFET board.
- Add a flyback diode across each valve coil.
- Tie controller ground and valve supply ground together unless the channel is fully isolated.
- Avoid IRF520-style boards if the gate drive will be only `3.3V`.

## Related notes

- [[Planning/Current Recommended Build]]
- [[Planning/Budget Shopping Guide]]
- [[Hardware/Controller Architecture]]
- [[Hardware/Power and Protection]]
- [[Growing/Coco Coir Grow Guide]]
