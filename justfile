set shell := ["bash", "-cu"]

venv:
    python -m venv .venv

install-python:
    .venv/bin/python -m pip install -e '.[dev]' -e apps/backend -e nodes/pi-camera

install-web:
    npm install

backend:
    .venv/bin/uvicorn grow_backend.main:app --reload --port 8000

camera:
    .venv/bin/uvicorn grow_camera.main:app --reload --port 8100

web:
    npm --workspace apps/web run dev

compose:
    docker compose --env-file deploy/compose/home-server/.env.example -f deploy/compose/home-server/docker-compose.yml up --build

sim-controller:
    python scripts/sim/fake_controller.py

sim-camera:
    python scripts/sim/fake_camera.py

test-python:
    .venv/bin/pytest

test-web:
    npm --workspace apps/web run test

test-e2e:
    npm run test:e2e

firmware-build:
    pio run -d firmware/esp32-controller

firmware-test:
    pio test -d firmware/esp32-controller -e native

firmware-compiledb:
    pio run -t compiledb -d firmware/esp32-controller
