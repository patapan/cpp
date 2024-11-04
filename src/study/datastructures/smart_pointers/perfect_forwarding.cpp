#include <iostream>
#include <utility>

// L value function
void func(int& x) {
    // lvalue
    std::cout << "L value\n";
}

// R value function
void func(int&& x) {
    std::cout << "R value\n";
}

template <typename T>
void forwarder(T&& t) {
    return func(std::forward<T>(t));
}

int main() {

    int x = 0;

    // passes as L-value
    forwarder(x);
    // passes as R-value;
    forwarder(2);

}