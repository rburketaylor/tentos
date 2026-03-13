from pathlib import Path
import re
import subprocess

ROOT = Path(__file__).resolve().parents[2]


def test_platformio_project_exists():
    assert (ROOT / "firmware/esp32-controller/platformio.ini").exists()
    assert (ROOT / "firmware/esp32-controller/src/main.cpp").exists()


def test_local_controller_config_is_ignored_and_untracked():
    relative_path = "firmware/esp32-controller/include/controller_config.local.h"
    gitignore = (ROOT / ".gitignore").read_text()

    assert relative_path in gitignore

    result = subprocess.run(
        ["git", "ls-files", "--error-unmatch", relative_path],
        cwd=ROOT,
        capture_output=True,
        check=False,
        text=True,
    )
    assert result.returncode != 0


def test_controller_config_header_prefers_local_override():
    config_header = (ROOT / "firmware/esp32-controller/include/controller_config.h").read_text()

    assert '__has_include("controller_config.local.h")' in config_header
    assert '#include "controller_config.example.h"' in config_header


def test_firmware_topics_match_documented_protocol_shape():
    protocol = (ROOT / "shared/contracts/protocol.yaml").read_text()
    header = (ROOT / "firmware/esp32-controller/include/grow_protocol.h").read_text()
    node_id = re.search(r'kNodeId = "([^"]+)"', header)

    assert node_id is not None
    concrete_node_id = node_id.group(1)

    for suffix in ("telemetry", "state", "command", "availability"):
        assert f"grow/{{node_id}}/{suffix}" in protocol
        assert f"grow/{concrete_node_id}/{suffix}" in header
