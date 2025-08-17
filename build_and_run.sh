#!/bin/bash
# Banglish Compiler Build and Run Script for Linux/macOS
# Equivalent to build_and_run.ps1 for Windows

set -e  # Exit on any error

echo "Banglish Compiler - Linux/macOS Build Script"
echo "=============================================="

# Create build directory
mkdir -p .generated

# Compile driver
echo "Compiling driver..."
if ! g++ -std=c++17 -O2 -o .generated/banglish_driver main.cpp; then
    echo "ERROR: Compilation failed"
    exit 1
fi

# Run driver
echo "Running driver..."
if ! ./.generated/banglish_driver; then
    echo "ERROR: Driver execution failed"
    exit 1
fi

# Compile transpiled code (if it exists)
if [ -f ".generated/transpiled.cpp" ]; then
    echo "Compiling transpiled code..."
    if ! g++ -std=c++17 -O2 -o .generated/program .generated/transpiled.cpp; then
        echo "ERROR: Transpiled code compilation failed"
        exit 1
    fi
    
    # Run program with input
    if [ -f "input.txt" ]; then
        echo "Running program with input..."
        ./.generated/program < input.txt > output.txt
        echo "Program output written to output.txt"
    else
        echo "WARNING: input.txt not found, running without input"
        ./.generated/program > output.txt
    fi
else
    echo "WARNING: No transpiled code generated"
fi

echo "Done. Check output files and .generated folder."
