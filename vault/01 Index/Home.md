---
type: index
status: current
updated: 2026-03-06
region: CA
---

# Home

This vault is the current planning and reference layer for the grow build. The notes below are the ones to follow for decisions, pricing, inventory, and build sequencing.

## Start here

- [[10 Planning/Current Recommended Build]]
- [[10 Planning/Software and Monorepo Layout]]
- [[10 Planning/Budget Shopping Guide]]
- [[20 Hardware/Controller Architecture]]
- [[20 Hardware/Camera and Network Options]]
- [[20 Hardware/Irrigation Architecture]]
- [[20 Hardware/Power and Protection]]
- [[20 Hardware/RF Outlet Remote Hack]]
- [[30 Growing/Environment Targets]]
- [[30 Growing/Odor, Drying, and Storage]]
- [[30 Growing/Tent Build and Safety]]
- [[30 Growing/Practice Crops and Plant Grouping]]
- [[50 Parts/Current Inventory]]
- [[50 Parts/Shopping List]]

## Current defaults

- Use a dedicated `vault/` subdirectory for planning; keep the repo root for code, firmware, and webserver work.
- Use a home server or main application host for automation and monitoring logic.
- Use an ESP32-class board as the hardware controller node.
- For this build, include a Raspberry Pi camera node in v1 because video monitoring is part of the setup.
- Favor cheap commodity parts, but do not keep older recommendations that are now worse on price/function.
