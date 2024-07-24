#include <iostream>
#include <utility>
#include <memory>

using namespace std;

class Node {
 public:
    int* data;

    Node() {}

    // Copy constructor
    Node(const Node& other_node) {
        data = new int(*other_node.data);
    }

    // Copy assignment
    Node& operator=(const Node& other_node) {
        if (this != &other_node) {
            data = new int(*other_node.data);
        }
        return *this;
    }

    // Move constructor
    Node(Node&& other_node) noexcept {
        delete data;
        data = other_node.data;
    }

    // Move assignment operator
    Node& operator=(Node&& other_node) noexcept {
        if (this != &other_node) {
            delete data;
            data = other_node.data;
            return *this;
        }
    }
};

void func(int& lvalue) {
    cout << "lvalue: " << lvalue << endl;
}

void func(int&& rvalue) {
    cout << "rvalue: " << rvalue << endl;
}


template <typename T>
void forwarder(T&& val) {
    return func(std::forward<T>(val));
}

int main() {
    int lvalue = 10;
    // func(lvalue); // l value
    // func(20); // r value
    // func(std::move(lvalue)); // casting l value to r value

    forwarder(lvalue);
    forwarder(20);

    Node* node = new Node();

    // Move contructed node
    Node* move_constructed_node(std::move(*node));

    // Move operator
    Node* move_operator_node;
    move_operator_node = std::move(*node);

    // Copy constructor
    Node copy_constructed_node = *node;

    // Copy assignment constructed node
    Node copy_assignment_node;
    copy_assignment_node = *node;
}