#include <atomic>

// https://www.codeproject.com/Articles/43510/Lock-Free-Single-Producer-Single-Consumer-Circular#heading_code_explained

// Wait free SCSP FIFO.

/*
Benefits of a ringbuffer
1. We don't have to deallocate memory.
2. 
*/

template<typename T, size_t Size>
class CircularFifo {
public:
    enum { Capacity = Size+1 };

    CircularFifo() : _tail(0), _head(0) {}
    virtual ~CircularFifo() {}

    // push to tail
    bool push(const T& item) {
        auto current_tail = _tail.load(std::memory_order_relaxed);
        auto next_tail = increment(current_tail);
        if(next_tail != _head.load(std::memory_order_acquire)) {
            _array[current_tail] = item;
            _tail.store(next_tail, std::memory_order_release);
            return true;
        }
        return false; // full queue
    }

    // pop from head into item.
    bool pop(T& item) {
        const auto current_head = _head.load(std::memory_order_relaxed);
        if(current_head == _tail.load(std::memory_order_acquire))
            return false; // empty queue

        item = _array[current_head];
        _head.store(increment(current_head), std::memory_order_release);
        return true;
    }

    // Empty if head == tail.
    bool wasEmpty() const {
        return (_head.load() == _tail.load());
    }

    // Full if the next tail is the head.
    bool wasFull() const {
        const auto next_tail = increment(_tail.load());
        return (next_tail == _head.load());
    }

    bool isLockFree() const {
        return (_tail.is_lock_free() && _head.is_lock_free());
    }

private:
    size_t increment(size_t idx) const {
        return (idx + 1) % Capacity;
    }

    std::atomic<size_t> _tail;
    T _array[Capacity];
    std::atomic<size_t> _head;
};

int main(){}