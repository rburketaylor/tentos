from __future__ import annotations

from grow_backend.models import (
    Alert,
    CameraState,
    DeviceState,
    HealthSummary,
    MediaRecord,
    Schedule,
)


class InMemoryRepository:
    def __init__(self) -> None:
        self._devices = [
            DeviceState(
                id="tent-controller-01",
                name="Tent Controller",
                status="online",
                temperature_c=24.3,
                humidity_rh=59.1,
                moisture_pct=42.0,
                active_schedule_id="feed-veg-am",
            )
        ]
        self._schedules = [
            Schedule(
                id="feed-veg-am",
                device_id="tent-controller-01",
                name="Morning Feed",
                enabled=True,
                start_time="08:00",
                duration_seconds=18,
                description="Primary irrigation pulse for vegetative schedule.",
            )
        ]
        self._alerts = [
            Alert(
                id="camera-signal-lag",
                source="pi-camera-01",
                level="warning",
                message="Camera node reported delayed snapshot upload.",
            )
        ]
        self._cameras = [
            CameraState(
                id="pi-camera-01",
                name="Tent Canopy Cam",
                status="online",
                stream_url="http://pi-camera.local:8100/stream",
                last_snapshot_url="http://pi-camera.local:8100/snapshots/latest.jpg",
            )
        ]
        self._media = [
            MediaRecord(
                id="media-001",
                camera_id="pi-camera-01",
                kind="snapshot",
                url="http://pi-camera.local:8100/snapshots/latest.jpg",
            )
        ]

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
