from __future__ import annotations

import argparse
import json
import os
import sys
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
        raise RuntimeError("GEMINI_API_KEY is required")

    model = os.environ.get("GEMINI_MODEL") or "gemini-2.5-pro"
    prompt = Path(args.bundle).read_text(encoding="utf-8")

    try:
        review = call_gemini(api_key, model, prompt)
    except urllib.error.HTTPError as exc:
        body = exc.read().decode("utf-8", errors="replace")
        raise RuntimeError(f"Gemini API error: {exc.code} {body}") from exc

    Path(args.output).write_text(normalize_review(review), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
