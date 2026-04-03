---
type: index
status: current
updated: 2026-03-06
region: CA
---

# Home

This vault is the current planning and reference layer for the grow build. The notes below are the ones to follow for decisions, pricing, inventory, and build sequencing.

## Start here

- [[Planning/Current Recommended Build]]
- [[Planning/Software and Monorepo Layout]]
- [[Planning/Budget Shopping Guide]]
- [[Hardware/Controller Architecture]]
- [[Hardware/Camera and Network Options]]
- [[Hardware/Irrigation Architecture]]
- [[Hardware/Power and Protection]]
- [[Hardware/RF Outlet Remote Hack]]
- [[Growing/Coco Coir Grow Guide]]
- [[Growing/Environment Targets]]
- [[Growing/Odor, Drying, and Storage]]
- [[Growing/Tent Build and Safety]]
- [[Growing/Practice Crops and Plant Grouping]]
- [[Parts/Current Inventory]]
- [[Parts/Shopping List]]

## Implementation plan

- [Implementation Roadmap](../../plans/implementation-roadmap.md) — Phased roadmap for completing all unimplemented features

## Current defaults

- Use a dedicated `docs/vault/` subdirectory for planning; keep the repo root for code, firmware, and webserver work.
- Use a home server or main application host for automation and monitoring logic.
- Use an ESP32-class board as the hardware controller node.
- For this build, include a Raspberry Pi camera node in v1 because video monitoring is part of the setup.
- Favor cheap commodity parts, but do not keep older recommendations that are now worse on price/function.
