// Thread Safe stack

#include <atomic>
#include <mutex>
#include <iostream>

template <typename T>
class Stack {
 private:
    std::mutex mutex;
    int length;

    struct Node {
        T data;
        Node* next;

        Node(T data) : data(data), next(nullptr) {
        }
    };

 public:
    Node* head;

    Stack() : head(nullptr), length(0) {}

    ~Stack() {
        while (size() > 0) {
            pop();
        }
    }

    T* get_head() {
        return head;
    }

    int size() {
        return length;
    }

    void push(T data) {
        std::scoped_lock lock(mutex); // lock
        Node* node = new Node(data);
        node->next = head;
        head = node;
        length++;
        // auto unlocks
    }

    T pop() {
        std::scoped_lock lock(mutex); //lock
        Node* tmp = head;
        head = head->next;
        T data = tmp->data;
        delete tmp;
        length--;
        return data;
    }

    // copy constructor
    Stack(const Stack& other) = delete;
    // copy assignment
    Stack& operator=(const Stack& other) = delete;
};

struct Factory {
    std::string name;
    int count;
};

int main() {
    Stack<int*> stack;
    int* two = new int(2);
    int* ten = new int(10);

    stack.push(two);
    stack.push(ten);

    while (stack.size() > 0) {
        std::cout << *stack.pop() << "\n";
    }
}