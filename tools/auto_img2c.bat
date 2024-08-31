@echo off
setlocal EnableDelayedExpansion

echo 1: clear files
echo 2: run script
echo other: run script
echo.
set /p choice="Enter your choice: "

if "%choice%"=="1" (
    del /S /Q *.c
    del /S /Q *.h
) else (
    python auto_img2c.py
)

exit /b