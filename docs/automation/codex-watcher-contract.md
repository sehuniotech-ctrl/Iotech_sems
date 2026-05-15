# Codex Watcher Contract

This document defines the minimum behavior for any external Codex watcher.

## Why this exists

GitHub Actions can run the Gemini side inside the repository, but they cannot directly wake a Codex chat thread by themselves.

## Target thread

The intended Codex thread for this project is:

`019dd240-2788-7e12-8b6e-220dac6f23c7`

The intended workspace scope is the full project folder:

`D:\work\15_지선차단기`

The machine-readable mapping is stored in:

`docs/automation/codex-thread-link.json`

A Codex watcher can be:

- a local script on your machine
- a scheduled worker
- a future Codex automation that monitors GitHub

## Watch conditions

Trigger Codex when one of the following appears:

1. PR label `needs-codex` is added
2. A new PR comment starts with `# Gemini Review` and includes `STATUS: NEEDS_CODEX`
3. An issue titled `[Codex Queue] PR #<n> follow-up` is opened or updated

## What the watcher should read

- The PR description
- The latest `# Gemini Review` comment
- The queue issue body, if present
- The changed files in the PR

## What the watcher should do

1. Resume or wake thread `019dd240-2788-7e12-8b6e-220dac6f23c7`.
2. Open the PR context.
3. Read the latest Gemini action request.
4. Apply only the requested follow-up changes.
5. Push a new commit to the same branch.
6. Optionally add a short reply comment summarizing what was changed.

## Safety rules

- Never force-push shared branches
- Never revert unrelated files
- Stay within the files listed by the latest action request unless the fix clearly requires a linked file
- If Gemini feedback is ambiguous, prefer a narrow fix and explain assumptions in the commit or PR comment

## Completion rule

The loop is considered complete when Gemini returns:

`STATUS: REVIEW_CLEAN`

At that point:

- remove or ignore `needs-codex`
- allow `review-clean` to remain
- stop launching follow-up Codex runs
