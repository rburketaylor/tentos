from pydantic import BaseModel


class Settings(BaseModel):
    api_title: str = "Grow Control API"
    api_version: str = "0.1.0"
    mqtt_topic_prefix: str = "grow"


settings = Settings()
