import os

from pydantic import BaseModel


class CameraSettings(BaseModel):
    camera_id: str = os.getenv("CAMERA_ID", "pi-camera-01")
    stream_url: str = os.getenv("CAMERA_STREAM_URL", "http://127.0.0.1:8100/stream")
    snapshot_url: str | None = os.getenv("CAMERA_SNAPSHOT_URL")


settings = CameraSettings()
