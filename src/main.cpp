#include "block_allocator.h"
#include <iostream>


int main() {
    const size_t blockSize = sizeof(int); // Size of each block in bytes
    const size_t blockCount = 10; // Number of blocks (total bytes, 40)
    int x = 100;

    BlockAllocator allocator(blockSize, blockCount);

    // Allocate block - uninitialized and of type void memory block returns
    void* block1 = allocator.Allocate();
    if (block1 != nullptr){
        std::cout << "Allocated block1: " << block1 << std::endl;
        /* Method: using an uninitialized type void block of memory..
            we cast it to type int(32) then assign the value of x to it.

            Note: We use the custom Cast function of BlockAllocator to ensure we handle the "block1" pointer
            from dangling.
        */
        int* intPtr = allocator.Cast<int>(block1);
        *intPtr = x;
        std::cout << "Casted block1 to type int, saved to intPtr and set value to: " << *intPtr << std::endl;
        std::cout << "block1 pointer is now pointing to: " << block1 << std::endl;

        // Deallocate - returns block back to pool, deletes data and nullify pointer
        allocator.Deallocate(intPtr);
        std::cout << "Deallocated block1(intPtr), now pointing to: " << intPtr << std::endl;
        
    }

    // Method: returns an initialized memory block of type int.
    int* agePtr = allocator.Allocate((int)32);
    std::cout << "Allocated agePtr: " << agePtr << " value set to " << *agePtr << std::endl;
    allocator.Deallocate(agePtr);
    std::cout << "Deallocated agePtr, now pointing to: " << agePtr << std::endl;

    // Just a block without initializing or casting type
    void* block4 = allocator.Allocate();
    std::cout << "Allocated block4: " << block4 << std::endl;
    allocator.Deallocate(block4);
    std::cout << "Deallocated block4, now pointing to: " << block4 << std::endl;
    
    system("pause");

    return 0;
}