from grow_camera.config import settings


def current_state() -> dict[str, str | None]:
    return {
        "camera_id": settings.camera_id,
        "status": "online",
        "stream_url": settings.stream_url,
        "snapshot_url": settings.snapshot_url,
    }
