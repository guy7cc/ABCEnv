@echo off
echo Starting environment setup...
powershell.exe -ExecutionPolicy Bypass -File "%~dp0setup_env.ps1"
if %errorlevel% neq 0 (
    echo Setup failed with error code %errorlevel%.
) else (
    echo Setup finished successfully.
)
pause
