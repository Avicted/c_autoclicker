#!/bin/bash

# Start measuring time
start=$(date +%s%N)

echo "=================================="
echo "           Autoclicker            "
echo "=================================="
echo "   A program by Victor Anderssén"
echo "=================================="

CC=x86_64-w64-mingw32-gcc

echo "Using $CC as the compiler"

# Get number of CPU cores / 2
CORES=$(grep -c '^processor' /proc/cpuinfo)
CORES=$((CORES / 2))
echo "Using $CORES cores for compilation"

# Compile the program
echo "Compiling autoclicker"
$CC -I/usr/x86_64-w64-mingw32/include ./src/win32_main.c -o autoclicker.exe -O3 -mwindows -Wall -Wextra -Wpedantic -Werror
if [ $? -eq 0 ]; then
    echo "Compilation successful"

    # End measuring time
    end=$(date +%s%N)
    runtime=$((end-start))
    echo "Compilation took $((runtime/1000000)) milliseconds"

    # Run the program
    echo "Running autoclicker"
    wine autoclicker.exe
else
    # End measuring time
    end=$(date +%s%N)
    runtime=$((end-start))
    echo "Compilation took $((runtime/1000000)) milliseconds"

    echo "Compilation failed"
fi
