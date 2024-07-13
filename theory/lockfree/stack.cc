// Lockfree stack
// Lockfree->systemwide progress
// Waitfree->every thread is making progress

#include <atomic>
#include <list>

template <typename T>
class Stack {
 private:
    struct Node {
        T data;
        Node* next;

        Node(T const data) : data(data) {}
    };

 public:
    std::atomic<Node*> head;

    Node* get_head() {
        return head.load(std::memory_order_acquire);
    }

    void push(const T& data) {
        Node* node = new Node(data);
        node->next = head;
        // keep trying to set head
        while(!head.compare_exchange_weak(node->next,node));
    }

    Node*
};

int main() {

}