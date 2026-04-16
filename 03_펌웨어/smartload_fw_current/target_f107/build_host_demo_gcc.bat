@echo off
setlocal

set SCRIPT_DIR=%~dp0
for %%I in ("%SCRIPT_DIR%..") do set ROOT=%%~fI
set OUTDIR=%ROOT%\target_f107\build
if not exist "%OUTDIR%" mkdir "%OUTDIR%"

gcc -std=c11 -Wall -Wextra -I"%ROOT%\inc" ^
    "%ROOT%\src\atm90e26.c" ^
    "%ROOT%\src\lcu_demo_app.c" ^
    "%ROOT%\target_f107\src\main_host_demo.c" ^
    -o "%OUTDIR%\main_host_demo.exe"

if errorlevel 1 (
    echo Build failed.
    exit /b 1
)

echo Build success.
"%OUTDIR%\main_host_demo.exe"
exit /b 0
