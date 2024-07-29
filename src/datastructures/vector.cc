#include <iostream>
#include <cstdlib> // For malloc and free
#include <new>     // For placement new
#include <cstring> // For memcpy

template <typename T>
class Vector {
public:
    Vector()
        : data(nullptr), capacity(0), size(0) {}

    ~Vector() {
        clear();
        std::free(data);
    }

    void push_back(const T& value) {
        if (size == capacity) {
            resize();
        }
        new (&data[size]) T(value); // Placement new
        ++size;
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    size_t get_size() const {
        return size;
    }

    size_t get_capacity() const {
        return capacity;
    }

    void clear() {
        for (size_t i = 0; i < size; ++i) {
            data[i].~T();
        }
        size = 0;
    }

private:
    void resize() {
        size_t new_capacity = (capacity == 0) ? 1 : (2 * capacity);
        T* new_data = static_cast<T*>(std::malloc(new_capacity * sizeof(T)));

        if (new_data) {
            for (size_t i = 0; i < size; ++i) {
                new (&new_data[i]) T(std::move(data[i])); // Move construct elements
                data[i].~T(); // Destroy the old element
            }

            std::free(data);
            data = new_data;
            capacity = new_capacity;
        } else {
            throw std::bad_alloc();
        }
    }

    T* data;
    size_t capacity;
    size_t size;
};

int main() {
    Vector<int> vec;
    vec.push_back(10);
    vec.push_back(20);

    std::cout << "Vector elements: ";
    for (size_t i = 0; i < vec.get_size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;

    std::cout << "Vector size: " << vec.get_size() << std::endl;
    std::cout << "Vector capacity: " << vec.get_capacity() << std::endl;

    return 0;
}
