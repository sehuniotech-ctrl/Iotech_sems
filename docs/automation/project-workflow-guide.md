# IOTECH Smart Load Project Workflow Guide

This document is the single entry point for future Codex/Gemini-assisted work
on the Smart Load / branch circuit breaker project.

Before starting a new task in another Codex thread, read this file first, then
follow the linked domain-specific guides.

## 1. Scope

Use this workflow for all project work, including:

- KiCad schematic updates
- PCB/artwork preparation notes
- firmware changes
- communication protocol documents
- hardware review reports
- schedule, WBS, and meeting-note updates
- automation scripts and GitHub PR review loops

## 2. Required Reading Order

For every task, read these first:

1. `docs/automation/project-workflow-guide.md`
2. `docs/automation/README.md`
3. `docs/automation/review-rubric.md`

For schematic or PCB-related work, also read:

4. `docs/automation/schematic-style-guide.md`

For Gemini/Codex automation setup or watcher work, also read:

5. `docs/automation/setup-checklist.md`
6. `docs/automation/codex-watcher-contract.md`
7. `docs/automation/codex-thread-link.json`

## 3. Universal Working Rules

1. Keep the task scope small.
   - Prefer one focused change per branch and PR.
   - Do not mix schematic, firmware, and protocol changes unless the task
     explicitly requires cross-domain coordination.

2. Preserve user work.
   - Never revert unrelated user edits.
   - If the worktree is dirty, inspect the target files before editing.
   - Modify only files needed for the current task.

3. Explain assumptions.
   - If a hardware pin, part number, footprint, or protocol behavior is not
     confirmed, mark it as a TODO or open question instead of silently guessing.

4. Prefer source-backed changes.
   - For hardware: use datasheets, existing schematic/net names, and project
     notes.
   - For firmware: use existing board headers, pin maps, and target abstraction
     files.
   - For documents: preserve existing terminology unless the task explicitly
     renames it.

5. Generate reviewable output.
   - For schematic changes, export a PDF or PNG crop where possible.
   - For firmware changes, run the smallest relevant build or syntax check.
   - For documentation changes, summarize the exact files and sections changed.

## 4. Schematic and PCB Rules

For KiCad schematic work, `docs/automation/schematic-style-guide.md` is the
primary rulebook.

Key principles:

- Inputs and sources on the left, outputs and loads on the right.
- Power ports face upward, ground ports face downward.
- Keep functional blocks grouped as readable islands.
- Use net labels for inter-island signals instead of long wires.
- Add visible connectors or terminal blocks for external PCB signals.
- CT and ZCT inputs must have terminal symbols.
- Avoid 4-way junction ambiguity.
- Keep text clear of wires, pins, symbols, and sheet borders.
- Direct Pin-to-Component routing is allowed only inside a local island when it
  improves readability and does not break the island architecture.

## 5. GitHub PR and Gemini Review Loop

Use this loop for tasks that should be reviewed by Gemini:

1. Create or switch to a focused branch.
2. Make the smallest useful change.
3. Run local validation.
4. Commit and push.
5. Open or update a GitHub PR.
6. Let the `Gemini PR Review` workflow run.
7. Read the latest PR comment starting with `# Gemini Review`.
8. If Gemini returns `STATUS: NEEDS_CODEX`, apply only the requested fixes and
   push again.
9. Repeat until Gemini returns `STATUS: REVIEW_CLEAN`.

The review workflow uses:

- `.github/workflows/gemini-review.yml`
- `docs/automation/review-rubric.md`
- `docs/automation/schematic-style-guide.md`
- `scripts/automation/build_review_bundle.py`
- `scripts/automation/run_gemini_review.py`
- `scripts/automation/sync_codex_queue_issue.py`

## 6. Local Codex Watcher

To let Codex pick up Gemini follow-up requests automatically, start the local
watcher from the repository root:

```powershell
powershell -ExecutionPolicy Bypass -File scripts/automation/start_codex_watcher.ps1
```

The watcher looks for:

- PR label `needs-codex`
- a PR comment starting with `# Gemini Review`
- `STATUS: NEEDS_CODEX`

Then it resumes the configured Codex thread and asks it to apply the latest
Gemini action request.

## 7. Prompt To Reuse In New Codex Threads

Use this prompt when starting another thread:

```text
D:\work\15_지선차단기\docs\automation\project-workflow-guide.md를 먼저 읽고,
그 문서에 연결된 자동화/검토 규칙을 따라 이번 작업을 진행해줘.

작업 규칙:
- 기존 사용자 작업은 되돌리지 마.
- 관련 없는 파일은 수정하지 마.
- 작은 범위로 작업하고 필요한 검증을 실행해.
- 작업 후 GitHub PR을 만들고 Gemini 리뷰까지 확인해.
- Gemini가 STATUS: NEEDS_CODEX를 주면 해당 피드백만 반영하고 다시 push해.
- Gemini가 STATUS: REVIEW_CLEAN을 줄 때까지 반복해.
```

## 8. Completion Criteria

A task is complete when:

- The requested change is implemented.
- Relevant local validation has run or the reason it could not run is stated.
- Modified files are summarized.
- Remaining TODOs or hardware risks are listed.
- If PR/Gemini review was requested, Gemini returns `STATUS: REVIEW_CLEAN` or
  any blocker is clearly reported.
