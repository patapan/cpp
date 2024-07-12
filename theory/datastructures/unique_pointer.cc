
template <typename T>
class UniquePointer {
 private:
    T* data;
 public:
    UniquePointer(T* data = nullptr) : data(data) {}

    // Destructor
    ~UniquePointer() {
        delete data;
    }

    // Fetch the raw pointer.
    T* get() const {
        return data;
    }

    // Copy constructor
    UniquePointer(const UniquePointer& other) = delete;
    // Copy assignment operator
    UniquePointer& operator=(const UniquePointer& other) = delete; 

    // Move constructor
    UniquePointer(UniquePointer&& other) noexcept {
        data = other.data;
        other.data = nullptr;
    }

    // Move assignment operator
    UniquePointer& operator=(UniquePointer&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
            
        }
        return *this;
    }

    // Return the raw pointer from management
    T* release() {
        T* ret = data;
        data = nullptr;
        return ret;
    }
};