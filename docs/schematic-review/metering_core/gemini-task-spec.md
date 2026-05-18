# Gemini Task Spec: METERING_CORE Island-Safe Direct Routing Review

## Task

- Task name: Review METERING_CORE island-safe Direct Pin-to-Component routing
- Target board or sheet: `docs/schematic-review/metering_core/METERING_CORE.kicad_sch`
- Purpose: Find actionable schematic follow-up work for Codex before the next
  implementation pass.

## Files Gemini Must Review

- `docs/schematic-review/metering_core/METERING_CORE.kicad_sch`
- `docs/schematic-review/metering_core/METERING_CORE_island_safe_direct_routing.pdf`
- `docs/schematic-review/metering_core/METERING_CORE_island_safe_direct_routing_top_notes.png`
- `docs/schematic-review/metering_core/METERING_CORE_color_review.pdf`
- `docs/schematic-review/metering_core/METERING_CORE_color_review_full_hi.png`
- `docs/schematic-review/metering_core/METERING_CORE_color_review_*.png`
- `docs/schematic-review/metering_core/METERING_CORE_erc_after_visual_fix.rpt`
- `docs/schematic-review/metering_core/visual-clearance-audit.md`
- `docs/schematic-review/metering_core/visual-clearance-audit-results.md`
- `docs/schematic-review/metering_core/README.md`
- `docs/automation/schematic-style-guide.md`

## Functional Requirements

- Confirm that the schematic no longer contains stale rules that ban all direct
  local components around U2.
- Confirm that the new rule allows local direct routing only when it stays
  short, readable, and inside the same island.
- Confirm that inter-island signals remain net-label based.
- Check whether the schematic source still contains visual or reviewability
  issues that Codex should fix in a narrow follow-up commit.
- Confirm that KiCad ERC reports zero `; error` entries. Treat remaining
  warning-only items as follow-up candidates only if they are concrete and
  narrow enough for this review snapshot, not broad PCB-release cleanup.
- Specifically inspect the current visual clearance failure classes:
  - MCU bottom/top vertical power-pin text drawn inside the chip body.
  - `+3.3V`, DGND, or GNDA text overlapping pin names, pin numbers, or rails.
  - Connector/header title text touching or crossing the sheet frame.
  - Floating pin numbers without a visible pin stroke or nearby symbol boundary.
  - PLC/module pin labels visually buried inside the module body instead of
    starting outside the body.
  - Power or ground port graphics piercing an IC/module body.
  - Bottom-edge MCU or IC ground pins routed sideways into DGND/GNDA labels
    instead of straight down to downward ground ports.
  - `+3.3V`, `+12V`, or other power-port symbols placed in the middle of a wire
    rather than snapped to a wire/rail endpoint.
  - CT/ZCT terminal-block pin strokes drawn from inside the connector body
    rather than protruding outside the body edge.
  - Latching relay coil-common routing crossing the ON/OFF transistor or
    flyback diode lanes when a clear lower/upper lane is available.

## Review Constraints

- Do not require a full schematic redesign.
- Do not ask Codex to move unrelated islands.
- Do not ask Codex to change electrical connectivity unless the issue is
  clearly visible in the schematic source or review artifacts.
- Prefer one or two small actionable follow-up items over broad style advice.

## Acceptance Criteria

Gemini should return `STATUS: NEEDS_CODEX` if it finds any concrete issue such
as:

- stale or contradictory schematic notes
- unreadable or clipped review text
- IC, connector, power, or ground text overlapping symbol bodies or wires
- top/bottom IC power pins whose text or connection stubs intrude into the IC
  body
- bottom-edge ground pins that do not drop vertically to a downward ground port
- labels, references, values, or connector titles outside the sheet frame
- power ports that overlap a mid-span wire or look like a dangling V marker
- terminal-block pins that visually start inside the connector body
- relay coil wiring that crosses through the latching relay driver graphics
- floating pin numbers or module labels that cannot be tied to a visible pin
- a style-guide rule that conflicts with the schematic README
- missing review instructions that prevent future Codex/Gemini loops from
  targeting the schematic snapshot

Gemini should return `STATUS: REVIEW_CLEAN` only if the snapshot and review
automation are ready for the next schematic implementation pass with no
meaningful follow-up.
