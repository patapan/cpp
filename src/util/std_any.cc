
/*
Let's implement std::any.

Any is a class which can hold any value in a type-safe manner.

We can use type_index and type_id to keep track of the type we have.
type_id() returns a type_index object.

We need to use a base and derived class to erase the type.
*/


#include <iostream>
#include <memory>
#include <typeindex>
#include <vector>
#include <string>

class Any {
private:
    struct Base {
        virtual ~Base() = default;
        virtual std::unique_ptr<Base> clone() const = 0;
    };

    template<typename T>
    struct Derived : Base {
        T value;

        Derived(const T& value) : value(value) {}

        std::unique_ptr<Base> clone() const override {
            return std::make_unique<Derived<T>>(value);
        }
    };

    std::unique_ptr<Base> clone() const {
        if (ptr) {
            return ptr->clone();
        }
        return nullptr;
    }

    std::unique_ptr<Base> ptr;
    std::type_index typeIndex;
public:
    Any() : typeIndex(typeid(void)) {}

    Any(const Any& other) : ptr(other.clone()), typeIndex(other.typeIndex) {}

    Any(Any&& other) noexcept : ptr(std::move(other.ptr)), typeIndex(other.typeIndex) {
        other.typeIndex = typeid(void); // Resets the moved-from object's typeIndex
    }

    template<typename T>
    Any(const T& value) : ptr(new Derived<T>(value)), typeIndex(typeid(T)) {}

    Any& operator=(const Any& other) {
        if (this == &other) // Check for self-assignment
            return *this;
        ptr = other.clone();
        typeIndex = other.typeIndex;
        return *this;
    }

    Any& operator=(Any&& other) noexcept {
        if (this == &other) // Check for self-assignment
            return *this;
        ptr = std::move(other.ptr);
        typeIndex = other.typeIndex;
        other.typeIndex = typeid(void); // Resets the moved-from object's typeIndex
        return *this;
    }

    template<typename T>
    Any& operator=(const T& value) {
        ptr = std::make_unique<Derived<T>>(value);
        typeIndex = typeid(T);
        return *this;
    }

    bool empty() const {
        return !bool(ptr);
    }

    template<typename T>
    bool is() const {
        return typeIndex == typeid(T);
    }

    template<typename T>
    T& any_cast() {
        if (!is<T>()) {
            throw std::bad_cast();
        }
        auto derived = dynamic_cast<Derived<T>*>(ptr.get());
        return derived->value;
    }
};

int main() {
    Any any = 11;
    int anyint = any.any_cast<int>();
    std::cout << anyint << std::endl;

    std::vector<Any> vec;
    vec.emplace_back(10);
    vec.emplace_back(std::string("Hello, Any!"));
    vec.emplace_back(3.14);

    std::cout << vec[0].any_cast<int>() << std::endl;
    std::cout << vec[1].any_cast<std::string>() << std::endl;
    std::cout << vec[2].any_cast<double>() << std::endl;

    try {
        std::cout << vec[1].any_cast<int>() << std::endl;  // This will throw std::bad_cast
    } catch (const std::bad_cast& e) {
        std::cerr << "Bad cast: " << e.what() << std::endl;
    }

    return 0;
}
