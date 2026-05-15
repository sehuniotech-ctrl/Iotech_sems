# Codex Action Request

Use this format in a PR comment when follow-up work is required.

```text
# Codex Action Request
- Branch:
- PR:
- Files:
- Required changes:
- Constraints:
- Done when:
```

## Example

```text
# Codex Action Request
- Branch: feature/schematic-uart-protection
- PR: #17
- Files: 02_하드웨어/회로도/..., 09_보고서/...
- Required changes: add missing pull-up on reset net, align UART labels with interface note
- Constraints: do not rename unchanged power nets, keep connector pin numbers stable
- Done when: schematic preview and notes match the updated connection plan
```
