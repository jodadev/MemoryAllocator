/*
 * BlockAllocator.h
 * 
 * Author: Edwin Martinez
 * Date: 6/13/2024
 * 
 * Description:
 * This file contains the definition of the BlockAllocator class, which
 * provides fixed-size block memory allocation and deallocation.
 * 
 * License: MIT License
 */
#pragma once

#include <sstream>
#include <iostream>
#include <vector>
#include <unordered_map>

class BlockAllocator {
public:
    /// @brief Custom fixed-size block memory allocation and deallocation. 
    /// @param blockSize size per memory block, in bytes.
    /// @param blockCount count memory blocks in pool.
    BlockAllocator(size_t blockSize, size_t blockCount);
    ~BlockAllocator();

    /// @brief Custom caster which casts pointers to a new type, nullifies the current pointer and returns a new pointer of type T.
    /// @tparam T type to cast to.
    /// @param block pointer to memory block.
    /// @return new pointer to memory block of type T.
    template<class T>
    T* Cast(void*& block) {
        T* ptr = static_cast<T*>(block);
        block = nullptr;
        return ptr;
    }

    /// @brief Allocates a new block of memory from pool.
    /// @return pointer to the block of memory of type void.
    void* Allocate();

    /// @brief Allocates a new block of memory from pool using a specified value to cast its pointer to.
    /// @tparam T new type to cast pointer to.
    /// @param val Value to evaluate its type to cast pointer to.
    /// @param initialize allows the initialization of pointer to value passed as argument. False by default.
    /// @return pointer to the block of memory of type T.
    template<class T>
    T* Allocate(T val){
        if (freeBlocks.empty()) {
            std::cerr << "Out of memory!" << std::endl;
            return nullptr;
        }

        T* typedBlock = nullptr;

        // get memory block and remove it from vector
        void* block = freeBlocks.back();
        freeBlocks.pop_back();

        // insert pointer by string into lookup table
        std::string ptrStr = stringifyPointer(block); 
        inUseBlocks.insert({ptrStr, ptrStr});

        // cast
        typedBlock = new (block) T(val);

        return typedBlock;
    };

    /// @brief Returns the memory block back to pool. Handles setting pointer to null.
    /// @param pointer memory block's pointer
    void Deallocate(void*& pointer);

    /// @brief Returns the memory block back to pool. Handles deleting data in block and setting pointer to null.
    /// @tparam T
    /// @param pointer memory block's pointer
    template<class T>
    void Deallocate(T*& pointer){
        std::string ptrStr = stringifyPointer(pointer);
        if (pointer != nullptr && inUseBlocks.find(ptrStr) != inUseBlocks.end()){
            //delete pointer; // causes unexpected behavior due to the fact we are allocating/deallocating manually
            pointer->~T(); // solution is to call the data's destructor explicitly
            void* block = static_cast<void*>(pointer); // Cast back to type void
            freeBlocks.push_back(block); // place back into vector 
            inUseBlocks.erase(ptrStr); // remove from lookup table
            // handle dangling ptr - since it is ref, the caller's ptr is no longer pointing to memory block
            pointer = nullptr;
            return;
        }
        std::cerr << "Could not deallocate memory. Either pointer is null or there is a misalignment with size." << std::endl;
    };

private:
    /// @brief Size of each memory block in bytes.
    size_t blockSize;
    /// @brief Count of memory blocks in pool.
    size_t blockCount;
    /// @brief Vector of all available blocks a memory to allocate.
    std::vector<void*> freeBlocks;
    /// @brief Lookup table to track which blocks are currently being used.
    std::unordered_map<std::string, std::string> inUseBlocks;
    /// @brief Actual block of memory allocated for the entire pool of sub-blocks of memory.
    void* pool;
    /// @brief Turns a memory address into a string. Used as a helper function for the lookup table, inUseBlocks.
    /// @param ptr pointer of memory block. 
    /// @return memory address as type string.
    std::string stringifyPointer(void* ptr);
};