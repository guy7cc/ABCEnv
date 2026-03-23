$ErrorActionPreference = "Stop"

$workspaceDir = $PSScriptRoot
if (-not $workspaceDir) {
    $workspaceDir = (Get-Location).Path
}

Write-Host "Setting up AtCoder C++23 Environment in $workspaceDir..." -ForegroundColor Cyan

# 1. Download and Extract WinLibs GCC 15.2.0
$mingwDir = Join-Path $workspaceDir "mingw64"
if (-not (Test-Path "$mingwDir\bin\g++.exe")) {
    Write-Host "Downloading WinLibs GCC 15.2.0..." -ForegroundColor Yellow
    $winlibsUrl = "https://github.com/brechtsanders/winlibs_mingw/releases/download/15.2.0posix-13.0.0-msvcrt-r6/winlibs-x86_64-posix-seh-gcc-15.2.0-mingw-w64msvcrt-13.0.0-r6.zip"
    $zipPath = Join-Path $workspaceDir "winlibs.zip"
    try {
        Invoke-WebRequest -Uri $winlibsUrl -OutFile $zipPath
        Write-Host "Extracting WinLibs GCC (this might take a while)..." -ForegroundColor Yellow
        Expand-Archive -Path $zipPath -DestinationPath $workspaceDir -Force
    } finally {
        if (Test-Path $zipPath) { Remove-Item $zipPath }
    }
    Write-Host "GCC extraction completed." -ForegroundColor Green
} else {
    Write-Host "GCC already exists in $mingwDir" -ForegroundColor Green
}

# 2. Download and Extract ac-library
$aclibDir = Join-Path $workspaceDir "ac-library"
if (-not (Test-Path $aclibDir)) {
    Write-Host "Downloading AtCoder Library (ac-library)..." -ForegroundColor Yellow
    $aclibUrl = "https://github.com/atcoder/ac-library/archive/refs/heads/master.zip"
    $aclibZipPath = Join-Path $workspaceDir "aclib.zip"
    try {
        Invoke-WebRequest -Uri $aclibUrl -OutFile $aclibZipPath
        Write-Host "Extracting ac-library..." -ForegroundColor Yellow
        Expand-Archive -Path $aclibZipPath -DestinationPath $workspaceDir -Force
        Rename-Item (Join-Path $workspaceDir "ac-library-master") "ac-library"
    } finally {
        if (Test-Path $aclibZipPath) { Remove-Item $aclibZipPath }
    }
    Write-Host "ac-library extraction completed." -ForegroundColor Green
} else {
    Write-Host "ac-library already exists in $aclibDir" -ForegroundColor Green
}

# 3. Download and Extract Portable Python 3
$pythonDir = Join-Path $workspaceDir "python3"
if (-not (Test-Path "$pythonDir\python.exe")) {
    Write-Host "Downloading Portable Python 3..." -ForegroundColor Yellow
    $pythonUrl = "https://www.python.org/ftp/python/3.12.3/python-3.12.3-embed-amd64.zip"
    $pyZipPath = Join-Path $workspaceDir "python.zip"
    try {
        Invoke-WebRequest -Uri $pythonUrl -OutFile $pyZipPath
        Write-Host "Extracting Python..." -ForegroundColor Yellow
        New-Item -ItemType Directory -Path $pythonDir -Force | Out-Null
        Expand-Archive -Path $pyZipPath -DestinationPath $pythonDir -Force
    } finally {
        if (Test-Path $pyZipPath) { Remove-Item $pyZipPath }
    }
    Write-Host "Python extraction completed." -ForegroundColor Green
} else {
    Write-Host "Portable Python already exists in $pythonDir" -ForegroundColor Green
}

# End of setup
