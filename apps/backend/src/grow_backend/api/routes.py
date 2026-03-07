from fastapi import APIRouter

from grow_backend.automation.service import get_status
from grow_backend.storage.repository import repository

router = APIRouter(prefix="/api/v1")


@router.get("/devices")
def list_devices():
    return repository.list_devices()


@router.get("/schedules")
def list_schedules():
    return repository.list_schedules()


@router.get("/alerts")
def list_alerts():
    return repository.list_alerts()


@router.get("/cameras")
def list_cameras():
    return repository.list_cameras()


@router.get("/media")
def list_media():
    return repository.list_media()


@router.get("/health")
def api_health():
    return {
        "summary": repository.health(),
        "automation": get_status(),
    }
