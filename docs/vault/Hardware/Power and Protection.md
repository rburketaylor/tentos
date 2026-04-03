---
type: reference
status: current
updated: 2026-03-06
region: CA
---

# Power and Protection

## Core rules

- Do not drop voltage with resistors.
- Use buck converters where voltage step-down is required.
- Keep inductive and noisy loads from sharing a fragile logic rail without planning.
- Keep mains switching and wet-area electronics physically separated when possible.
- If you already own an RF outlet, prefer hacking the low-voltage remote over modifying mains hardware.

## Recommended split

- Main host: own normal supply
- Pi node if used: stable 5V supply
- Pump rail: dedicated 12V supply
- ESP32, valves, and small sensors: separate regulated rail sized for the actual load

If the ESP32 switches a pump or valve with a MOSFET, the grounds usually need a common reference unless the control path is fully isolated.

## Fusing

- Fuse the positive lead.
- Place the fuse close to the power source.
- A small pump rail can start with about `1A` fuse sizing.
- A small electronics rail can start with about `2A` fuse sizing.

```text
12V PSU + -> inline fuse -> pump
12V PSU + -> inline fuse -> buck -> 5V electronics rail
```

## Moisture safety

- Mount boards outside the tent when possible.
- If any electronics must live near the grow, use a plastic enclosure with cable glands.
- Mount enclosures high and use drip loops on cables.
- Keep sensor wires away from direct spray and standing runoff.

## Low-voltage heat options

### Cheapest safe-first options

- Seedling heat mat for root-zone heating
- DC silicone heater pad on an aluminum spreader plate
- Driver heat from the light only if the wiring and placement are clean and safe

## Humidity control options

- Cheapest practical option: a mechanical-knob humidifier controlled by switched power
- More DIY option: ultrasonic mist module plus fan

Use hysteresis so the humidifier or heater is not toggled constantly.

## Example threshold logic

```text
If TEMP < 21 C -> heater ON
If TEMP > 26 C -> heater OFF

If RH < 55% -> humidifier ON
If RH > 65% -> humidifier OFF
```

## Related notes

- [[Hardware/Controller Architecture]]
- [[Hardware/RF Outlet Remote Hack]]
- [[Hardware/Irrigation Architecture]]
- [[Growing/Tent Build and Safety]]
