# Schematic Automation Loop

This repository can run a GitHub-centered review loop for schematic work.

## Goal

Use GitHub as the shared event hub between:

- Codex: implements the requested schematic change and pushes updates
- Gemini: reviews the latest PR diff and leaves structured feedback
- GitHub Actions: runs the review and adds labels that drive the next step

## Recommended flow

1. Create or update a branch for one schematic task.
2. Open a PR using the schematic PR template.
3. GitHub Actions runs Gemini on PR open, reopen, and synchronize.
4. Gemini posts a structured review comment.
5. If fixes are required, the workflow adds the `needs-codex` label.
6. Codex reads the PR and the latest Gemini review, applies changes, and pushes again.
7. The review reruns until Gemini reports no further action items.

## Files

- `docs/automation/task-spec-template.md`
- `docs/automation/review-rubric.md`
- `docs/automation/codex-action-template.md`
- `docs/automation/codex-watcher-contract.md`
- `docs/automation/setup-checklist.md`
- `scripts/automation/build_review_bundle.py`
- `scripts/automation/run_gemini_review.py`
- `scripts/automation/sync_codex_queue_issue.py`
- `.github/workflows/gemini-review.yml`

## Shared conventions

### One task per PR

Keep each PR focused on one schematic implementation target. This makes the review loop much more stable.

### Structured Gemini output

Gemini is asked to always return one of these top-level states:

- `STATUS: NEEDS_CODEX`
- `STATUS: REVIEW_CLEAN`

The workflow uses that marker to add or remove the `needs-codex` label.

### Codex pickup rule

Codex should treat the latest PR comment that starts with `# Gemini Review` as the current action request.

## What this setup automates

- Build a review bundle from the current PR
- Call Gemini with repository rubric + task context
- Post the review back to the PR
- Add `needs-codex` when further edits are required
- Add `review-clean` when Gemini reports no blocking feedback
- Create or update a GitHub issue that acts as the Codex follow-up queue

## What still needs external wiring

This repo can fully automate the GitHub side, but Codex thread wakeup is not natively available from this repository alone.

You still need one of these:

1. A Codex automation that watches GitHub labels or PR comments
2. A local/background runner that launches Codex when `needs-codex` appears
3. A manual trigger where you open Codex on the labeled PR

If you later add a Codex watcher, it should monitor:

- PR label `needs-codex`
- New PR comment whose header is `# Gemini Review`
- Updated issue body matching `Codex Action Request`
