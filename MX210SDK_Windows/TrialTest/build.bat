@echo off
setlocal

echo === Configuring CMake ===
cmake -S . -B build

if errorlevel 1 (
    echo.
    echo CMake configuration failed.
    pause
    exit /b 1
)

echo.
echo === Building ===
cmake --build build

if errorlevel 1 (
    echo.
    echo Build failed.
    pause
    exit /b 1
)

echo.

set "EXE_PATH=%CD%\build\Debug\MX210Test_**.exe"

echo.
echo ========================================
echo Build successful!
echo EXE:
echo %EXE_PATH%
echo ========================================
echo.

pause