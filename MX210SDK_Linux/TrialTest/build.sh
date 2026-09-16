#!/bin/bash

echo "=== Configuring CMake ==="
cmake -S . -B build
if [ $? -ne 0 ]; then
    echo ""
    echo "CMake configuration failed."
    exit 1
fi

echo ""
echo "=== Building ==="
cmake --build build
if [ $? -ne 0 ]; then
    echo ""
    echo "Build failed."
    exit 1
fi

echo ""
echo "----------------------------------------"
echo "Build successful!"
echo "EXE:"
echo "$(pwd)/build/MX210Test_***"
echo "----------------------------------------"
