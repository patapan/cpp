
#include <stdexcept>
#include <iostream>

template <typename T>
class Optional {
public:
    T data;
    bool initialized; // whether data is initialized or not.

    Optional() {
        initialized = false;
    }

    //  assignment
    Optional& operator=(T other) noexcept {
        initialized = true;
        data = other;
        return *this;
    }

    T value() const {
        if (!initialized) {
            throw std::runtime_error("Optional value not initialized, yet requested");
        }
        return data;
    }

    bool has_value() const {
        return initialized;
    }

    // Copy constructor 
    Optional(const Optional& other) {
        initialized = other.initialized;
        if (initialized) data = other.data;
    } 

    // Copy assignment
    Optional& operator=(const Optional& other) {
        if (this != &other) {
            initialized = other.initialized;
            if (initialized) data = other.data;
        }
        return *this;
    }

    // Move constructor
    Optional(Optional&& other) noexcept {
        initialized = other.initialized;
        if (initialized) data = std::move(other.data);
        other.initialized = false;
    }

    // Move assignment 
    Optional& operator=(Optional&& other) noexcept {
        if (this != &other) {
            initialized = other.initialized;
            if (initialized) data = std::move(other.data);
            other.initialized = false;
        }
        return *this;
    }

};

int main() {
    // std::optional<int> a;
    int a = 10;

    Optional<int> var;

    std::cout << var.has_value() << std::endl;
    var = 10;
    std::cout << var.has_value() << std::endl;
    std::cout << var.value() << std::endl;
    
    
}