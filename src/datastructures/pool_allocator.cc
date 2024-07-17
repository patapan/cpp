
#include <cstddef>
#include <list>
#include <cstring>

class PoolAllocator {
 private:
    size_t CHUNK_SIZE;
    size_t CHUNK_NUM;

    void* memory;

    std::list<void*> freelist;

 public:
    PoolAllocator(size_t CHUNK_SIZE_, size_t CHUNK_NUM_) : CHUNK_SIZE(CHUNK_SIZE_), CHUNK_NUM(CHUNK_NUM_) {
        memory = new char[CHUNK_SIZE * CHUNK_NUM];

        // initially all the memory is free.
        for (int i = 0; i < CHUNK_NUM; i++) {
            freelist.push_back(static_cast<char*>(memory) + CHUNK_SIZE * i);
        }
    }

    // Return a chunk of memory for use by the user.
    void* allocate() {
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

int main(){}