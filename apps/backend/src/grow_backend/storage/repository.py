from __future__ import annotations

import json
from pathlib import Path

from grow_backend.models import (
    Alert,
    CameraState,
    DeviceState,
    HealthSummary,
    MediaRecord,
    Schedule,
)


def _repo_root() -> Path:
    for parent in Path(__file__).resolve().parents:
        if (parent / "shared" / "fixtures").is_dir():
            return parent

    raise RuntimeError("Unable to locate shared fixtures directory from backend repository")


def _load_fixture_list(path: Path, model):
    payload = json.loads(path.read_text())
    return [model.model_validate(item) for item in payload]


class InMemoryRepository:
    def __init__(self) -> None:
        fixtures_dir = _repo_root() / "shared" / "fixtures"
        self._devices = _load_fixture_list(fixtures_dir / "devices.json", DeviceState)
        self._schedules = _load_fixture_list(fixtures_dir / "schedules.json", Schedule)
        self._alerts = _load_fixture_list(fixtures_dir / "alerts.json", Alert)
        self._cameras = _load_fixture_list(fixtures_dir / "cameras.json", CameraState)
        self._media = _load_fixture_list(fixtures_dir / "media.json", MediaRecord)

    def list_devices(self) -> list[DeviceState]:
        return self._devices

    def list_schedules(self) -> list[Schedule]:
        return self._schedules

    def list_alerts(self) -> list[Alert]:
        return self._alerts

    def list_cameras(self) -> list[CameraState]:
        return self._cameras

    def list_media(self) -> list[MediaRecord]:
        return self._media

    def health(self) -> HealthSummary:
        return HealthSummary(
            devices_online=sum(device.status == "online" for device in self._devices),
            active_alerts=sum(alert.active for alert in self._alerts),
            cameras_online=sum(camera.status == "online" for camera in self._cameras),
        )


repository = InMemoryRepository()
