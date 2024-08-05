/*
Move is a casting operator which casts an L-value to an X-value (expiring ref)

*/

#include <iostream>

// Empty primary case
template <typename T>
struct remove_reference;

// Implement remove_reference for L refs
template <typename T>
struct remove_reference<T&> {
    using type = T;
};

// Implement remove_reference for R refs
template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

// Implement std::move
template <typename T>
remove_reference<T>::type&& move(T&& ref) {
    return static_cast<remove_reference<T>::type&&>(ref);
}

// Now let's see it in action

struct Foo {
    std::string name;

    Foo(std::string name) : name(name){}

    // move constructor
    Foo(Foo&& other) noexcept {
        name = other.name;
        other.name = "";
    }

    friend std::ostream& operator<<(std::ostream& stream, const Foo& foo) {
        stream << foo.name;
        return stream;
    }
};

int main() {
    Foo x("500");

    Foo y = move(x);

    std::cout << x << std::endl;
    std::cout << y << std::endl;
}