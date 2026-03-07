from grow_backend.storage.repository import repository


def current_device_ids() -> list[str]:
    return [device.id for device in repository.list_devices()]
