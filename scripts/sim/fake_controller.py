from __future__ import annotations

import json
from pathlib import Path


def main() -> None:
    fixture = Path("shared/fixtures/device-telemetry.json")
    payload = json.loads(fixture.read_text())
    print("Simulated controller telemetry")
    print(json.dumps(payload, indent=2))


if __name__ == "__main__":
    main()
