@echo off

REM Use UTF-8 encoding, halå 
chcp 65001 > nul

REM Start measuring time
for /f "tokens=1-3 delims=:." %%a in ("%time%") do set /a "start=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100"

echo ==================================
echo            Autoclicker            
echo ==================================
echo   A program by Victor Anderssén
echo ==================================


set CC=clang

echo Using %CC% as the compiler
echo.


REM Get number of CPU cores / 2
for /f "tokens=*" %%I in ('WMIC CPU Get NumberOfCores /value ^| findstr "="') do (
    for /f "delims=" %%J in ("%%I") do set "CORES=%%J"
)
set /a CORES=%CORES% / 2
echo Using %CORES% cores for compilation

REM Compile the program
echo Compiling autoclicker

%CC% ./src/win32_main.c -o autoclicker.exe -O3 -lUser32 -Wall -Wextra -Wpedantic -Werror
if %errorlevel% equ 0 (
    echo Compilation successful
    REM Run the program only if compilation is successful
    echo Running autoclicker
    autoclicker.exe
) else (
    echo Compilation failed
)

REM End measuring time
for /f "tokens=1-3 delims=:." %%a in ("%time%") do set /a "end=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100"
set /a "runtime=end-start"
echo Compilation took %runtime% milliseconds
