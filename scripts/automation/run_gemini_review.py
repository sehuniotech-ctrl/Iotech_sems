from __future__ import annotations

import argparse
import json
import os
import time
import urllib.error
import urllib.request
from pathlib import Path


SYSTEM_PROMPT = """You are reviewing a GitHub pull request for schematic implementation work.
Be concrete, conservative, and action-oriented.
Only report meaningful issues.
If there is nothing meaningful to fix, return STATUS: REVIEW_CLEAN.
"""


def call_gemini(api_key: str, model: str, prompt: str) -> str:
    url = (
        "https://generativelanguage.googleapis.com/v1beta/models/"
        f"{model}:generateContent?key={api_key}"
    )
    payload = {
        "system_instruction": {
            "parts": [{"text": SYSTEM_PROMPT}],
        },
        "contents": [
            {
                "role": "user",
                "parts": [{"text": prompt}],
            }
        ],
        "generationConfig": {
            "temperature": 0.2,
        },
    }
    request = urllib.request.Request(
        url,
        data=json.dumps(payload).encode("utf-8"),
        headers={"Content-Type": "application/json"},
        method="POST",
    )
    with urllib.request.urlopen(request, timeout=120) as response:
        raw = response.read().decode("utf-8")
    data = json.loads(raw)
    candidates = data.get("candidates", [])
    if not candidates:
        raise RuntimeError(f"No candidates returned: {raw}")
    parts = candidates[0]["content"]["parts"]
    text_parts = [part.get("text", "") for part in parts]
    return "\n".join(text_parts).strip()


def normalize_review(text: str) -> str:
    cleaned = text.strip()
    if not cleaned.startswith("# Gemini Review"):
        cleaned = "# Gemini Review\n" + cleaned
    return cleaned + "\n"


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--bundle", required=True)
    parser.add_argument("--output", required=True)
    args = parser.parse_args()

    api_key = os.environ.get("GEMINI_API_KEY")
    if not api_key:
        blocked = """# Gemini Review
STATUS: REVIEW_BLOCKED

## Summary
- Gemini review could not run because `GEMINI_API_KEY` was not available to the workflow.

## Findings
- Severity: blocking. File: repository settings. Reason: add or verify the Actions secret named `GEMINI_API_KEY`.

## Codex Action Request
- Branch:
- Files:
- Required changes: none until the secret is available
- Constraints:
- Done when: rerun the workflow after adding `GEMINI_API_KEY`

## Confidence
- High
"""
        Path(args.output).write_text(blocked, encoding="utf-8")
        return 0

    model = os.environ.get("GEMINI_MODEL") or "gemini-2.5-flash"
    prompt = Path(args.bundle).read_text(encoding="utf-8")

    max_attempts = int(os.environ.get("GEMINI_REVIEW_MAX_ATTEMPTS") or "4")
    retryable_statuses = {429, 500, 502, 503, 504}
    last_error: tuple[int, str] | None = None

    for attempt in range(1, max_attempts + 1):
        try:
            review = call_gemini(api_key, model, prompt)
            Path(args.output).write_text(normalize_review(review), encoding="utf-8")
            return 0
        except urllib.error.HTTPError as exc:
            body = exc.read().decode("utf-8", errors="replace")
            last_error = (exc.code, body)
            if exc.code in retryable_statuses and attempt < max_attempts:
                time.sleep(min(15 * attempt, 60))
                continue
            break
        except Exception as exc:
            blocked = f"""# Gemini Review
STATUS: REVIEW_BLOCKED

## Summary
- Gemini review runner failed before review could complete.

## Findings
- Severity: blocking. File: `scripts/automation/run_gemini_review.py`. Reason: `{type(exc).__name__}: {exc}`.

## Codex Action Request
- Branch:
- Files:
- Required changes: inspect the workflow error and rerun after fixing it
- Constraints:
- Done when: Gemini returns `NEEDS_CODEX` or `REVIEW_CLEAN`

## Confidence
- Medium
"""
            Path(args.output).write_text(blocked, encoding="utf-8")
            return 0

    if last_error is not None:
        code, body = last_error
        blocked = f"""# Gemini Review
STATUS: REVIEW_BLOCKED

## Summary
- Gemini API returned an HTTP error before review could complete.

## Findings
- Severity: blocking. File: GitHub Actions secret or variable. Reason: Gemini API error `{code}` using model `{model}` after `{max_attempts}` attempt(s).

## Codex Action Request
- Branch:
- Files:
- Required changes: verify `GEMINI_API_KEY` and optional `GEMINI_MODEL`
- Constraints:
- Done when: rerun the workflow and Gemini returns `NEEDS_CODEX` or `REVIEW_CLEAN`

## Details
```text
{body[:2000]}
```

## Confidence
- High
"""
        Path(args.output).write_text(blocked, encoding="utf-8")
        return 0

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
