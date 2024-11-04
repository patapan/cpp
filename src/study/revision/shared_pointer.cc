
/*

Shared Pointer is a smart pointer which tracks the number of references

*/

#include <iostream>
#include <atomic>
#include <utility>
#include <memory>


template <typename T>
class SharedPtr {
private:
    T* data;
    mutable std::atomic<int>* count;
public:
    SharedPtr(T* data_ = nullptr) : data(data_) {
        count = data_ == nullptr ? nullptr : new std::atomic<int>(1);
    }

    // Get underlying raw pointer
    T* get() {
        return data;
    }

    // Destructor
    ~SharedPtr() {
        std::cout << "destructor\n";
        if (count && count->fetch_sub(1) == 1) {
            std::cout << "deallocating\n";
            delete data;
            delete count;
        }
    }

    // Rule of five
    // Copy constructor 
    SharedPtr(const SharedPtr& other) {
        std::cout << "copy constructor\n";
        if (other.count) other.count->fetch_add(1);
        data = other.data;
        count = other.count;
    }

    // copy assignment
    SharedPtr& operator=(const SharedPtr& other) {
        std::cout << "copy assignment\n";
        if (this != &other) {
            if (other.count) other.count->fetch_add(1);
            this->~SharedPtr();
            data = other.data;
            count = other.count;
        }
        return *this;
    }

    // Move constructor 
    SharedPtr(SharedPtr&& other) noexcept {
        std::cout << "move constructor\n";
        data = other.data;
        count = other.count;
        other.data = nullptr;
        other.count = nullptr;
    }

    // Move assignment 
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        std::cout << "move assignment \n";
        if (this != &other) {
            this->~SharedPtr();
            data = other.data;
            count = other.count;
            other.data = nullptr;
            other.count = nullptr;
        }
        return *this;
    }
};

template <typename T, typename ...Args>
SharedPtr<T> make_shared(Args&&... args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

struct Factory {
    int a;
    std::string name;
    Factory(std::string name_) : a(10), name(name_){}
};

int main() {
    // Factory* f = new Factory("cheese");

    SharedPtr<Factory> copy_ass_sp;

    {
        SharedPtr<Factory> sp = make_shared<Factory>("cheese");

        // copy construction of sp
        auto sp2 = sp;

        // copy assignment
        copy_ass_sp = sp2;

        // move construction
        auto sp4_move_con = std::move(copy_ass_sp);

        std::cout << "chicken\n";
        sp2 = std::move(sp4_move_con);
        // move assignment
    }
    std::cout << "got here\n";
}


template <typename T, Args... args>
SharedPointer<T> make_shared(Args&& ... args) {
    return SharedPointer(new T(std::forward(args)...));
}