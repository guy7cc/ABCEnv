# .vscode/scripts/run.ps1
param(
    [string]$Exe = "submit.exe",
    [string]$InFile = ""
)

$exePath = Join-Path $PSScriptRoot "..\..\$Exe"

if (!(Test-Path $exePath)) {
    Write-Error "$Exe not found. Please build first."
    exit 1
}

Write-Host "[START] Run $Exe"
if ($InFile -ne "") {
    $inPath = Join-Path $PSScriptRoot "..\..\$InFile"
    if (Test-Path $inPath) {
        Get-Content $inPath | & $exePath
    } else {
        Write-Error "Input file $InFile not found."
        exit 1
    }
} else {
    & $exePath
}

Write-Host "[DONE] Run $Exe"
