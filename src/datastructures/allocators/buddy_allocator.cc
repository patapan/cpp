#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

class BuddyAllocator {
private:
    static const int MAX_MEM = 1024; // Maximum memory size in bytes
    static const int MIN_BLOCK_SIZE = 32; // Minimum block size in bytes
    std::vector<void*> freeLists[5]; // Free lists for each block size index

    // Calculate the index for the given size
    int getIndex(size_t size) {
        int index = 0;
        size = (size > MIN_BLOCK_SIZE) ? size : MIN_BLOCK_SIZE;
        while ((1 << (index + 5)) < size) index++;
        return index;
    }

    // Splitting the block until the desired index is reached
    void* splitBlock(int index, int desiredIndex) {
        if (index == desiredIndex) {
            return freeLists[index].back();
        }
        if (freeLists[index].empty()) {
            void* block = splitBlock(index + 1, desiredIndex);
            freeLists[index].push_back(static_cast<char*>(block) + (1 << (index + 5)));
            return block;
        }
        void* block = freeLists[index].back();
        freeLists[index].pop_back();
        freeLists[index - 1].push_back(static_cast<char*>(block) + (1 << (index + 4)));
        return block;
    }

public:
    BuddyAllocator() {
        // Initialize with one block of MAX_MEM
        freeLists[4].push_back(malloc(MAX_MEM)); // Assume 1024 is 2^10, thus index 4 for 2^10/32
        assert(freeLists[4].back() != nullptr); // Ensure memory was allocated
    }

    ~BuddyAllocator() {
        // Free all allocated memory on destruction
        for (auto& list : freeLists) {
            for (auto& block : list) {
                free(block);
            }
            list.clear();
        }
    }

    void* allocate(size_t size) {
        int index = getIndex(size);
        if (freeLists[index].empty()) {
            void* block = splitBlock(index + 1, index);
            return block;
        }
        void* block = freeLists[index].back();
        freeLists[index].pop_back();
        return block;
    }

    void freeMemory(void* block, size_t size) {
        int index = getIndex(size);
        freeLists[index].push_back(block);
        // Buddy merging logic should be implemented here
    }
};

int main() {
    BuddyAllocator allocator;

    // Example of allocating and freeing memory
    void* ptr1 = allocator.allocate(64);
    std::cout << "Allocated 64 bytes at " << ptr1 << std::endl;
    allocator.freeMemory(ptr1, 64);

    return 0;
}
