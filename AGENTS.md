## Project Scope

- This repo is the grow project software monorepo.
- `docs/vault/` is the curated planning and documentation layer.
- The code and deployment assets live at the repo root:
  - `apps/backend`: FastAPI home-server API, automation, alerts, MQTT bridge, and seeded repository data
  - `apps/web`: React/Vite operator dashboard
  - `firmware/esp32-controller`: PlatformIO firmware for the controller node
  - `nodes/pi-camera`: FastAPI Raspberry Pi camera service
  - `shared/contracts`: protocol source of truth and JSON schemas
  - `shared/fixtures`: canonical payload examples for tests and simulators
  - `deploy/compose/home-server`: Docker Compose deployment entry point
  - `deploy/pi-camera`: Pi deployment notes and env example
  - `deploy/systemd`: service units for Pi or host installs
  - `scripts/sim`: fake controller and fake camera publishers
  - `tests`: integration, contract, firmware layout, and Playwright E2E coverage

## Working Rules

- Preserve the current documentation-specific guidance below when editing `docs/vault/`.
- Prefer updating the real source files over generated artifacts.
- If an important CLI command hangs or stalls and missing networking, sandbox access, or other permissions could plausibly be the cause, default to asking for the needed permissions first instead of spending time on workaround paths. Only chase other explanations first when there is concrete evidence the issue is not permission-related.
- If Git staging, diffing the index, or committing fails with `.git/index.lock` or a read-only filesystem error, rerun the Git command with escalated permissions. In this environment, repository metadata writes under `.git/` may be blocked by the sandbox even when normal file edits are allowed.
- Do not assume `pre-commit` is runnable from the default shell PATH just because a pyenv shim exists. If hook execution matters, verify the executable and config first; a shim-only `pre-commit` failure is separate from sandbox permission issues.
- Avoid editing build output and dependency directories unless the task explicitly requires it:
  - `apps/web/dist`
  - `node_modules`
  - `firmware/esp32-controller/.pio`
  - `*.egg-info`
  - `__pycache__`
- Keep changes aligned with the current architecture:
  - backend remains a single Python service with modules separated by concern
  - MQTT is the machine-to-machine transport boundary
  - protocol details belong in `shared/contracts` before being duplicated elsewhere
  - the Pi camera node stays a first-class v1 component

## Repo Commands

- Bootstrap local dependencies: `scripts/dev/bootstrap.sh`
- Create venv: `just venv`
- Install Python packages: `just install-python`
- Install web packages: `just install-web`
- Run backend: `just backend`
- Run Pi camera service: `just camera`
- Run web app: `just web`
- Run home-server compose stack: `just compose`
- Run simulators: `just sim-controller`, `just sim-camera`
- Python tests: `just test-python` or `.venv/bin/pytest`
- Web tests: `just test-web`
- E2E tests: `just test-e2e`
- Firmware build/test: `just firmware-build`, `just firmware-test`

## Service Notes

- Backend entry point is `apps/backend/src/grow_backend/main.py`.
- Backend API routes currently live in `apps/backend/src/grow_backend/api/routes.py` under `/api/v1`.
- The backend uses seeded in-memory repository data today; preserve that behavior unless the task is explicitly about storage changes.
- Pi camera entry point is `nodes/pi-camera/src/grow_camera/main.py` and currently exposes `/health`, `/state`, and a placeholder `/stream`.
- The web app reads `VITE_API_BASE_URL` and falls back to seeded data if API requests fail; do not break that development path casually.
- Wire contracts and schemas in `shared/contracts` are the cross-service source of truth.

## Testing Expectations

- For backend or shared contract changes, run the relevant Python tests in `tests/`.
- For web changes, run the relevant workspace tests in `apps/web` and add/update Playwright coverage when behavior changes materially.
- For firmware-related tasks, prefer `pio run` / `pio test` against `firmware/esp32-controller`.
- If you change contracts, verify both the schemas and the affected consumers.

## Vault Documentation Rules

- Treat changes under `docs/vault/` as curated documentation work, not isolated file edits.
- If you add, move, rename, merge, split, or materially rewrite any `docs/vault/**/*.md` note, update links so the vault stays navigable.
- Check both directions:
  - outbound links in the edited note
  - inbound links from related notes, indexes, and overview pages
- Update `Related notes` sections where appropriate.
- Update index notes such as `docs/vault/01 Index/Home.md` when a note should be discoverable from the main entry points.
- If a note is moved out of `docs/vault/00 Inbox/`, remove or replace the inbox copy so there is a single canonical version.
- Before finishing, verify that any `[[wikilinks]]` you added or changed resolve to existing notes and that no obvious orphaned docs were created by the edit.
