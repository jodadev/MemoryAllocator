# Memory Allocator
###### by Ghost

This project implements a simple custom memory allocator in C++. The allocator provides fixed-size block memory allocation and deallocation, allowing efficient memory management for systems programming.

## Features

- Fixed-size block memory allocation
- Memory deallocation
- Type-safe memory allocation with template support

## Getting Started

### Prerequisites

- A C++ compiler (e.g., g++, clang++)

### Building the Project

#### Windows
In the main project directory, open a new shell and run the .bat file. This will compile, build and run the program.
```sh
./build_and_run.bat
```

#### Mac/Linux
In the main project directory, open a new shell and run the .sh file. This will compile, build and run the program.
```sh
chmod +x ./build_and_run.sh
./build_and_run.sh
```

## Usage

The main program demonstrates the basic usage of the custom memory allocator. It allocates and deallocates blocks of memory, showing how to use the `BlockAllocator` class.

## Visualization
This shows an example how the memory pool looks when setting the block size and block count variables at `BlockAllocator` class construction.
```
blockSize = sizeof(int) - aka 4 bytes
blockCount = 10
totalBytes = blockSize * blockCount

Memory Pool (40 total bytes)
---------------------------------------------------------
| Block 0 | Block 1 | Block 2 | ... | Block 9 |
| 4 bytes | 4 bytes | 4 bytes | ... | 4 bytes |
---------------------------------------------------------

```

### Example Code

```cpp
#include <iostream>
#include "BlockAllocator.h"

int main() {
    // some data
    int x = 32;

    // base each memory block size by some type you plan on using it for is the best approach
    const size_t blockSize = sizeof(int);
    // define how may blocks will exist in the pool - in this case a total of 40 bytes of memory will be allocated for use.
    const size_t blockCount = 10;

    BlockAllocator allocator(blockSize, blockCount);

    // returns an uninitialized memory block of type void by passing no args
    void* block2 = allocator.Allocate();
    if (block2 != nullptr){
        std::cout << "Allocated block: " << block2 << std::endl;
        allocator.Deallocate(block2);
    } else {
        std::cerr << "Memory allocation failed!" << std::endl;
    }

    // returns an initialized memory block of type int.
    int* num = allocator.Allocate((int)105);
    if (num != nullptr){
        std::cout << "Value in num: " << *num << std::endl;
        allocator.Deallocate(num);
    } else {
        std::cerr << "Memory allocation failed!" << std::endl;
    }

    // recast block3 to type int, save as typedBlock. block3 set to nullptr
    void* block3 = allocator.Allocate();
    int* typedBlock = allocator.Cast<int>(block3);

    return 0;
}
```

## License

Feel free to modify the content to fit your project's specifics and requirements - [License](/LICENSE).