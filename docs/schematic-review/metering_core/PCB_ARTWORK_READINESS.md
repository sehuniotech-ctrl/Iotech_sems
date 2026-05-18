# PCB / Artwork Readiness

This note records the transition status from the `METERING_CORE` schematic
review snapshot to PCB artwork.

## Current Board Target

- Board size target: 40 mm x 40 mm.
- PLC module interface: board-mounted module via two side pin headers.
- Schematic style: island architecture with local direct routing only for small
  support circuits.
- Latest visual review artifact: `METERING_CORE_color_review.pdf`.

## Visual Cleanup Completed

- Moved the MCU reset island title away from the reset RC wiring.
- Moved `U_MCU` reference/value away from the lower power pin text.
- Spread the visible lower MCU power pins to reduce text overlap.
- Moved `DC12V_IN` / `DC12V_GND` labels inside the schematic frame.
- Shortened the PLC SPI header note to prevent frame overflow and wire overlap.
- Simplified the latching relay `COIL_COM` path to a short local `+12V` power
  port connection, removed the duplicate old `+12V` marker near the flyback
  diodes, and added a direct OFF-coil driver wire.
- Exported color PDF and PNG crops for visual inspection.

## PCB Blocking Items

KiCad ERC was run after the visual cleanup pass:

- Report: `METERING_CORE_erc_after_visual_fix.rpt`
- Result: 0 errors, 336 warnings.

The remaining ERC warnings must be triaged before release artwork. The first
visible classes are:

- Unconnected sensing pins and labels around `VP` / `VN`.
- Power pins not driven on isolated interface and MCU ground pins.
- Pull-down resistor pins in the latching relay drive island still unconnected.
- Several endpoint-off-grid warnings from custom island coordinates.
- Missing or unresolved local project library links for custom symbols and
  footprints.

## Recommended Artwork Sequence

1. Fix ERC errors that affect connectivity and power driving.
2. Resolve or document endpoint-off-grid warnings before netlist-to-PCB.
3. Confirm all footprints for production parts, especially PLC headers, relay,
   terminal blocks, CT/ZCT connectors, and AC/DC module.
4. Create a 40 mm x 40 mm board outline.
5. Place the PLC module headers first because they define mechanical alignment.
6. Place high-current relay/load terminals with clearance from MCU and metering
   domains.
7. Place MCU, ATM90E26, RS-485, power, sensing, and relay islands by functional
   zones.
8. Run DRC before generating Gerber/drill files.
