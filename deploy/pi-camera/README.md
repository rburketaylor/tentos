# Pi Camera Deployment

The Pi camera node is intended to run on Raspberry Pi OS Lite and expose a simple local API:

- `/health`
- `/state`
- `/stream`

Start simple. Keep the real camera capture implementation behind the service boundary so you can swap `mjpg-streamer`, `ffmpeg`, or another USB-camera pipeline later.

## Suggested Setup

1. Install Python 3.11+ and create a virtualenv.
2. Install the camera node package with `pip install -e /path/to/repo/nodes/pi-camera`.
3. Copy [`camera.env.example`](/home/burket/Git/tentos/deploy/pi-camera/camera.env.example) to `/etc/grow-pi-camera.env`.
4. Install the systemd unit from [`deploy/systemd/grow-pi-camera.service`](/home/burket/Git/tentos/deploy/systemd/grow-pi-camera.service).
