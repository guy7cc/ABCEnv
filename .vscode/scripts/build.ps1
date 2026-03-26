# .vscode/scripts/build.ps1
param(
    [string]$Source = "submit.cpp",
    [string]$Output = "submit.exe"
)

$cppFile = Join-Path $PSScriptRoot "..\..\$Source"
$exeFile = Join-Path $PSScriptRoot "..\..\$Output"
$hashFile = Join-Path $PSScriptRoot "submit.cpp.hash"
$gpp = Join-Path $PSScriptRoot "..\..\mingw64\bin\g++.exe"
$acLib = Join-Path $PSScriptRoot "..\..\ac-library"
$myLib = Join-Path $PSScriptRoot "..\..\my-library"

Write-Host "[START] Build $Source"

if (Test-Path $cppFile) {
    $currentHash = (Get-FileHash $cppFile -Algorithm SHA256).Hash
    if (Test-Path $exeFile) {
        if (Test-Path $hashFile) {
            $oldHash = Get-Content $hashFile
            if ($currentHash -eq $oldHash) {
                Write-Host "[SKIP] $Source is unchanged. Skipping build."
                Write-Host "[DONE] Build $Source"
                exit 0
            }
        }
    }

    # Compilation
    & $gpp -fdiagnostics-color=always -std=c++23 -O2 -static -I $acLib -I $myLib $cppFile -o $exeFile

    if ($LASTEXITCODE -eq 0) {
        $currentHash | Out-File -FilePath $hashFile -Encoding utf8
        Write-Host "[DONE] Build $Source"
    } else {
        Write-Error "Compilation failed."
        exit $LASTEXITCODE
    }
} else {
    Write-Error "Source file $Source not found."
    exit 1
}
