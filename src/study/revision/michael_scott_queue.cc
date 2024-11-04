#include <atomic>
#include <memory>

// Multi producer multi consumer lock free queue
// Michael Scott Queue

template <typename T>
class MichaelScottQueue {
private:
    struct Node {
        std::shared_ptr<T> data;
        std::atomic<Node*> next;
        Node(T value) : data(std::make_shared<T>(value)), next(nullptr) {}
    };

    std::atomic<Node*> head;
    std::atomic<Node*> tail;

public:
    MichaelScottQueue() {
        Node* dummy = new Node(T());
        head.store(dummy, std::memory_order_relaxed);
        tail.store(dummy, std::memory_order_relaxed);
    }

    ~MichaelScottQueue() {
        while (Node* node = head.load(std::memory_order_relaxed)) {
            head.store(node->next.load(std::memory_order_relaxed), std::memory_order_relaxed);
            delete node;
        }
    }

    void enqueue(T value) {
        Node* new_node = new Node(value);
        Node* old_tail = tail.load(std::memory_order_acquire);

        while (true) {
            Node* next = old_tail->next.load(std::memory_order_acquire);
            if (next == nullptr) {
                if (old_tail->next.compare_exchange_weak(next, new_node, std::memory_order_release, std::memory_order_relaxed)) {
                    break;
                }
            } else {
                tail.compare_exchange_weak(old_tail, next, std::memory_order_release, std::memory_order_relaxed);
            }
        }

        tail.compare_exchange_weak(old_tail, new_node, std::memory_order_release, std::memory_order_relaxed);
    }

    std::shared_ptr<T> dequeue() {
        Node* old_head = head.load(std::memory_order_acquire);

        while (true) {
            Node* next = old_head->next.load(std::memory_order_acquire);
            if (next == nullptr) {
                return std::shared_ptr<T>(); // Queue is empty
            }

            if (head.compare_exchange_weak(old_head, next, std::memory_order_release, std::memory_order_relaxed)) {
                return next->data;
            }
        }
    }

    // Delete copy constructor and copy assignment operator
    MichaelScottQueue(const MichaelScottQueue& other) = delete;
    MichaelScottQueue& operator=(const MichaelScottQueue& other) = delete;
};

int main() {
    MichaelScottQueue<int> queue;
    queue.enqueue(1);
    queue.enqueue(2);
    queue.enqueue(3);

    std::shared_ptr<int> value1 = queue.dequeue();
    std::shared_ptr<int> value2 = queue.dequeue();
    std::shared_ptr<int> value3 = queue.dequeue();

    if (value1) std::cout << *value1 << std::endl;
    if (value2) std::cout << *value2 << std::endl;
    if (value3) std::cout << *value3 << std::endl;

    return 0;
}
