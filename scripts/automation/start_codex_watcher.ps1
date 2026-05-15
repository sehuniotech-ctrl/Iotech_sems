param(
    [int]$IntervalSeconds = 60,
    [string]$GitHubToken,
    [switch]$AllowDangerousBypass,
    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

$RepoRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$LogDir = Join-Path $RepoRoot "tmp\codex_watcher"
New-Item -ItemType Directory -Force -Path $LogDir | Out-Null

if (-not $env:CODEX_WATCHER_THREAD_ID) {
    $env:CODEX_WATCHER_THREAD_ID = "019dd240-2788-7e12-8b6e-220dac6f23c7"
}

if (-not $env:CODEX_WATCHER_WORKSPACE) {
    $env:CODEX_WATCHER_WORKSPACE = "$RepoRoot"
}

if ($GitHubToken) {
    $env:GITHUB_TOKEN = $GitHubToken
}

if (-not $env:GITHUB_TOKEN) {
    Write-Warning "GITHUB_TOKEN is not set. Public repositories may still work, but authenticated polling is recommended."
}

$ArgsList = @(
    "scripts/automation/watch_codex_queue.py",
    "--config", "docs/automation/codex-thread-link.example.json",
    "--interval", "$IntervalSeconds"
)

if ($DryRun) {
    $ArgsList += "--dry-run"
}

if ($AllowDangerousBypass) {
    Write-Warning "AllowDangerousBypass enables Codex to run without sandbox/approval prompts. Use only on trusted branches."
    $ArgsList += "--allow-dangerous-bypass"
}

$OutLog = Join-Path $LogDir "watcher.out.log"
$ErrLog = Join-Path $LogDir "watcher.err.log"
$ArgumentString = ($ArgsList | ForEach-Object { '"' + ($_ -replace '"', '\"') + '"' }) -join " "

Start-Process `
    -FilePath "python" `
    -ArgumentList $ArgumentString `
    -WorkingDirectory $RepoRoot `
    -WindowStyle Hidden `
    -RedirectStandardOutput $OutLog `
    -RedirectStandardError $ErrLog

Write-Output "Codex watcher started. Logs: $LogDir"
