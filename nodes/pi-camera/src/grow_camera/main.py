import uvicorn
from fastapi import FastAPI

from grow_camera.capture import current_state

app = FastAPI(title="Grow Pi Camera", version="0.1.0")


@app.get("/health")
def healthcheck():
    return {"status": "ok", "service": "pi-camera"}


@app.get("/state")
def state():
    return current_state()


@app.get("/stream")
def stream():
    return {
        "status": "placeholder",
        "message": "Replace with mjpg-streamer, ffmpeg, or a direct camera pipeline on the Pi."
    }


def run() -> None:
    uvicorn.run("grow_camera.main:app", host="0.0.0.0", port=8100, reload=False)


if __name__ == "__main__":
    run()
