from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]


def test_platformio_project_exists():
    assert (ROOT / "firmware/esp32-controller/platformio.ini").exists()
    assert (ROOT / "firmware/esp32-controller/src/main.cpp").exists()
