# Visual Clearance Audit Results

This file records the Codex cleanup pass for the six visual issues reported on
the color schematic preview.

## Fixed Hotspots

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

KiCad ERC still reports violations after this visual cleanup pass. This pass was
limited to schematic readability and did not claim electrical-release closure.
The ERC report should be triaged separately before PCB artwork release.
