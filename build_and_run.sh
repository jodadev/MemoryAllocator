#!/bin/bash

# Create "out" directory or remove (recursively) and create the directory
if [ ! -d "out" ]; then
    mkdir out
else
    rm -r out
    mkdir out
fi

# Compile the project
g++ -o out/MemoryAllocator -std=c++17 ./src/*.cpp

# Run the program automatically if the compilation is successful
if [ -f "out/MemoryAllocator" ]; then
    ./out/MemoryAllocator
fi
