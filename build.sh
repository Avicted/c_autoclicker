#!/bin/bash

# Start measuring time
start=`date +%s`

echo "=================================="
echo "           Autoclicker            "
echo "=================================="
echo "  A program by Victor Anderssén"
echo "=================================="


export CC=gcc

echo "Using $CC as the compiler"
echo ""


# Get number of cores / 2
CORES=$(nproc --all)
CORES=$((CORES / 2))
echo "Using $CORES cores for compilation"

CXXFLAGS="-O3 -Wall -Wextra -Wpedantic -Werror -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function -Wno-unused-but-set-variable -Wno-unused-value -Wno-unused-local-typedefs -Wno-unused-private-field"

# Compile the program
echo "Compiling autoclicker"

$CC ./src/main.c -o autoclicker $CXXFLAGS -lX11 -lXtst && echo "Compilation successful" || echo "Compilation failed"

# End measuring time
end=`date +%s`
runtime=$((end-start))
echo "Compilation took $runtime seconds"

# Run the program
echo "Running autoclicker"
./autoclicker
