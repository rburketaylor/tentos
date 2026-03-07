import os

from pydantic import BaseModel


class CameraSettings(BaseModel):
    camera_id: str = os.getenv("CAMERA_ID", "pi-camera-01")
    stream_url: str = os.getenv("CAMERA_STREAM_URL", "http://pi-camera.local:8100/stream")
    snapshot_url: str = os.getenv(
        "CAMERA_SNAPSHOT_URL",
        "http://pi-camera.local:8100/snapshots/latest.jpg",
    )


settings = CameraSettings()
