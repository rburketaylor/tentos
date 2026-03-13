---
type: reference
status: current
updated: 2026-03-06
region: CA
---

# Current Inventory

This note tracks parts already on hand. It is seeded from the hardware explicitly mentioned in the earlier planning notes and should be corrected as you confirm quantities and condition.

## Controllers and compute

| Item | Qty | Status | Planned use | Notes |
|---|---:|---|---|---|
| ESP32 dev board | 1 | owned | Main hardware controller | Existing preferred control node. |
| Raspberry Pi 2 B | 1 | owned | V1 camera node | Better camera host than Pi Zero v1.3 and now part of the initial software stack. |
| Raspberry Pi Zero v1.3 | 1 | owned | Backup / edge experiment | No built-in Wi-Fi. |
| Arduino | 1 | owned | Backup IO / expansion | Optional only. |
| Home server | 1 | owned | Main app, broker, logging, dashboard | Preferred host for monitoring and control software. |

## Camera gear

| Item | Qty | Status | Planned use | Notes |
|---|---:|---|---|---|
| USB camera | 1 | owned | Pi camera node capture device | Planned for the Pi 2 B v1 camera service. |

## Environment and airflow

| Item | Qty | Status | Planned use | Notes |
|---|---:|---|---|---|
| Circulation fan | 1 | owned | Tent airflow | Already covers the circulation-fan requirement. |
| Temp/RH sensor | 1 | owned | Environmental monitoring | Use with the ESP32 and home-server control flow. |

## Networking

| Item | Qty | Status | Planned use | Notes |
|---|---:|---|---|---|
| Spare router | 1 | owned | Pi network bridge | Useful if it supports client or bridge mode. |

## Power

| Item | Qty | Status | Planned use | Notes |
|---|---:|---|---|---|
| 12V 2A adapter | 2+ | assumed owned | Pump rail / electronics rail | Confirm real quantity and connector types. |
| Anker USB-C 100W charger | 1 | owned | Stable 5V supply source | Useful for Pi or other USB-powered gear. |

## Inventory workflow

- When you confirm an item physically, leave it here and update quantity or condition.
- If an owned item is a bad fit, keep it here and mark the shopping-list entry separately.
- If something is only inferred from old notes, change `assumed owned` to either `owned` or `not found`.

## Related notes

- [[50 Parts/Shopping List]]
- [[10 Planning/Budget Shopping Guide]]
- [[20 Hardware/Controller Architecture]]
- [[20 Hardware/Camera and Network Options]]
- [[10 Planning/Software and Monorepo Layout]]
