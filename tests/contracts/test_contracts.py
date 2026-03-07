from __future__ import annotations

import json
from pathlib import Path

from jsonschema import validate

ROOT = Path(__file__).resolve().parents[2]


def load_json(path: str) -> dict:
    return json.loads((ROOT / path).read_text())


def test_fixtures_validate_against_schemas():
    pairs = [
        ("shared/contracts/device-telemetry.schema.json", "shared/fixtures/device-telemetry.json"),
        ("shared/contracts/device-command.schema.json", "shared/fixtures/device-command.json"),
        ("shared/contracts/camera-state.schema.json", "shared/fixtures/camera-state.json"),
        ("shared/contracts/media-record.schema.json", "shared/fixtures/media-record.json"),
        ("shared/contracts/alert.schema.json", "shared/fixtures/alert.json"),
    ]

    for schema_path, fixture_path in pairs:
        validate(instance=load_json(fixture_path), schema=load_json(schema_path))


def test_protocol_declares_camera_resources():
    protocol = (ROOT / "shared/contracts/protocol.yaml").read_text()

    assert "cameras" in protocol
    assert "grow/camera/{camera_id}/state" in protocol
