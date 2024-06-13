@echo off

rem Create "out" directory or remove (recursively) and create the directory
if not exist out (
    mkdir out
) else (
    rm -r out
    mkdir out
)

rem Compile the project
g++ -o out/MemoryAllocator.exe -std=c++17 ./src/*.cpp

rem Run the program automatically if the compilation is successful
if exist out/MemoryAllocator.exe (
    start ./out/MemoryAllocator.exe
)