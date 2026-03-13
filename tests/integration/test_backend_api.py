import json
from pathlib import Path

from fastapi.testclient import TestClient
from jsonschema import validate

from grow_backend.main import app

ROOT = Path(__file__).resolve().parents[2]
client = TestClient(app)


def load_json(path: str):
    return json.loads((ROOT / path).read_text())


def test_healthcheck():
    response = client.get("/health")

    assert response.status_code == 200
    assert response.json()["service"] == "backend"
    assert response.json()["repository_mode"] == "seeded"


def test_resource_endpoints_return_seeded_data_and_match_contracts():
    responses = {
        "devices": client.get("/api/v1/devices"),
        "schedules": client.get("/api/v1/schedules"),
        "alerts": client.get("/api/v1/alerts"),
        "cameras": client.get("/api/v1/cameras"),
        "media": client.get("/api/v1/media"),
    }

    for resource, response in responses.items():
        assert response.status_code == 200
        validate(
            instance=response.json(),
            schema=load_json(f"shared/contracts/{resource}.response.schema.json"),
        )

    assert responses["devices"].json()[0]["id"] == "tent-controller-01"
    assert responses["cameras"].json()[0]["id"] == "pi-camera-01"
    assert responses["alerts"].json()[0]["level"] == "warning"
