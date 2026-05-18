# METERING_CORE Gemini Review Snapshot

This folder contains a repository-local snapshot of the current KiCad
`METERING_CORE` schematic so Gemini can review the schematic text and exported
preview artifacts from a GitHub PR.

## Files

- `METERING_CORE.kicad_sch`
  - KiCad schematic source copied from the local working schematic.
  - Primary file for Gemini text/diff review.
- `METERING_CORE_island_safe_direct_routing.pdf`
  - KiCad PDF export for human visual inspection.
- `METERING_CORE_island_safe_direct_routing_top_notes.png`
  - Rendered crop showing the updated island-safe direct routing rule notes.
- `METERING_CORE_color_review.pdf`
  - Color KiCad PDF export used for visual review before PCB/artwork.
- `METERING_CORE_color_review_*.png`
  - Rendered color crops for the reset island, MCU bottom labels, DC12V input,
    PLC header, and full-sheet visual inspection.
- `METERING_CORE_erc_after_visual_fix.rpt`
  - KiCad ERC report captured after the visual cleanup pass.
- `PCB_ARTWORK_READINESS.md`
  - PCB/artwork readiness note for the 40 mm x 40 mm board transition.
- `visual-clearance-audit.md`
  - Explicit checklist for MCU power-pin text, PLC/module label boundaries,
    power/ground port piercing, frame clearance, and similar visual failures.
- `junction_audit.md`
  - Coordinate-based audit of schematic junction objects.
  - Records whether any true 4-way junctions exist in the schematic source.

## Review Focus

Gemini should review this snapshot against:

- `docs/automation/project-workflow-guide.md`
- `docs/automation/review-rubric.md`
- `docs/automation/schematic-style-guide.md`

The main rule change being reviewed is:

- Direct Pin-to-Component routing is allowed only inside the same local island.
- Inter-island connections should remain net-label based.
- Local support parts such as crystals, reset RC filters, pull-up/down
  resistors, and decoupling capacitors may use short direct wires only when the
  result improves readability and does not crowd the IC.

## Current Intent

This snapshot does not claim the schematic is final for PCB release. It is a
reviewable checkpoint so Gemini can flag style, connectivity, and documentation
issues before the next Codex implementation pass.

## Generated Outputs

This review snapshot intentionally includes generated schematic preview
artifacts:

- PDF export: `METERING_CORE_island_safe_direct_routing.pdf`
- PNG crop: `METERING_CORE_island_safe_direct_routing_top_notes.png`
- Color PDF export: `METERING_CORE_color_review.pdf`
- Color PNG crops: `METERING_CORE_color_review_*.png`
- Visual clearance audit: `visual-clearance-audit.md`

These files are included so reviewers can compare the KiCad source with a human
readable preview.
