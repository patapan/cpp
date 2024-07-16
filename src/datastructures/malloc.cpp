#include <utility>

template <typename T>
class UniquePointer {
 private:
    T* data;

 public:
    UniquePointer(T* data = nullptr) : data(data) {}

    ~UniquePointer() {
        delete data;
    }

    T* get() {
        return data;
    }

    // delete copy constructor & copy assignment
    // because unique pointers are *unique*
    UniquePointer(const UniquePointer& other) = delete;
    UniquePointer& operator=(const UniquePointer& other) = delete;

    // move constructor
    UniquePointer(UniquePointer&& other) noexcept {
        delete data;
        data = other.data;
        other.data = nullptr;
    }

    // move assignment 
    UniquePointer& operator=(UniquePointer&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};

// make_unique constructs an object and returns a unique pointer to it. this way we 
// know for certain that this pointer is unique.
template <typename T, typename ...Args>
UniquePointer<T> make_unique(Args&& ...args) {
    return UniquePointer<T>(new T(std::forward(args)...));
}

int main(){}