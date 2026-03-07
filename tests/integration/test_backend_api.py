from fastapi.testclient import TestClient
from grow_backend.main import app

client = TestClient(app)


def test_healthcheck():
    response = client.get("/health")

    assert response.status_code == 200
    assert response.json()["service"] == "backend"


def test_resource_endpoints_return_seeded_data():
    devices = client.get("/api/v1/devices")
    cameras = client.get("/api/v1/cameras")
    alerts = client.get("/api/v1/alerts")

    assert devices.status_code == 200
    assert devices.json()[0]["id"] == "tent-controller-01"
    assert cameras.json()[0]["id"] == "pi-camera-01"
    assert alerts.json()[0]["level"] == "warning"
