from __future__ import annotations

import argparse
import json
import subprocess
from pathlib import Path


def run(cmd: list[str]) -> str:
    completed = subprocess.run(cmd, capture_output=True, text=True, check=True)
    return completed.stdout


def gh_json(cmd: list[str]) -> object:
    return json.loads(run(cmd))


def find_existing_issue(repo: str, title: str) -> dict | None:
    issues = gh_json(
        [
            "gh",
            "issue",
            "list",
            "--repo",
            repo,
            "--state",
            "all",
            "--search",
            f'"{title}" in:title',
            "--json",
            "number,title,state",
        ]
    )
    for issue in issues:
        if issue["title"] == title:
            return issue
    return None


def create_issue(repo: str, title: str, body_path: Path) -> None:
    run(
        [
            "gh",
            "issue",
            "create",
            "--repo",
            repo,
            "--title",
            title,
            "--label",
            "needs-codex",
            "--body-file",
            str(body_path),
        ]
    )


def edit_issue(repo: str, number: int, body_path: Path) -> None:
    run(
        [
            "gh",
            "issue",
            "edit",
            str(number),
            "--repo",
            repo,
            "--body-file",
            str(body_path),
            "--add-label",
            "needs-codex",
        ]
    )


def close_issue(repo: str, number: int) -> None:
    run(
        [
            "gh",
            "issue",
            "close",
            str(number),
            "--repo",
            repo,
            "--comment",
            "Gemini reported REVIEW_CLEAN, so this Codex queue item is closed.",
        ]
    )


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--repo", required=True)
    parser.add_argument("--pr", required=True, type=int)
    parser.add_argument("--branch", required=True)
    parser.add_argument("--review-file", required=True)
    args = parser.parse_args()

    review_path = Path(args.review_file)
    review_text = review_path.read_text(encoding="utf-8")
    title = f"[Codex Queue] PR #{args.pr} follow-up"

    queue_body = (
        "# Codex Action Request\n\n"
        f"- Branch: {args.branch}\n"
        f"- PR: #{args.pr}\n"
        f"- Repo: {args.repo}\n\n"
        "## Latest Gemini Review\n\n"
        f"{review_text}\n"
    )
    queue_path = review_path.with_name("codex_queue_issue.md")
    queue_path.write_text(queue_body, encoding="utf-8")

    existing = find_existing_issue(args.repo, title)

    if "STATUS: REVIEW_CLEAN" in review_text:
        if existing and existing["state"].lower() == "open":
            close_issue(args.repo, existing["number"])
        return 0

    if "STATUS: NEEDS_CODEX" not in review_text:
        return 0

    if existing:
        if existing["state"].lower() == "closed":
            create_issue(args.repo, title, queue_path)
        else:
            edit_issue(args.repo, existing["number"], queue_path)
    else:
        create_issue(args.repo, title, queue_path)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
