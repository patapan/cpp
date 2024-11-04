
/*
A ring buffer is a queue which utilizes pre allocated chunks of memory to reduce latency. 

*/

#include <atomic>
#include <memory>
#include <array>
#include <iostream>

constexpr size_t BUFFER_SIZE = 1024;

template <typename T>
class RingBuffer {
    private:
        std::atomic<size_t> head;
        std::atomic<size_t> tail;
        T buffer[BUFFER_SIZE];

    public:
        RingBuffer() {
            // allocate buffer
            head.store(0);
            tail.store(0);
        }

        // push onto the tail
        bool push(T& value) {
            int tail_idx = tail.load();
            int head_idx = head.load();

            if ((tail_idx + 1) % BUFFER_SIZE == head_idx) {
                // we are full
                return false;
            } else {
                buffer[tail_idx] = std::move(value);
                tail.store((tail_idx + 1) % BUFFER_SIZE);
                return true;
            }
        }

        // pop from the head
        T pop() {
            int old_head_idx = head.load();
            if (old_head_idx == tail.load()) {
                // empty
                throw std::runtime_error("Trying to pop an empty queue.");
            }

            head.store((old_head_idx + 1) % BUFFER_SIZE);
            return buffer[old_head_idx];
        }
};

int main() {
    RingBuffer<int> rb;

    rb.push(10);
    rb.push(11);

    std::cout << rb.pop() << std::endl;
    std::cout << rb.pop() << std::endl;
    return 0;
}