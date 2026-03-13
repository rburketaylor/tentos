# Code Review Issues and Follow-Up Plan

Updated: March 13, 2026

This document is the current summary of code review findings after the recent hardening pass across deployment, backend, web, contracts, and firmware configuration.

It is not a dump of every possible improvement. It is a planning document for the issues that are still real, why they matter, and the order they should be fixed in.

## Current State

The repo is in a better state than it was before this pass:

- The tracked firmware local secret file was removed from the normal repo state.
- The Compose stack no longer pretends PostgreSQL and MQTT are active backend dependencies.
- Seeded backend API data now comes from shared fixtures instead of duplicated hardcoded values.
- The dashboard no longer silently hides API failures by swapping in fake healthy data.
- Shared HTTP response schemas now exist for the main backend collection endpoints.

That means several earlier review findings are no longer active issues. The remaining work is mostly about production access control, backend resilience, and a few fragile architecture/tooling shortcuts.

## Already Addressed or No Longer Active

These items do not need to stay at the top of the plan unless the architecture changes again.

### Completed

- Hardcoded credentials in tracked firmware local config
  - The repo no longer relies on a tracked `controller_config.local.h` with real values.
  - The ignored local-override pattern is now enforced by tests.

- Dead PostgreSQL Compose wiring
  - The current Compose stack no longer runs Postgres or passes fake persistence wiring to the backend.
  - This removed a misleading “looks production-ready” path that was not real.

- Silent fallback behavior in the web app
  - The dashboard now reports API failure explicitly instead of masking it with seed data.
  - Seeded mode is now an explicit opt-in behavior.

### Inactive for the current stack

- Anonymous MQTT access in Compose
  - This was real in the previous Compose layout.
  - It is not part of the current active Compose path because the broker is no longer launched there.
  - If MQTT is reintroduced into Compose, auth and broker hardening should be treated as required work.

- Hardcoded `.local` camera URLs as the default app path
  - The main seed/deploy path now uses safer local defaults and no longer advertises a fake snapshot route as a normal default.
  - Camera URL strategy still needs a better long-term model if multiple deployment targets are expected.

## Remaining Issues

## 1. No API Authentication

**Files**
- [apps/backend/src/grow_backend/api/routes.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/api/routes.py)
- [deploy/compose/home-server/nginx.conf](/home/burket/Git/tentos/deploy/compose/home-server/nginx.conf)

**Why it matters**

The backend API is still open. That is acceptable for local development, but it is not acceptable for any real deployment where the web tier or API is reachable by anything outside a fully trusted network.

**What needs to be fixed**

- Choose an authentication model for operator access.
- Apply that model consistently at the API boundary.
- Decide whether service-to-service access uses the same mechanism or a separate one.
- Document which routes, if any, are intentionally public.

**Priority**

Highest production priority.

## 2. Backend Routes Still Have Minimal Failure Handling

**File**
- [apps/backend/src/grow_backend/api/routes.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/api/routes.py)

**Why it matters**

The routes mostly hand back repository data directly. If the implementation grows beyond the current seeded repository, failures will become raw 500s with little structure, and operational debugging will get messy fast.

**What needs to be fixed**

- Define a consistent error-handling strategy for API routes.
- Add explicit mapping from internal failures to stable HTTP error responses.
- Add tests for expected failure paths, not only happy paths.

**Priority**

High.

## 3. Backend Architecture Still Uses Global Singletons and Implicit Selection

**Files**
- [apps/backend/src/grow_backend/storage/repository.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/storage/repository.py)
- [apps/backend/src/grow_backend/camera/service.py](/home/burket/Git/tentos/apps/backend/src/grow_backend/camera/service.py)

**Why it matters**

The backend still depends on a module-level `repository` instance, and the camera service still assumes the first camera is the primary one. That is manageable today, but it will become a friction point when persistence, multiple cameras, or richer service wiring arrive.

**What needs to be fixed**

- Move repository access behind FastAPI dependency injection or an equivalent service layer.
- Remove “first element is primary” behavior and replace it with explicit selection or explicit caller intent.
- Keep seeded mode available, but make it one backend implementation rather than the global default shape of the code.

**Priority**

High.

## 4. Dependency Policy Is Too Narrow for Normal Maintenance

**File**
- [apps/backend/pyproject.toml](/home/burket/Git/tentos/apps/backend/pyproject.toml)

**Why it matters**

`fastapi>=0.135,<0.136` is narrower than necessary. That creates unnecessary upgrade friction and can block straightforward bug-fix adoption.

**What needs to be fixed**

- Widen the acceptable version range to a maintained minor window.
- Do the same review for other tightly constrained dependencies.
- Keep the lockfile and tests as the real guardrail, not overly narrow package constraints.

**Priority**

Medium-high.

## 5. Simulator Scripts Still Depend on Running from Repo Root

**Files**
- [scripts/sim/fake_controller.py](/home/burket/Git/tentos/scripts/sim/fake_controller.py)
- [scripts/sim/fake_camera.py](/home/burket/Git/tentos/scripts/sim/fake_camera.py)

**Why it matters**

These scripts still use relative paths like `Path("shared/fixtures/...")`. That works only when run from the repository root. It is a small issue, but it is exactly the kind of tooling fragility that wastes time later.

**What needs to be fixed**

- Resolve paths from the script location instead of the current working directory.
- Add a small test or smoke check if these scripts are expected to remain part of the normal workflow.

**Priority**

Medium.

## 6. Web Environment Documentation Is Better, but Still Incomplete

**Files**
- [README.md](/home/burket/Git/tentos/README.md)
- [apps/web](/home/burket/Git/tentos/apps/web)

**Why it matters**

The current docs now explain seeded mode, but there is still no web-specific `.env.example` documenting the frontend environment surface in one obvious place.

**What needs to be fixed**

- Add an `apps/web/.env.example` with `VITE_API_BASE_URL` and `VITE_USE_SEEDED_DATA`.
- Keep the README short and let the env example be the canonical reference.

**Priority**

Medium.

## 7. Bootstrap Script Uses `python` Instead of `python3`

**File**
- [scripts/dev/bootstrap.sh](/home/burket/Git/tentos/scripts/dev/bootstrap.sh)

**Why it matters**

This is a small portability issue. It will not matter on many modern systems, but when it does matter it fails early and confusingly.

**What needs to be fixed**

- Switch the bootstrap script to `python3`.
- Optionally check for Python 3.11+ explicitly and fail with a clear message.

**Priority**

Medium-low.

## 8. Firmware Reliability and Cleanup Items

**Files**
- [firmware/esp32-controller/src/main.cpp](/home/burket/Git/tentos/firmware/esp32-controller/src/main.cpp)

**Why it matters**

The remaining firmware concerns are mostly robustness and maintainability issues rather than immediate repo hygiene problems. They are real, but they are behind the backend and access-control work in urgency.

**What needs to be fixed**

- Evaluate whether a watchdog should be enabled for the ESP32 runtime.
- Replace obvious magic numbers such as `4095` with named constants.
- Review the relay pin initialization path for any transient unsafe state risk during startup.

**Priority**

Medium-low.

## Suggested Fix Order

1. Add API authentication and define the deployment trust boundary.
2. Add backend error-handling policy and failure-path tests.
3. Refactor backend singleton/implicit-selection patterns before adding more runtime complexity.
4. Widen dependency ranges and clean up simulator/bootstrap tooling.
5. Finish the smaller docs and firmware cleanup items.

## Notes for Future Reviews

- Do not re-add the old Postgres/MQTT Compose findings unless those services are brought back into the active deployment path.
- Do not treat seeded mode itself as a bug. The problem was implicit and misleading fallback behavior, which has already been corrected.
- Prefer tracking issues that are both real in the current repo state and likely to affect the next implementation step.
