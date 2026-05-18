# IOTECH KiCad Schematic Style Guide

This guide is the required review standard for Smart Load schematic PRs.
Gemini should use this document as the primary visual and electrical readability
rubric when reviewing schematic-related changes.

## Core Rules

1. Signal flow
   - Place inputs, sources, connectors, and sensor terminals on the left side of
     each functional block.
   - Place outputs, loads, relays, and field outputs on the right side.

2. Power and ground direction
   - Power ports such as +12V, +5V, +3.3V, and VCC must face upward.
   - Ground ports such as GND, DGND, GNDA, AGND, and RS485_GND must face
     downward.
   - Wires must stop exactly at the power or ground port terminal. Wires must
     not pass through the graphical body of a power or ground symbol.

3. Functional grouping
   - Group circuits as readable islands: Power Supply, MCU Core, Metering Core,
     CT/ZCT Input, Voltage Sensing, RS-485, PLC, Relay Drive, and Load Switching.
   - Leave visible whitespace between islands.
   - Do not move unrelated islands when making a localized schematic change.

4. Net label usage
   - Use net labels for inter-island connections and long buses.
   - Avoid long wires across the sheet.
   - Net labels must not overlap pins, wires, reference designators, or values.

5. Decoupling capacitors
   - Decoupling capacitors should be placed close to the related IC power pins
     or in a clearly named decoupling island.
   - Each capacitor must have an unambiguous power and ground connection.

6. No ambiguous junctions
   - Avoid 4-way cross junctions with a single dot.
   - Use staggered T-junctions to avoid misreading a crossing as a short.

7. Text cleanliness
   - Reference designators and values must not overlap wires, symbols, pin names,
     pin numbers, or sheet borders.
   - Repeated component text should be aligned on the same grid where possible.
   - IC pin names, pin numbers, net labels, and power labels must not be drawn
     inside the filled body of a different symbol. If text is visually inside an
     IC, connector, relay, or module body and it is not that symbol's intentional
     internal pin name, the schematic fails visual review.

8. External terminals
   - Any signal entering or leaving the PCB must have a connector or terminal
     symbol.
   - CT and ZCT sensor inputs must use visible terminal block or connector
     symbols, not only net labels.

9. Visual alignment and aesthetics
   - Align the top edges of major ICs, especially STM32L053R8T6 and ATM90E26,
     when doing a full-sheet cleanup.
   - Place CT and ZCT sensing islands adjacent to each other and visually
     symmetric where sheet space allows.
   - Keep repeated resistor or capacitor banks aligned with uniform spacing.
   - Balance large islands across the sheet. Avoid one overcrowded corner and one
     empty corner unless the empty space is reserved for PCB notes or title
     block clearance.

10. Island-safe direct pin-to-component routing
   - Direct Pin-to-Component routing is allowed only inside the same local island
     and only when it improves readability.
   - Good candidates: crystals, reset RC filters, boot pull resistors, required
     pull-up or pull-down resistors, and local decoupling capacitors.
   - Do not directly wire distant functional blocks together. Inter-island
     signals must remain net-label based.
   - If direct routing would crowd the IC, use a short pin stub plus a net label,
     then place the local circuit in a named island.
   - Direct wires must be short, orthogonal, and free of 4-way junctions.
   - Decision rule:
     - Use direct routing when the component belongs physically and visually to
       the same IC island and the wire length stays short.
     - Use net labels when the component belongs to a separate functional
       island, when the wire would cross another block, or when direct routing
       would reduce readability.
     - If a local component is split into a named island for clarity, label the
       island clearly, for example `ATM90 crystal local island` or
       `MCU reset local island`.
   - Reviewers should flag direct routing only when it creates long wires,
     crosses island boundaries, or hides the relationship between the IC pin and
     the local component.

11. IC top and bottom power-pin readability
   - For vertical IC pins on the top or bottom edge, the connection point and
     the visible pin text must stay outside the IC body. Wires should meet the
     pin at the outside terminal and must not run through the filled IC body.
   - Do not place DGND, GNDA, AGND, +3.3V, VDD, VSS, VDDA, VSSA, or similar
     power text inside the MCU or IC body when it is meant to describe an
     external net.
   - If several power pins are close together, spread them on the symbol or use
     a local decoupling island so pin numbers and pin names remain readable.
   - Power-port text such as `+3.3V` must be above the rail or at the left/right
     end of the rail with clear spacing. It must not overlap pin names or pin
     numbers.

12. Connector and module text boundary
   - Header and module labels must start outside the symbol body. For example,
     PLC RX/TX pin labels should be outside the `U_PLC` body or represented as
     external net labels; they must not be visually buried inside the yellow
     module box.
   - Connector reference/value text must not touch the sheet frame. If a header
     is too close to the top or right border, move the whole island inward or
     shorten the note text.
   - Pin numbers must be anchored near the actual pin terminal. Floating pin
     numbers with no visible pin stroke or symbol boundary are not acceptable.

13. Power and ground port clearance around ICs
   - A power or ground port connected to an IC must sit outside the IC body with
     the wire ending at the port terminal. The port graphic must not pierce or
     overlap the IC body.
   - Ground ports connected to lower pins must face downward and sit below the
     wire, not inside the chip or module.
   - Power ports connected to upper pins must face upward and sit above the
     wire, not inside the chip or module.
   - When a power rail feeds multiple pins, put the power-port symbol at one
     uncluttered rail end, preferably the left end for a horizontal rail, and
     keep at least one grid step of spacing from pin text.
   - Bottom-edge IC ground pins must route straight downward from the pin
     terminal to a downward ground port. Do not fold adjacent bottom pins into a
     U-shaped side label bundle, and do not place DGND/GNDA text between or
     inside the IC pins.
   - Power ports such as `+3.3V` and `+12V` must be placed at the end of the
     wire or rail. A power-port glyph or label placed on the middle of a wire is
     a visual-review failure because it looks like a dangling marker or an
     overlap.

14. Sheet-frame clearance
   - All text, labels, symbols, and wires must stay inside the drawing frame.
   - Keep at least 2.54 mm clearance from the sheet frame for normal labels and
     at least 5.08 mm for long note text.
   - If an island cannot fit, move the island or shorten the note. Do not allow
     titles to ride on the frame line.

15. Connector pin boundary
   - Connector and terminal-block pin strokes must protrude from the outside
     edge of the symbol body. A wire must connect to the external pin terminal,
     not appear to start from inside the connector box.
   - Pin numbers and pin names on terminal blocks must not be visually buried
     inside the body border. If the symbol footprint is small, move the text
     outside or enlarge the body rather than letting the pin stroke intrude.

16. Relay coil routing lanes
   - Latching relay ON and OFF coil-drive nets must use separate horizontal
     lanes. The common coil pin must not route through the ON/OFF transistor,
     diode, or control-label area when there is open space below or above.
   - Relay coil wires should leave the relay pin orthogonally, then turn in an
     uncluttered lane. Do not cross through `RELAY_ON_CTRL`, `RELAY_OFF_CTRL`,
     flyback diode labels, or transistor bodies.
   - Latching relay common coil pins such as `COIL_COM` must use the shortest
     readable route to their supply rail, normally `+12V`. Do not preserve an
     old wire path if it creates a loop, detour, or route that goes down and
     then back up through another driver lane.
   - `COIL_COM` must not be physically routed through, over, or into the
     flyback diode lane. If the flyback diode cathode also belongs to `+12V`,
     use a separate `+12V` Power Port at the diode cathode rather than drawing
     a long shared green wire from `COIL_COM` up to the diode rail.
   - If a relay pin naturally exits downward, route it downward first and place
     the power or ground port at the wire endpoint. Do not bend the wire around
     unrelated blocks just to reuse an existing rail.
   - When a previous schematic edit leaves a connected-but-ugly detour, prefer
     deleting and redrawing that local wire segment over extending the detour.
     Electrical connectivity alone is not sufficient; the visual path must also
     communicate the circuit intent.
   - Do not freeze component locations so strongly that a local relay-driver
     wire must jog around empty space. For ON/OFF latching relay drivers, it is
     acceptable and preferred to move the related local driver parts as a group
     so the transistor collector, flyback diode, and relay coil pin share a
     straight horizontal lane.
   - Hide or move package-only diode pin text such as `A` / `K` when it lands on
     the flyback common rail. The diode orientation must remain clear from the
     symbol graphic and value text; duplicated `K K` or `A A` rail text is a
     visual defect.
   - Flyback diode graphics must not leave a stray vertical pin/stub protruding
     above or below a horizontal relay rail. If the symbol pin stroke creates a
     useless-looking stub, redesign the local diode symbol/pin length so the
     visible diode body terminates cleanly at the rail endpoint.
   - Do not distort a standard diode's graphic geometry to solve a routing or
     text-clearance problem. The diode bar/triangle proportions must remain
     recognizable as a normal diode symbol. Fix visual issues by moving the
     diode, moving labels, hiding package-only pin names, or redrawing local
     wires, not by stretching the diode body into an X/hourglass shape.
   - Diode pin leads must not protrude beyond the relay rail they connect to.
     If a custom symbol is needed, keep the electrical pin terminal on the rail
     and draw any visible lead only inside the diode body span. A red lead stub
     sticking above or below the green rail is a visual failure.

17. Power-port text clearance
   - Power-port text such as `+3.3V`, `+5V`, and `+12V` must not overlap IC
     body text, package notes, pin numbers, pin names, or title notes.
   - If a power port is close to an IC edge, move the power text outward/upward
     and move nearby notes away. Do not allow voltage text to sit on top of
     `LQFP64`, `VDD`, `VDDA`, `VSS`, or similar pin/package text.
   - A visual review or Gemini review must reject any new schematic change that
     leaves voltage text partially covering another label, even when the net is
     electrically correct.
   - A Power Port graphic must not float in empty space. If the red V/arrow or
     ground triangle is not visibly attached to a wire endpoint or a visible
     pin stub, delete it or redraw the local wire so the port snaps to the
     endpoint.
   - A Power Port graphic must never penetrate an IC or module body. If the red
     V/arrow, ground triangle, or its text lands inside a yellow symbol body,
     move the port outside the body and connect it with a short orthogonal
     wire.
   - Do not place a Power Port directly on the end of a long horizontal wire
     when it reads like a dangling red V. Route the signal to a local node,
     draw a short vertical stub, and place the power symbol at that vertical
     endpoint.
   - Visible IC power and ground pins should have one clear local port per
     visible pin. Hidden or auxiliary power pins must not create extra visible
     ground/power triangles floating between visible pins unless the pin itself
     is also visible.

18. IC body and note text clearance
   - No label, reference, value, note, or helper text may start inside an IC,
     connector, relay, or module body unless it is an intentional pin name or
     symbol-internal field.
   - If a long blue note or island title collides with circuitry or the sheet
     frame, split it into two or three shorter text objects instead of forcing
     a single long line.

## Smart Load Specific Checks

- MCU must remain STM32L053R8T6 unless the PR explicitly changes it.
- Metering IC must remain ATM90E26-YU or the approved orderable variant unless
  the PR explicitly changes it.
- DCU UART/RS-485, PLC SPI, and ATM90 SPI nets must be named so their target is
  clear.
- Latching relay drive must clearly distinguish RELAY_ON_CTRL and
  RELAY_OFF_CTRL.
- Live/load switching nets must be visually separated from low-voltage MCU and
  metering logic.
- Any schematic change that affects connectivity should include updated PDF or
  PNG preview artifacts for human review.
- Visual review must explicitly inspect the common failure cases documented in
  `docs/schematic-review/metering_core/visual-clearance-audit.md` when that file
  exists.
