// Mutually exclusive thread safe lock.
// Spinlocks are not blocking meaning that they consume CPU resources while waiting.

#include <atomic>

class SpinLock {
 private:
    std::atomic_flag flag;

 public:
    SpinLock() {
        flag.clear();
    }

    void lock() {
        while (flag.test_and_set(std::memory_order_acquire)) {
         //busy wait   
        }
    }

    void unlock() {
        flag.clear(std::memory_order_release);
    }
};

int main() {
    return 0;
}