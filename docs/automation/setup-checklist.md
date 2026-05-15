# Setup Checklist

Complete this once in GitHub before relying on the loop.

## Required repository secrets

- `GEMINI_API_KEY`: Google Gemini API key

## Optional local watcher environment

- `GITHUB_TOKEN`: GitHub token for authenticated local polling. Public repositories may work without it, but setting it avoids low anonymous API limits and is required for private repositories.
- `CODEX_WATCHER_THREAD_ID`: target Codex thread id. The starter script defaults this to `019dd240-2788-7e12-8b6e-220dac6f23c7`.
- `CODEX_WATCHER_WORKSPACE`: local project folder. The starter script defaults this to the repository root.
- `AllowDangerousBypass`: optional PowerShell switch for the starter script. Leave it off by default for schematic and hardware work.

## Optional repository variables

- `GEMINI_MODEL`: defaults to `gemini-2.5-flash`
- `GEMINI_REVIEW_MAX_ATTEMPTS`: defaults to `4` for retrying temporary Gemini API errors

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
6. Start the local watcher if you want Codex follow-up to run without manual pickup.

## Notes

- The workflow only automates the GitHub review side.
- Codex wakeup still needs a watcher, scheduler, or manual invocation.
