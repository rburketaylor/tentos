import uvicorn
from fastapi import FastAPI

from grow_backend.api.routes import router as api_router
from grow_backend.config import settings

app = FastAPI(title=settings.api_title, version=settings.api_version)
app.include_router(api_router)


@app.get("/health")
def healthcheck():
    return {"status": "ok", "service": "backend"}


def run() -> None:
    uvicorn.run("grow_backend.main:app", host="0.0.0.0", port=8000, reload=False)


if __name__ == "__main__":
    run()
