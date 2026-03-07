---
type: reference
status: current
updated: 2026-03-05
region: CA
---

# Irrigation Architecture

## Current recommendation

Start with the simplest plumbing that meets the real need:

- One reservoir
- One 12V pump
- One timed watering circuit
- Split tubing to each pot

Only add valves, dosing pumps, or moisture-driven logic when you have a concrete reason.

## Stage 1: cheapest reliable layout

```text
Reservoir -> 12V pump -> main line -> T fittings -> pot lines
```

- Control the pump with a MOSFET module.
- Meter water with runtime and emitters if needed.
- Keep tubing short and serviceable.
- Gravity drip is still the absolute cheapest fallback if you want near-zero electronics.
- A timer plus pump remains the best low-complexity automation tier before sensor-driven control.

## Stage 2: when to add complexity

Add per-plant valves only if:

- plants are on different schedules
- you need isolation for testing or maintenance
- a shared line cannot be balanced cleanly

Add nutrient dosing only if:

- manual mixing is becoming the main operational pain
- you need repeatable nutrient injection

## Current valve guidance

- Standard parallel valves remain the easiest to reason about.
- The 3-port switching tree still works, but it is no longer the default recommendation.
- The routing tree saves valve count, not system complexity. Use it only if valve cost or wiring density actually matters more than plumbing clarity.

### 3-port switching valve quick reference

- Port `1` = inlet
- Port `2` = default outlet with no power
- Port `3` = powered outlet
- OFF: `1 -> 2`
- ON: `1 -> 3`

### 4-output routing tree

```text
                 Valve1
              /          \
           Valve2        Valve3
          /     \       /     \
       Plant1  Plant2  Plant3  Plant4
```

| Valve1 | Valve2 | Valve3 | Output |
|---|---|---|---|
| OFF | OFF | - | Plant 1 |
| OFF | ON | - | Plant 2 |
| ON | - | OFF | Plant 3 |
| ON | - | ON | Plant 4 |

Scaling:

- 1 valve -> 2 outputs
- 3 valves -> 4 outputs
- 7 valves -> 8 outputs
- 15 valves -> 16 outputs

## Pump guidance

- Prefer 12V pumps over 5V pumps for irrigation duty.
- Judge pumps by head pressure and restriction tolerance, not just zero-head flow numbers.
- Use submersible pumps for cheap and quiet if the line is simple.
- Use diaphragm pumps if emitters, long runs, or routing valves need more pressure.
- A 12V 4.8W pump only draws about `0.4A`, so a 12V 2A supply is already comfortable.
- Use a reducer if the pump outlet is much larger than the tubing ID.

## Tubing and emitters

- For the previously discussed micro valve barbs, `3 mm ID x 5 mm OD` silicone is the best fit.
- `4 mm ID` tubing is usually too loose on a `3.2-3.6 mm` barb unless clamped.
- Buy about `2-3 m` of tubing per plant; `20 m` is a good starter roll for a small build.
- Drip emitters stabilize flow and help equalize plants.
- Typical emitter ratings are `1 L/h`, `2 L/h`, and `4 L/h`.
- `2 L/h` is about `33 mL/min`, which is enough to show why you do not need pump-scale flow at the dripper.
- Many emitters target `4 mm` tubing, so verify fit or plan adapters if the main system is `3 mm`.

## Sensor guidance

- Soil moisture sensing is optional, not mandatory.
- If used, use capacitive probes and treat them as advisory unless calibrated well in the actual grow medium.
- Time-based watering with observation is still the safest starting point.

## Failure prevention

- Keep nutrient concentrate out of the main reservoir if biofilm becomes a problem.
- Add a short water-only flush after dosing if nutrients are injected inline.
- Fuse the pump supply.
- Keep power conversion and controller boards outside the wet zone.
- Consider an inline filter before the pump if the reservoir is likely to collect debris.

## Valve drive basics

- Never drive a solenoid from GPIO directly.
- Use a logic-level MOSFET per valve or a multi-channel MOSFET board.
- Add a flyback diode across each valve coil.
- Tie controller ground and valve supply ground together unless the channel is fully isolated.
- Avoid IRF520-style boards if the gate drive will be only `3.3V`.

## Watering sequence

1. Select the plant path if valves are in use.
2. Start the pump.
3. Start nutrient dosing if that mode is enabled.
4. Run for the measured time.
5. Stop dosing.
6. Optionally flush with plain water for a few seconds.
7. Stop the pump.

## Related notes

- [[10 Planning/Current Recommended Build]]
- [[10 Planning/Budget Shopping Guide]]
- [[20 Hardware/Controller Architecture]]
- [[20 Hardware/Power and Protection]]
