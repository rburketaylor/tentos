from grow_backend.config import settings


def device_topic(node_id: str, suffix: str) -> str:
    return f"{settings.mqtt_topic_prefix}/{node_id}/{suffix}"


def camera_topic(camera_id: str, suffix: str) -> str:
    return f"{settings.mqtt_topic_prefix}/camera/{camera_id}/{suffix}"
