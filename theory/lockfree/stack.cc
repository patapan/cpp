// Lockfree stack
// Lockfree->systemwide progress
// Waitfree->every thread is making progress

#include <atomic>

template <typename T>
class Stack {
 private:
    struct Node {
        T* data;
        Node* next;
        atomic<int> count; // count to know when node can be safely deleted

        Node(T* const data) : data(data), count(0), next(nullptr) {}
    };

 public:
    std::atomic<Node*> head;

    Stack() : head(nullptr) {} 

    ~Stack() {
        Node node;
        while (node = pop()) {
        }
    }

    Node* get_head() {
        return head.load(std::memory_order_acquire);
    }

    void push(const T& data) {
        Node* node = new Node(data);
        node->next = head.load(std::memory_order_acquire);
        // keep trying to set head
        while(!head.compare_exchange_weak(node->next,node));
    }

    T* pop() {
        // we never want 2 different threads to pop and return the same node
        Node* old_head = head.load(std::memory_order_acquire);
        while (old_head && head.compare_exchange_weak(old_head,old_head->next));
        return old_head->data;
        delete old_head; // this is now the issue
    }

    // copy constructor
    Stack(const Stack& other) = delete;
    // copy assignment
    Stack& operator=(const Stack& other) = delete;
};

int main() {

}