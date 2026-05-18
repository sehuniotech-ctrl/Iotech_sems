from __future__ import annotations

import argparse
import json
import subprocess
import sys
from pathlib import Path


def run(cmd: list[str]) -> str:
    completed = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return completed.stdout


def load_event(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def git_diff_base_head(base: str, head: str) -> str:
    return run(["git", "diff", f"{base}...{head}"])


def gh_pr_view(repo: str, pr_number: int) -> dict:
    data = run(
        [
            "gh",
            "pr",
            "view",
            str(pr_number),
            "--repo",
            repo,
            "--json",
            "title,body,headRefName,baseRefName,author,files,labels",
        ]
    )
    return json.loads(data)


def read_optional(path: Path) -> str:
    if not path.exists():
        return ""
    return path.read_text(encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo", required=True)
    parser.add_argument("--pr", required=True, type=int)
    parser.add_argument("--event-path", required=True)
    parser.add_argument("--rubric", required=True)
    parser.add_argument("--style-guide", default="")
    parser.add_argument("--task-spec", default="")
    args = parser.parse_args()

    event = load_event(Path(args.event_path))
    pr = gh_pr_view(args.repo, args.pr)

    base_sha = event["pull_request"]["base"]["sha"]
    head_sha = event["pull_request"]["head"]["sha"]
    diff_text = git_diff_base_head(base_sha, head_sha)

    changed_files = []
    for item in pr.get("files", []):
        changed_files.append(f'- {item["path"]}')

    task_spec = read_optional(Path(args.task_spec)) if args.task_spec else ""
    rubric = read_optional(Path(args.rubric))
    style_guide = read_optional(Path(args.style_guide)) if args.style_guide else ""

    bundle = f"""# Review Bundle

## Repository

- Repo: {args.repo}
- PR: #{args.pr}
- Title: {pr.get("title", "")}
- Head: {pr.get("headRefName", "")}
- Base: {pr.get("baseRefName", "")}

## PR Body

{pr.get("body", "")}

## Changed Files

{chr(10).join(changed_files)}

## Shared Review Rubric

{rubric}

## Schematic Style Guide

{style_guide}

## Optional Task Spec

{task_spec}

## Unified Diff

```diff
{diff_text}
```

## Required Reviewer Behavior

Return exactly this structure:

```text
# Gemini Review
STATUS: NEEDS_CODEX | REVIEW_CLEAN

## Summary
- ...

## Findings
- ...

## Codex Action Request
- Branch:
- Files:
- Required changes:
- Constraints:
- Done when:

## Confidence
- High | Medium | Low
```
"""
    sys.stdout.write(bundle)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
