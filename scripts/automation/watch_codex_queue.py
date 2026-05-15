from __future__ import annotations

import argparse
import json
import os
import subprocess
import time
import urllib.error
import urllib.request
from dataclasses import dataclass
from pathlib import Path


@dataclass
class LinkConfig:
    thread_id: str
    workspace: str
    repository: str
    needs_label: str
    clean_label: str
    blocked_label: str
    review_header: str


def api_get(url: str, token: str | None = None) -> object:
    headers = {
        "Accept": "application/vnd.github+json",
        "User-Agent": "codex-queue-watcher",
    }
    if token:
        headers["Authorization"] = f"Bearer {token}"
    req = urllib.request.Request(url, headers=headers)
    with urllib.request.urlopen(req, timeout=60) as response:
        return json.loads(response.read().decode("utf-8"))


def load_config(path: Path) -> LinkConfig:
    data = json.loads(path.read_text(encoding="utf-8"))
    signals = data["github_signals"]
    thread_id = data.get("target_thread_id") or os.environ.get(
        data.get("target_thread_id_env", "CODEX_WATCHER_THREAD_ID"),
    )
    workspace = data.get("workspace") or os.environ.get(
        data.get("workspace_env", "CODEX_WATCHER_WORKSPACE"),
    )
    if not thread_id:
        raise RuntimeError("Set CODEX_WATCHER_THREAD_ID or provide target_thread_id in config")
    if not workspace:
        raise RuntimeError("Set CODEX_WATCHER_WORKSPACE or provide workspace in config")
    return LinkConfig(
        thread_id=thread_id,
        workspace=workspace,
        repository=data["repository"],
        needs_label=signals["pull_request_label"],
        clean_label=signals["clean_label"],
        blocked_label=signals["blocked_label"],
        review_header=signals["review_comment_header"],
    )


def load_state(path: Path) -> dict:
    if not path.exists():
        return {"handled": {}}
    return json.loads(path.read_text(encoding="utf-8"))


def save_state(path: Path, state: dict) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(state, ensure_ascii=False, indent=2), encoding="utf-8")


def label_names(issue: dict) -> set[str]:
    return {label["name"] for label in issue.get("labels", [])}


def latest_gemini_comment(repo: str, issue_number: int, token: str | None) -> dict | None:
    comments = api_get(
        f"https://api.github.com/repos/{repo}/issues/{issue_number}/comments?per_page=100",
        token,
    )
    gemini_comments = [
        comment
        for comment in comments
        if comment.get("body", "").lstrip().startswith("# Gemini Review")
    ]
    if not gemini_comments:
        return None
    return gemini_comments[-1]


def build_prompt(config: LinkConfig, pr: dict, comment: dict) -> str:
    return f"""GitHub에서 Codex 자동 후속 작업 신호가 감지됐습니다.

대상 프로젝트 전체 범위: {config.workspace}
대상 저장소: {config.repository}
대상 PR: #{pr["number"]} {pr["html_url"]}
브랜치: {pr["head"]["ref"]}

최신 Gemini 리뷰 댓글:

{comment["body"]}

해야 할 일:
1. PR과 최신 Gemini 리뷰를 읽고 수정 요청을 반영하세요.
2. 기존 사용자 작업을 되돌리지 말고 필요한 파일만 좁게 수정하세요.
3. 수정 후 같은 브랜치에 커밋/푸시하세요.
4. 수정 내용과 남은 위험을 짧게 정리하세요.
5. Gemini가 다시 리뷰할 수 있게 PR 상태를 유지하세요.
"""


def run_codex(config: LinkConfig, prompt: str, log_dir: Path, allow_dangerous_bypass: bool) -> int:
    log_dir.mkdir(parents=True, exist_ok=True)
    stamp = time.strftime("%Y%m%d_%H%M%S")
    output_path = log_dir / f"codex_resume_{stamp}.md"

    cmd = [
        "codex",
        "exec",
        "-C",
        config.workspace,
        "-o",
        str(output_path),
        "resume",
        config.thread_id,
        "-",
    ]
    if allow_dangerous_bypass:
        cmd.insert(4, "--dangerously-bypass-approvals-and-sandbox")
    completed = subprocess.run(
        cmd,
        input=prompt,
        text=True,
        capture_output=True,
    )
    (log_dir / f"codex_resume_{stamp}.stdout.log").write_text(
        completed.stdout,
        encoding="utf-8",
    )
    (log_dir / f"codex_resume_{stamp}.stderr.log").write_text(
        completed.stderr,
        encoding="utf-8",
    )
    return completed.returncode


def scan_once(
    config: LinkConfig,
    state: dict,
    token: str | None,
    log_dir: Path,
    dry_run: bool,
    allow_dangerous_bypass: bool,
) -> bool:
    repo = config.repository
    pulls = api_get(f"https://api.github.com/repos/{repo}/pulls?state=open&per_page=50", token)
    changed = False

    for pr in pulls:
        issue = api_get(f"https://api.github.com/repos/{repo}/issues/{pr['number']}", token)
        labels = label_names(issue)
        if config.needs_label not in labels:
            continue
        if config.clean_label in labels or config.blocked_label in labels:
            continue

        comment = latest_gemini_comment(repo, pr["number"], token)
        if not comment:
            continue
        if "STATUS: NEEDS_CODEX" not in comment.get("body", ""):
            continue

        key = f"pr-{pr['number']}"
        marker = f"{pr['head']['sha']}:{comment['id']}"
        if state["handled"].get(key) == marker:
            continue

        prompt = build_prompt(config, pr, comment)
        prompt_path = log_dir / f"pending_pr_{pr['number']}.md"
        log_dir.mkdir(parents=True, exist_ok=True)
        prompt_path.write_text(prompt, encoding="utf-8")

        if dry_run:
            print(f"[dry-run] would wake Codex for PR #{pr['number']}")
        else:
            print(f"[watcher] waking Codex for PR #{pr['number']}")
            rc = run_codex(config, prompt, log_dir, allow_dangerous_bypass)
            if rc != 0:
                print(f"[watcher] codex exited with {rc}; will retry next scan")
                continue

        state["handled"][key] = marker
        changed = True

    return changed


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", default="docs/automation/codex-thread-link.json")
    parser.add_argument("--state", default="tmp/codex_watcher/state.json")
    parser.add_argument("--log-dir", default="tmp/codex_watcher")
    parser.add_argument("--interval", type=int, default=60)
    parser.add_argument("--once", action="store_true")
    parser.add_argument("--dry-run", action="store_true")
    parser.add_argument("--allow-dangerous-bypass", action="store_true")
    parser.add_argument("--token-env", default="GITHUB_TOKEN")
    args = parser.parse_args()

    config = load_config(Path(args.config))
    state_path = Path(args.state)
    state = load_state(state_path)
    token = None

    token = os.environ.get(args.token_env)

    while True:
        try:
            if scan_once(
                config,
                state,
                token,
                Path(args.log_dir),
                args.dry_run,
                args.allow_dangerous_bypass,
            ):
                save_state(state_path, state)
        except urllib.error.HTTPError as exc:
            print(f"[watcher] GitHub API error {exc.code}: {exc.reason}")
        except Exception as exc:
            print(f"[watcher] unexpected error: {type(exc).__name__}: {exc}")

        if args.once:
            break
        time.sleep(args.interval)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
