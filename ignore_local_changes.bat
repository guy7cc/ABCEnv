@echo off
echo Disabling tracking for local changes to main.cpp and in.txt
echo (They must be committed to the repository first for this to take effect)

git update-index --skip-worktree main.cpp
git update-index --skip-worktree in.txt

if %errorlevel% neq 0 (
    echo [!ERROR!] Git is not initialized, or the files are not yet committed.
    echo Please run "git add main.cpp in.txt" and "git commit -m initial" first!
) else (
    echo Local changes to main.cpp and in.txt will now be completely ignored by Git.
)
pause
