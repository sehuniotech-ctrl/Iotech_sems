# Visual Clearance Audit Results

This file records the Codex cleanup pass for the six visual issues reported on
the color schematic preview.

## Fixed Hotspots

Latest follow-up pass:

1. MCU bottom ground ports
   - Removed the U-shaped DGND/GNDA side-label bundle at the bottom of `U_MCU`.
   - Each bottom ground-related pin now drops vertically to a downward ground
     power port placed below the IC edge.
   - Check image: `METERING_CORE_color_review_mcu_bottom_ground_fix.png`.

2. ATM90 +3.3V power-port text
   - Moved the visible `+3.3V` text off the AVDD/DVDD wires so the power port
     is read as a snapped endpoint, not as a mid-wire dangling marker.
   - Check image: `METERING_CORE_color_review_atm90_avdd_power_fix.png`.

3. CT/ZCT terminal-block pin boundary
   - Adjusted the shared CT/ZCT terminal symbol so pin terminals protrude
     outside the connector body before the sensor wires begin.
   - Check image: `METERING_CORE_color_review_ct_terminal_fix.png`.

4. Latching relay COIL_COM routing
   - Re-routed `K_RLY1` pin 2 (`COIL_COM`) into a lower clear lane so it does
     not run through the ON driver/flyback diode area.
   - Check image: `METERING_CORE_color_review_relay_coil_common_fix.png`.

Previous pass:

1. MCU lower power pins
   - Moved `DGND` / `GNDA` labels below the `U_MCU` lower pin terminals.
   - Shortened/redirected the local wires so the labels no longer sit inside
     the MCU body.
   - Check image: `METERING_CORE_color_review_mcu_bottom_fix.png`.

2. MCU upper +3.3V rail
   - Moved the `+3.3V` power-port text away from the dense VDD/VDDA/VLCD pin
     text cluster.
   - Check image: `METERING_CORE_color_review_mcu_top_fix.png`.

3. PLC SPI header near the frame
   - Moved the PLC SPI header down from the top frame area.
   - Reduced local header-label text size and removed the visible long note that
     collided with the relay/load labels.
   - Check image: `METERING_CORE_color_review_plc_header_fix.png`.

4. PLC module right-side labels
   - Hid the duplicated internal PLC RX/TX pin-name text on the module body.
   - Kept the external `PLC_RX_N/P` and `PLC_TX_N/P` labels outside the module
     body.
   - Check image: `METERING_CORE_color_review_plc_module_right_fix.png`.

5. RS-485 isolation power/ground clearance
   - Moved the left-side +3.3V power port and DGND label away from the isolator
     body so the graphic/text no longer pierces the chip.
   - Check image: `METERING_CORE_color_review_iso_left_fix.png`.

6. Power output capacitor text
   - Moved `C_PWR_OUT1` / `C_PWR_OUT2` reference text above the output rail.
   - Moved the +3.3V power port farther right so it no longer overlaps the
     capacitor labels.
   - Check image: `METERING_CORE_color_review_pwr_out_fix.png`.

## Generated Artifacts

- Color PDF: `METERING_CORE_color_review.pdf`
- Full-sheet PNG: `METERING_CORE_color_review_full_hi.png`
- Hotspot PNGs: `METERING_CORE_color_review_*_fix.png`
- ERC report: `METERING_CORE_erc_after_visual_fix.rpt`

## ERC Note

The follow-up ERC cleanup pass reduced KiCad ERC **errors to zero** in
`METERING_CORE_erc_after_visual_fix.rpt`.

Residual ERC items are warnings only:

- `endpoint_off_grid`: inherited review-snapshot coordinates that should be
  normalized during the PCB-ready grid cleanup pass.
- `lib_symbol_mismatch` / `lib_symbol_issues`: expected while this repo-local
  review snapshot embeds or references project-local `SmartLoad` symbols without
  installing the full local KiCad library table in CI.
- `isolated_pin_label`: expected for island-style review stubs where some nets
  intentionally terminate at labels for later block integration.
- `footprint_link_issues`: expected for placeholder review symbols whose final
  footprints are still part of the PCB/artwork preparation step.
- `unconnected_wire_endpoint`: remaining review-snapshot custom-symbol endpoint
  warnings around DC input and latching-relay island stubs; these are warnings,
  not ERC errors, and should be handled with the broader symbol/footprint
  normalization pass rather than this visual-clearance patch.
