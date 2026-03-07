from __future__ import annotations

from datetime import datetime, timezone
from typing import Literal

from pydantic import BaseModel, Field


def utc_now() -> datetime:
    return datetime.now(timezone.utc)


class DeviceState(BaseModel):
    id: str
    name: str
    status: Literal["online", "offline", "degraded"]
    temperature_c: float
    humidity_rh: float
    moisture_pct: float | None = None
    active_schedule_id: str | None = None
    last_seen: datetime = Field(default_factory=utc_now)


class Schedule(BaseModel):
    id: str
    device_id: str
    name: str
    enabled: bool = True
    start_time: str
    duration_seconds: int
    description: str


class Alert(BaseModel):
    id: str
    source: str
    level: Literal["info", "warning", "critical"]
    message: str
    active: bool = True
    created_at: datetime = Field(default_factory=utc_now)


class CameraState(BaseModel):
    id: str
    name: str
    status: Literal["online", "offline", "degraded"]
    stream_url: str
    last_snapshot_url: str | None = None
    last_seen: datetime = Field(default_factory=utc_now)


class MediaRecord(BaseModel):
    id: str
    camera_id: str
    kind: Literal["snapshot", "stream"]
    url: str
    created_at: datetime = Field(default_factory=utc_now)


class HealthSummary(BaseModel):
    backend: Literal["online"] = "online"
    devices_online: int
    active_alerts: int
    cameras_online: int
