@echo off
setlocal

set SCRIPT_DIR=%~dp0
set PYTHON=python
set MCU_PORT=COM6

echo [1/3] Starting meter simulator...
start "Smart Load Meter Sim" cmd /k %PYTHON% "%SCRIPT_DIR%smartload_meter_sim.py"

timeout /t 1 > nul

echo [2/3] Starting GUI sender...
start "Smart Load GUI" cmd /k %PYTHON% "%SCRIPT_DIR%smartload_gui.py"

timeout /t 1 > nul

echo [3/3] Starting MCU simulator on %MCU_PORT%...
start "Smart Load MCU Sim" cmd /k %PYTHON% "%SCRIPT_DIR%smartload_mcu_sim.py" --port %MCU_PORT%

echo.
echo Demo processes started.
echo - Meter simulator: TCP 127.0.0.1:9101
echo - GUI sender: updates meter simulator values
echo - MCU simulator: sends frames to %MCU_PORT%
echo - Hercules: open the paired port (example COM5)
echo.
pause
