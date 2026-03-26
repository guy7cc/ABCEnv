# .vscode/scripts/bundle.ps1
param(
    [string]$Source = "main.cpp",
    [string]$Output = "submit.cpp",
    [string]$LibDir = "my-library"
)

Write-Host "[START] 1. Bundle to $Output"
$python = Join-Path $PSScriptRoot "..\..\python3\python.exe"
$expander = Join-Path $PSScriptRoot "..\..\my-library\expander.py"

& $python $expander $Source --lib $LibDir -o $Output

if ($LASTEXITCODE -eq 0) {
    Write-Host "[DONE] 1. Bundle to $Output"
} else {
    Write-Error "Bundling failed."
    exit $LASTEXITCODE
}
