# Setup Checklist

Complete this once in GitHub before relying on the loop.

## Required repository secrets

- `GEMINI_API_KEY`: Google Gemini API key

## Optional repository variables

- `GEMINI_MODEL`: defaults to `gemini-2.5-pro`

## Recommended labels

- `needs-codex`
- `review-clean`
- `schematic`

## Recommended branch naming

- `feature/schematic-*`
- `fix/schematic-*`

## First-time validation

1. Open a PR with the schematic template.
2. Confirm the `Gemini PR Review` workflow runs.
3. Confirm a PR comment titled `# Gemini Review` appears.
4. Confirm either `needs-codex` or `review-clean` is added.
5. If `needs-codex` is added, have Codex pick up the PR and apply the requested changes.

## Notes

- The workflow only automates the GitHub review side.
- Codex wakeup still needs a watcher, scheduler, or manual invocation.
