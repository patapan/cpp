// Multi producer multi consumer lockless queue.

/*
The idea for popping / adding is that 
1. We have a dummy node that we always keep at the end of the queue.
2. Atomically move the tail pointer to a new node, then update to show the link.

*/

#include <atomic>
#include <optional>

template <typename T>
class MpmcQueue {
private:
    struct Node {
        std::optional<T> data;
        std::atomic<Node*> next;
        Node() : data(std::nullopt), next(nullptr){}
        Node(const T& data_) : data(data_), next(nullptr){}
    };
    std::atomic<Node*> head;
    std::atomic<Node*> tail;
public:
    MpmcQueue() {
        Node* dummy = new Node();
        head.store(dummy, std::memory_order_relaxed);
        tail.store(dummy, std::memory_order_relaxed);
    }

    // Push into existing tail, and make a new empty node.
    void push(const T& data) {
        Node* new_tail = new Node();
        Node* old_tail = tail.load();
        while (true) {
            auto empty_tail = old_tail->next.load();

            if (empty_tail == nullptr) {
                if (old_tail->next.compare_exchange_weak(empty_tail, new_tail)) {
                    old_tail->data = data;
                    break;
                }
            }
        }
        // If we can, update the tail now (unless something else got to it first)
        tail->compare_exchange_weak(old_tail, new_tail);
    }

    T pop() {
        Node* old_head = head.load();
        Node* tail_node = tail.load();

        while (true) {
            Node* new_head = old_head->next.load();
            if (new_head == nullptr) throw std::runtime_error("Cannot pop empty queue");
            if (head.compare_exchange_weak(old_head, new_head)) {
                T data = std::move(old_head->data);
                delete old_head;
                return data;
            }

        }
    }

    // delete copy and move semantics
};

int main(){

}