# .vscode/scripts/test.ps1
param(
    [string]$TestPattern = "my-library/test/test_*.cpp"
)

$wsRoot = Join-Path $PSScriptRoot "..\.."
$gpp = Join-Path $wsRoot "mingw64\bin\g++.exe"
$acLib = Join-Path $wsRoot "ac-library"
$myLib = Join-Path $wsRoot "my-library"

Write-Host "[START] 4. Run All Tests"

$testFiles = Get-ChildItem (Join-Path $wsRoot $TestPattern)
$fail = $false

foreach ($file in $testFiles) {
    $outExe = $file.FullName -replace '\.cpp$', '.exe'
    Write-Host ""
    Write-Host "---- $($file.BaseName) ----"

    # Compile
    & $gpp -std=c++23 -O2 -static -I $acLib -I $myLib $file.FullName -o $outExe
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Compilation of $($file.Name) failed."
        $fail = $true
        continue
    }

    # Run
    & $outExe
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Test $($file.BaseName) FAILED." -ForegroundColor Red
        $fail = $true
    }
}

if ($fail) {
    Write-Host ""
    Write-Host "[FAILED] Some tests encountered an error!" -ForegroundColor Red
    exit 1
} else {
    Write-Host ""
    Write-Host "[OK] All tests ran successfully!" -ForegroundColor Green
    Write-Host "[DONE] 4. Run All Tests"
}
