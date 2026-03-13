import os
from typing import Literal

from pydantic import BaseModel


class Settings(BaseModel):
    api_title: str = os.getenv("GROW_API_TITLE", "Grow Control API")
    api_version: str = os.getenv("GROW_API_VERSION", "0.1.0")
    mqtt_topic_prefix: str = os.getenv("GROW_MQTT_TOPIC_PREFIX", "grow")
    mqtt_broker_url: str | None = os.getenv("MQTT_BROKER_URL")
    database_url: str | None = os.getenv("DATABASE_URL")
    repository_mode: Literal["seeded"] = os.getenv("GROW_REPOSITORY_MODE", "seeded")


settings = Settings()
