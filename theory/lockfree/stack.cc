// Lockfree stack

#include <atomic>
#include <thread>

template <typename T>
class Stack {
    T* head;
    std::atomic_bool flag;
    
    Stack() {
        flag.clear();
    }

    T* get_head() {
        return head;
    }

    bool push(T* data) {
        while(!flag.test_and_set(std::memory_order_seq_cst)) {
            ;
        }
    }
};

int main() {

}