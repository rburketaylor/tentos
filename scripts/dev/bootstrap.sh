#!/usr/bin/env bash
set -euo pipefail

python -m venv .venv
.venv/bin/python -m pip install -e '.[dev]' -e apps/backend -e nodes/pi-camera
npm install

echo "Bootstrap complete."
