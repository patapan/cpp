/*
std forward retains the ref type of the value that is passed in.

The point of this is to invoke the correct constructor for a given class (copy, move)
*/

#include <utility> 
#include <memory>
#include <string>
#include <iostream>

namespace alex {

// Implement Remove Reference

template <typename T>
struct remove_reference { 
    using type = T;
};

template <typename T>
struct remove_reference<T&&> {
    using type = T;
};

template <typename T>
struct remove_reference<T&> {
    using type = T;
};

// std::move is a casting operator from L value to R value
template <typename T>
constexpr remove_reference<T>::type&& move(T&& t) {
    return static_cast<remove_reference<T>::type&&>(t);
}

// std::forward retains the value type of the arg passed in.
template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& t) {
    return static_cast<T&&>(t);
}

template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type&& t) {
    return static_cast<T&&>(t);
}

class Factory {
private:
    std::string name;
public:
    // move 
    Factory(std::string&& name) noexcept : name(alex::move(name)){
        std::cout << "Move constructor\n";
    }

    // copy
    Factory(const std::string& name) : name(name){
        std::cout << "Copy constructor\n";
    } 
};


template <typename T, typename ...Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new Factory(std::forward<Args>(args)...));
}

}  // namespace alex

int main() {
    auto p = alex::make_unique<alex::Factory>("alex");

    std::string name = "stephen";

    auto p2 = alex::make_unique<alex::Factory>(name);
    auto p3 = alex::make_unique<alex::Factory>(alex::move(name));

    std::cout << name << std::endl;
}