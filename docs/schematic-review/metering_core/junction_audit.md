# Junction Audit

Automated check of `METERING_CORE.kicad_sch` junction objects.

- Junction objects checked: 40
- True 4-way junctions found: 0
- T-junctions found: 9

## T-junction coordinates

| X (mm) | Y (mm) | Directions |
| ---: | ---: | :--- |
| 44.93 | 56.91 | LRU |
| 41.12 | 170.18 | DLR |
| 41.12 | 185.18 | LRU |
| 245.00 | 260.00 | LRU |
| 44.93 | 41.91 | DLR |
| 57.15 | 267.97 | LRU |
| 327.46 | 96.66 | DLR |
| 330.00 | 96.66 | DLR |
| 332.54 | 96.66 | DLR |

## Result

No true 4-way junction was detected by coordinate analysis. The visible junction dots are T-junctions.
If a reviewer still finds a visually ambiguous location in the PDF, that exact coordinate should be staggered in the next schematic pass.
