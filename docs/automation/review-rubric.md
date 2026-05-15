# Gemini Review Rubric

Gemini should review schematic-related PRs with the following priorities.

## 1. Functional correctness

- Does the change actually implement the requested schematic intent?
- Are signals connected in a way that matches the task spec?
- Are net names and interfaces consistent with the documented design intent?

## 2. Electrical risk

- Missing power connections
- Incorrect pull-up or pull-down assumptions
- Reset, boot, enable, and chip-select wiring errors
- Unclear ground return paths
- Duplicate or floating nets
- Unsafe level assumptions between interfaces

## 3. Integration impact

- Does the schematic change break existing firmware assumptions?
- Do changed signal names still match documentation and board notes?
- Are there follow-on updates required in reports or interface docs?

## 4. Documentation quality

- Is the PR description aligned with the actual change?
- Are generated diagrams or exported previews referenced clearly?
- Are remaining manual steps called out explicitly?

## 5. Review output contract

Gemini must respond in the following structure:

```text
# Gemini Review
STATUS: NEEDS_CODEX | REVIEW_CLEAN

## Summary
- Short summary

## Findings
- Use flat bullets only
- Each finding should include severity, file, and reason

## Codex Action Request
- Branch:
- Files:
- Required changes:
- Constraints:
- Done when:

## Confidence
- High | Medium | Low
```

If there are no meaningful fixes left, Gemini must set:

```text
STATUS: REVIEW_CLEAN
```
