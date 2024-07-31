
#include <atomic>
#include <memory>
#include <iostream>

// Lock free single producer, single consumer  Spsc
// Not wait free as we dynamically allocate memory which can block.

template <typename T> 
class  Spsc {
 private:
    struct Node {
        std::shared_ptr<T> data;
        Node* next;
        Node() : next(nullptr) {}
    };
    std::atomic<Node*> head;
    std::atomic<Node*> tail;

    void release() {
        while (Node* node = head.load(std::memory_order_acquire)) {
            head.store(node->next, std::memory_order_release);
            delete node;
        }
    }

 public:    
     Spsc() {
        // initialize empty Spsc
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

    ~Spsc() {
        release();
    }

    // Delete copy constructor and copy assignment operators 
     Spsc(const  Spsc& other) = delete;
     Spsc& operator=(const  Spsc& other) = delete;

    // Move constructors
     Spsc( Spsc&& other) noexcept {
        head.store(other.head.load());
        other.head.store(nullptr);
        tail.store(other.tail.load());
        other.tail.store(nullptr);
    }

     Spsc& operator=(Spsc&& other) noexcept {
        if (this != &other) {
            release();
            head.store(other.head.load());
            other.head.store(nullptr);
            tail.store(other.tail.load());
            other.tail.store(nullptr);
        }
    }
};

int main(){
    int a = 1;
    int b = 2;
    Spsc<int> q;
    q.push(a);
    q.push(b);
    std::cout << *(q.pop()) << " " << *(q.pop()) << std::endl;
}