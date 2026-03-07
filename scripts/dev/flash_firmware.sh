#!/usr/bin/env bash
set -euo pipefail

pio run -d firmware/esp32-controller -t upload
