# METERING_CORE Visual Clearance Audit

This audit captures visual failure cases that must be checked before a
schematic snapshot is considered ready for PCB/artwork.

## New Review Rules Added

The following rules were added to
`docs/automation/schematic-style-guide.md` so Gemini and Codex review the same
failure modes:

- IC top/bottom power-pin text must remain outside the IC body.
- Power and ground port text must not overlap pin names, pin numbers, rails, or
  symbol bodies.
- Connector and module labels must start outside the symbol body.
- Floating pin numbers with no visible pin stroke or nearby symbol boundary are
  visual-review failures.
- Power and ground port graphics must not pierce IC or module bodies.
- Long island notes and connector titles must keep frame clearance.
- Bottom-edge IC ground pins must drop vertically to downward ground ports; do
  not use side-facing DGND/GNDA net-label bundles between IC pins.
- Power ports must sit at the end of their wire or rail, not on top of the
  middle of a conductor.
- Terminal-block pins must protrude outward from the symbol body so wires start
  outside the box.
- Latching relay coil-common wiring must use its own clear lane and must not
  pass through the ON/OFF driver graphics.

## Current Failure Examples To Check

1. MCU lower power pins
   - DGND/GNDA/VSS-style text must connect below the pins and remain outside the
     `U_MCU` body.
   - Pin text must not be drawn inside the yellow MCU body.

2. MCU upper power rail
   - `+3.3V` must be moved above or to the uncluttered end of the rail.
   - The power label must not overlap VDD/VDDA/VDD_USB/VLCD pin text or pin
     numbers.

3. PLC SPI header near the sheet frame
   - `J_PLC_SPI` reference and header note must stay inside the frame and away
     from relay/load labels.
   - Any floating pin number or orphaned header mark must be removed or tied to
     a visible pin.

4. PLC module differential pins
   - `PLC_RX_N`, `PLC_RX_P`, `PLC_TX_N`, and `PLC_TX_P` must not be buried
     inside the `U_PLC` body. If these are external nets, place labels outside
     the right side of the module.

5. RS-485 isolation island
   - Ground/power symbols must not pierce the isolator body.
   - `+3.3V` text must be moved to a rail end or above the rail so it does not
     overlap wires or pin numbers.

6. Power supply island output capacitors
   - Capacitor references/values must not overlap each other.
   - `+3.3V` and output capacitor labels must be separated from the horizontal
     rail and from each other.

7. CT/ZCT terminal blocks
   - `J_CT1` and `J_ZCT1` pin strokes must be visible outside the connector
     body before the sensor wires begin.
   - Pin numbers and N/P labels must remain readable and not look like they
     are embedded deep inside the terminal block.

8. Latching relay drive island
   - `K_RLY1` pin 2 (`COIL_COM`) must not share or cross the `RLY_ON` transistor
     lane when a lower routing lane is available.
   - ON/OFF flyback diode text and coil labels must not touch the relay pin
     wires.

## Required Verification Before PCB

- Regenerate a color PDF after every schematic visual cleanup.
- Render PNG crops for each changed hotspot.
- Run KiCad ERC and keep the report in this folder.
- Ask Gemini to review this audit together with the color PDF/PNG artifacts.
