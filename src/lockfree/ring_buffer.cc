#include <atomic>

// https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular#heading_code_explained

// Wait free SCSP FIFO.

/*
Benefits of a ringbuffer
1. We don't have to deallocate memory.
2. 
*/

constexpr size_t BUFFER_SIZE = 1024; // 1024 items

template<typename T, size_t Size>
class RingBuffer {
public:
    RingBuffer() : tail(0), head(0) {}
    virtual ~RingBuffer() {}

    // push to tail
    bool push(const T& item) {
        auto current_tail = tail.load(std::memory_order_relaxed);
        auto next_tail = increment(current_tail);
        if(next_tail != head.load(std::memory_order_acquire)) {
            buffer[current_tail] = item;
            tail.store(next_tail, std::memory_order_release);
            return true;
        }
        return false; // full queue
    }

    // pop from head into item.
    bool pop(T& item) {
        const auto current_head = head.load(std::memory_order_relaxed);
        if(current_head == tail.load(std::memory_order_acquire))
            return false; // empty queue

        item = buffer[current_head];
        head.store(increment(current_head), std::memory_order_release);
        return true;
    }

    // Empty if head == tail.
    bool wasEmpty() const {
        return (head.load() == tail.load());
    }

    // Full if the next tail is the head.
    bool wasFull() const {
        const auto next_tail = increment(tail.load());
        return (next_tail == head.load());
    }

private:
    size_t increment(size_t idx) const {
        return (idx + 1) % BUFFER_SIZE;
    }

    std::atomic<size_t> tail;
    T buffer[BUFFER_SIZE];
    std::atomic<size_t> head;
};

int main(){}