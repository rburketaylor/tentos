from grow_backend.storage.repository import repository


def latest_stream_url() -> str:
    cameras = repository.list_cameras()
    return cameras[0].stream_url if cameras else ""
