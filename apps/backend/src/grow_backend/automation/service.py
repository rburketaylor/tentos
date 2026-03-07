from pydantic import BaseModel


class AutomationStatus(BaseModel):
    scheduler: str = "idle"
    alerts: str = "armed"
    mode: str = "controller-first"


def get_status() -> AutomationStatus:
    return AutomationStatus()
