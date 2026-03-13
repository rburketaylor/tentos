---
type: reference
status: current
updated: 2026-03-06
region: CA
---

# Shopping List

This note tracks gaps, not every possible part. The recommendation reference stays in [[10 Planning/Budget Shopping Guide]], and the owned hardware lives in [[50 Parts/Current Inventory]].

## Already covered by inventory

| Item                  | Covered by                       | Notes                                                            |
| --------------------- | -------------------------------- | ---------------------------------------------------------------- |
| Main controller       | ESP32 dev board                  | Existing preferred controller node.                              |
| Main application host | Home server                      | Use for dashboards, logging, and control app.                    |
| Camera host           | Raspberry Pi 2 B                 | Already chosen for the v1 camera node.                           |
| USB camera            | Existing USB camera              | Covers the current camera-device requirement for the Pi node.    |
| Circulation fan       | Existing circulation fan         | Already covers the baseline airflow requirement inside the tent. |
| Temp/RH sensor        | Existing temp/RH sensor          | Already covers the first environmental-monitoring requirement.   |
| Backup edge hardware  | Raspberry Pi Zero v1.3 / Arduino | Keep as fallback, not default.                                   |
| Basic power supplies  | 12V adapters and USB-C charger   | Confirm connectors and real quantities before buying duplicates. |
| Router bridge option  | Spare router                     | Only useful if it supports client or bridge mode.                |

## Need soon

| Item | Qty | Priority | Target price | Why it is still needed |
|---|---:|---|---:|---|
| Tent or DIY enclosure materials | 1 | High | 55-95 CAD | Physical grow space is still missing. |
| Budget grow light | 1 | High | 50-90 CAD | Highest-impact missing part. |
| Inline fan | 1 | High | 25-45 CAD | Exhaust and environmental stability. |
| MOSFET module | 1 | High | 3-8 CAD | Required for DC pump control. |
| 12V pump | 1 | High | 8-20 CAD | Required for irrigation. |
| Tubing + fittings | 1 set | High | 10-25 CAD | Required to distribute water. |
| Pots or containers | 1 set | Medium | 8-15 CAD | Required unless already sourced locally. |

## Conditional buys

| Item | Qty | Trigger | Target price | Notes |
|---|---:|---|---:|---|
| Carbon filter | 1 | Buy only if odor matters | 30-50 CAD | Skip if smell control is unnecessary. |
| Ducting + clamps | 1 set | Needed with inline fan layout | 10-18 CAD | Do not forget to budget this with the fan. |
| Soil moisture sensor | 1-2 | Buy only if you want feedback-driven watering | 3-8 CAD each | Optional at the start. |
| 3-port valves | 2-4 | Buy only if plants need independent routing | 8-18 CAD each | Shared schedule is cheaper and simpler. |
| Electronics enclosure | 1 | Buy only if boards must sit near the grow | 8-15 CAD | Safer than exposing boards to humidity. |
| LCD hygrometer | 1 | Buy only if you want a no-code fallback display | 4-8 CAD | Useful but not essential. |

## Decision checks before ordering

- Confirm whether the spare router really supports client or bridge mode.
- Confirm whether odor control actually matters enough to justify a carbon filter.
- Confirm whether the enclosure will be bought or built.
- Confirm whether one shared watering schedule is enough before buying valves.
- Keep timing and scheduling in the ESP32 plus home-server stack; do not add a standalone timer to the shopping path.

## Related notes

- [[50 Parts/Current Inventory]]
- [[10 Planning/Budget Shopping Guide]]
- [[10 Planning/Current Recommended Build]]
- [[20 Hardware/Camera and Network Options]]
