
#include <iostream>

class Example {
public:
    // Explicit constructor
    explicit Example(int value) : value(value) {
        std::cout << "Explicit constructor called" << std::endl;
    }

    int getValue() const {
        return value;
    }

private:
    int value;
};

int main() {
    Example ex(42);  // OK: Direct initialization
    // Example ex2 = 42;  // Error: Implicit conversion not allowed

    return 0;
}
