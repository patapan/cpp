// Multi producer multi consumer lockless queue.

/*
The idea for popping / adding is that 
1. We have a dummy node that we always keep at the end of the queue.
2. Atomically move the tail pointer to a new node, then update to show the link.

*/

#include <atomic>
#include <optional>
#include <thread>
#include <iostream>
#include <chrono>

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
    std::atomic<size_t> size;
public:
    MpmcQueue() : size(0) {
        Node* dummy = new Node();
        head.store(dummy, std::memory_order_relaxed);
        tail.store(dummy, std::memory_order_relaxed);
    }

    ~MpmcQueue() {
        while (Node* node = head.load(std::memory_order_relaxed)) {
            head.store(node->next.load(std::memory_order_relaxed), std::memory_order_relaxed);
            delete node;
        }
    }

    // Push into existing tail, and make a new empty node.
    void push(const T& data) {
        Node* new_tail = new Node();
        Node* old_tail = tail.load(std::memory_order_acquire);
        while (true) {
            Node* empty_tail = old_tail->next.load(std::memory_order_acquire);
            if (empty_tail == nullptr) {
                if (old_tail->next.compare_exchange_weak(empty_tail, new_tail)) {
                    old_tail->data = data;
                    size.fetch_add(1);
                    break;
                }
            }
        }
        // If we can, update the tail now (unless something else got to it first)
        tail.compare_exchange_weak(old_tail, new_tail);
    }

    // Keep trying to move the head to the next value. When you suceed, return that data.
    std::optional<T> pop() {
        Node* old_head = head.load(std::memory_order_acquire);
        Node* tail_node = tail.load(std::memory_order_acquire);

        while (true) {
            Node* new_head = old_head->next.load(std::memory_order_acquire);
            if (new_head == nullptr) return std::nullopt;
            if (head.compare_exchange_weak(old_head, new_head)) {
                T data = std::move(old_head->data.value());
                delete old_head;
                size.fetch_sub(1);
                return data;
            }
        }
    }

    size_t get_size() {
        return size;
    }

    // delete copy and move semantics
    MpmcQueue(const MpmcQueue& other) = delete;
    MpmcQueue& operator=(const MpmcQueue& other) = delete;
    MpmcQueue(MpmcQueue&& other) noexcept = delete;
    MpmcQueue& operator=(MpmcQueue&& other) noexcept = delete;
};

void producer(MpmcQueue<int>& mpmc) {
    for (int i = 0; i < 10; i += 1) {
        std::cout << "Pushing " << i << " to the queue" << std::endl;
        mpmc.push(i);
    }
}

void consumer(MpmcQueue<int>& mpmc) {
    for (int i = 0; i < 20; i ++) {
        // std::cout << mpmc.pop() << std::endl;
    }
}

// int main(){
//     MpmcQueue<int> mpmc;

//     std::vector<std::jthread> workers;

//     for (int i = 0; i < 2; i++) {
//         workers.push_back(std::jthread{producer, std::ref(mpmc)});
//     }

//     std::this_thread::sleep_for(std::chrono::seconds(1));

//     for (int i = 0; i < 1; i++) {
//         workers.push_back(std::jthread(consumer, std::ref(mpmc)));
//     }    
// }