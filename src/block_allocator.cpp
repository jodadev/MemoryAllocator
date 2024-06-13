/*
 * BlockAllocator.cpp
 * 
 * Author: Edwin Martinez
 * Date: 6/13/2024
 * 
 * Description:
 * This file contains the implementation of the BlockAllocator class,
 * which provides fixed-size block memory allocation and deallocation.
 * 
 * License: MIT License
 */
#include "block_allocator.h"

BlockAllocator::BlockAllocator(size_t blockSize, size_t blockCount)
    : blockSize(blockSize), blockCount(blockCount){
        /* Allocate a large block of memory for pool
            using the block size to represent each memory block's byte size, we multiply
            that by how may blocks will exist in the pool - both size and count defined 
            by user in main.

            - The use of scope :: is to ensure we are using the global scope of new 
            and not any overloaded version of it.

            - The memory can be used for any type data so long as you cast it for it. 

        */
        pool = ::operator new(blockSize * blockCount);
        // Init the free blocks list based on blockCount - we will save pointers to each block here
        for(size_t i = 0; i < blockCount; ++i){   
            /* Understanding of what's going on...
                Each iteration adds a pointer to each block of memory in the pool to the freeBlocks vector. By 
                iterating over all blocks, this ensures that all blocks are initially marked as 
                free and can be allocated later as needed. The static_cast ensures correct pointer 
                arithmetic, while the multiplication by blockSize computes the correct memory address 
                for each block.

                - static_cast<char*>(pool) - converts the void* to char* pointers in the pool.
                - We use char* to allow pointer arithmetic which allows us to move the pointer by a specific 
                number of bytes.
                - "+ i * blockSize" This adds the calculated offset to the base address of the pool(blockSize).
                The result is a pointer to the beginning of the i-th block within the pool.
                - we push back to freeBlocks - this adds the pointer to the i-th block to the freeBlocks 
                vector. The freeBlocks vector maintains a list of pointers to all free memory blocks 
                in the pool. When allocating, we get a pointer from free blocks and use it to store memory.
            */
            freeBlocks.push_back(static_cast<char*>(pool) + i * blockSize);
        }
        // Look up table
        inUseBlocks = {};
}

BlockAllocator::~BlockAllocator(){
    // Deallocate the entire block of memory
    // since we used global operator to allocate(new) we must do the same to deallocate(delete) 
    ::operator delete(pool);
}

std::string BlockAllocator::stringifyPointer(void* ptr){
    std::stringstream ss;
    ss << ptr;
    return ss.str();
}

void* BlockAllocator::Allocate(){
    if (freeBlocks.empty()) {
        std::cerr << "Out of memory!" << std::endl;
        return nullptr;
    }
    
    void* block = freeBlocks.back();
    
    std::string ptrStr = stringifyPointer(block);
    inUseBlocks.insert({ptrStr, ptrStr});

    freeBlocks.pop_back();
    return block;
}

void BlockAllocator::Deallocate(void*& pointer){
    std::string ptrStr = stringifyPointer(pointer);
    if (pointer != nullptr && inUseBlocks.find(ptrStr) != inUseBlocks.end()){
        freeBlocks.push_back(pointer); // place back into vector 
        inUseBlocks.erase(ptrStr); // remove from lookup table
        // handle dangling ptr - since it is ref, the caller's ptr is no longer pointing to memory block
        pointer = nullptr;
        return;
    }
    std::cerr << "Could not deallocate memory. Either pointer is null or there is a misalignment with size." << std::endl;
}