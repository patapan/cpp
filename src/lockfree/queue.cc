
#include <atomic>
#include <memory>
#include <iostream>

// single producer, single consumer queue
// Not wait free as we dynamically allocate memory which can block.

template <typename T> 
class Queue {
 private:
    struct Node {
        std::shared_ptr<T> data;
        Node* next;
        Node() : next(nullptr) {}
    };
    std::atomic<Node*> head;
    std::atomic<Node*> tail;
 public:    
    Queue() {
        // initialize empty queue
        head.store(new Node(), std::memory_order_relaxed);
        tail.store(head.load(std::memory_order_relaxed));
    }

    void push(T data) {
        Node* new_node = new Node();
        Node* old_tail = tail.load(std::memory_order_acquire);
        old_tail->data = std::make_shared<T>(data);
        old_tail->next = new_node;
        tail.store(new_node, std::memory_order_release);
    }

    // pop head
    std::shared_ptr<T> pop() {
        Node* node = head.load(std::memory_order_acquire);
        if (node == tail.load(std::memory_order_acquire)) {
            return std::shared_ptr<T>();
        }
        head.store(node->next, std::memory_order_release);
        auto data = node->data;
        delete node;
        return data;
    }

    ~Queue() {
        while (Node* node = head.load(std::memory_order_acquire)) {
            head.store(node->next, std::memory_order_acquire);
            delete node;
        }
    }
};

int main(){
    int a = 1;
    int b = 2;
    Queue<int> q;
    q.push(a);
    q.push(b);
    std::cout << "here\n";
    std::cout << *(q.pop()) << " " << *(q.pop()) << std::endl;
    std::cout << 
}