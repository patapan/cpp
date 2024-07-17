
#include <list>

/*
Implements a simple pool allocator with a free list.
- Memory is pre-allocated on construction to optimize future allocations.
- Freelist is implemented as a linked list, initially set to full.
*/

class PoolAllocator {
 private:
    void* memory;
    std::list<void*> freelist;

 public:
    size_t CHUNK_SIZE;
    size_t CHUNK_NUM;

    PoolAllocator(const size_t CHUNK_SIZE_, const size_t CHUNK_NUM_) : CHUNK_SIZE(CHUNK_SIZE_), CHUNK_NUM(CHUNK_NUM_) {
        memory = new char[CHUNK_SIZE * CHUNK_NUM];

        // initially all the memory is free.
        for (int i = 0; i < CHUNK_NUM; i++) {
            freelist.push_back(static_cast<char*>(memory) + CHUNK_SIZE * i);
        }
    }

    // Return a chunk of memory for use by the user.
    void* allocate() {
        if (freelist.size() == 0) return nullptr;

        // use head of freelist.
        void* chunk = freelist.front();
        freelist.pop_front();
        return chunk;
    }

    void deallocate(void* chunk) {
        // Clean up
        memset(chunk, 0, CHUNK_SIZE);
        freelist.push_back(chunk);
    }

    ~PoolAllocator() {
        delete[] static_cast<char*>(memory);
    }
};

/*
An abstraction on top of Pool Allocator to further optimize memory alignment.
We have various pool sizes and fit the data to the most appropriate bucket.
*/
class MultiPoolAllocator {
 private:
    std::vector<PoolAllocator> pools;
    size_t CHUNK_NUM;

    // Get the appropriate pool for this sized chunk.
    PoolAllocator& get_pool(size_t size) {
        return *(std::upper_bound(pools.begin(), pools.end(), size, [](size_t size, const PoolAllocator& pool){ return size > pool.CHUNK_SIZE; }));
    }

 public:
    MultiPoolAllocator(std::vector<size_t>& chunk_sizes, size_t CHUNK_NUM_) : CHUNK_NUM(CHUNK_NUM_) {
        pools.reserve(chunk_sizes.size());

        for (size_t chunk_size : chunk_sizes) {
            pools.push_back(PoolAllocator(chunk_size, CHUNK_NUM));
        }
    }

    // Allocate memory in the smallest pool that fits the size.
    void* allocate(size_t size) {
        PoolAllocator pool = get_pool(size);
        return pool.allocate();
    }

    // Note size must be the original size the user requested, not the pool size.
    void deallocate(void* chunk, size_t size) {
        PoolAllocator pool = get_pool(size);
        pool.deallocate(chunk);
    }
};

int main(){}