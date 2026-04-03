---
type: reference
status: current
updated: 2026-03-06
region: CA
---

# RF Outlet Remote Hack

## Current recommendation

If you already own an RF-controlled outlet and remote, the cleanest automation path is usually to hack the handheld remote instead of modifying the outlet or building a mains relay box.

```text
ESP32 GPIO -> button-press interface -> remote PCB button pads
remote transmits RF -> outlet toggles power -> grow light
```

This keeps the automation work on the low-voltage side and avoids mains-side modification.

## Why this is the preferred version

- No outlet teardown or mains rewiring
- Reuses hardware already on hand
- Easy to test in stages
- Small and cheap final build

Tradeoffs:

- The outlet uses a toggle command, so controller state can drift if the physical remote is also used
- The remote PCB still needs to be opened and soldered
- Reliability still depends on the RF link

## How the remote is used

The remote button is normally just a momentary switch between two electrical nodes:

```text
pad A --- switch --- pad B
```

The goal is to let the ESP32 briefly short those two nodes through an interface device, which simulates a button press.

## Practical implementation

1. Open the remote.
2. Identify the two switch sides with a multimeter.
3. Solder one thin wire to each side of the button.
4. Connect those wires to a button-press interface driven by the ESP32.
5. Pulse that interface for about `100-200 ms`.

Only one pad from each switch side is needed.

## Interface options

### Best default: small transistor

Use a small NPN transistor if you have one available.

Examples:

- `2N2222`
- `PN2222`
- `2N3904`
- `BC547`
- `S8050`

### Good isolation option: optocoupler

Use an optocoupler if you want the control path isolated.

Examples:

- `PC817`
- `EL817`

### Acceptable fallback: tiny relay module

Using a relay here is acceptable because it is only simulating a button press, not switching mains.

```text
remote pad A -> relay COM
remote pad B -> relay NO
ESP32 -> relay VCC / GND / IN
```

This is bulkier and noisier than a transistor or optocoupler, but it is still fine for a light that only changes state occasionally.

## Powering the remote

The remote will usually work from its original coin cell, which is the safest first test path.

If you want a cleaner permanent install, you can test powering it from the ESP32 `3.3V` rail:

```text
ESP32 3.3V -> remote battery positive
ESP32 GND  -> remote battery negative
```

Notes:

- Confirm polarity before connecting anything
- `3.3V` is slightly above a nominal `3.0V` coin cell, so test stability before treating it as permanent
- If needed later, add a diode drop or proper regulator instead of guessing

## Test sequence

### 1. Verify the switch pads

Use continuity mode to confirm:

- two legs on one side of the tact switch are common
- two legs on the other side are common
- the two sides connect only when pressed

### 2. Manual short test

With the remote powered, briefly short pad A and pad B.

If that toggles the outlet, the hack is valid.

### 3. Interface test

Trigger the relay, transistor, or optocoupler from the ESP32 and verify that it produces the same outlet toggle.

### 4. Power test

Compare remote behavior on:

- original coin cell
- ESP32 `3.3V` rail

## Software note

Because the outlet toggles rather than exposing true on/off state, the controller should assume a startup state and avoid manual use of the handheld remote unless you also have a way to resynchronize state.

For a basic grow light schedule, that limitation is usually acceptable.

## Rejected options

These were discussed and rejected for this build:

- switching mains directly with a tiny relay board
- building a separate mains relay box
- modifying the outlet itself
- flashing cheap smart plugs
- replacing the whole setup with new smart-home hardware first

## Related notes

- [[Planning/Current Recommended Build]]
- [[Hardware/Controller Architecture]]
- [[Hardware/Power and Protection]]
- [[Growing/Tent Build and Safety]]
