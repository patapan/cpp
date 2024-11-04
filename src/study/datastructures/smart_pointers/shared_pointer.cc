#include <atomic>
#include <utility>
#include <string>
#include <thread>
#include <stop_token>
#include <iostream>

template <typename T>
class SharedPointer {
 private:
    T* data; //multiple copies of sharedpointer have the same pointer address to data
    std::atomic<int>* counter;

    // release the pointer
    void release() noexcept {
        if (counter && counter->fetch_sub(1, std::memory_order_acq_rel) == 1) {
            delete data;
            delete counter;
        }
    }

 public:
    // The issue with this constructor is that we don't know how many times this
    // pointer has been copied already.
    explicit SharedPointer(T* data_ = nullptr) : data(data_), counter(nullptr) {
        if (data_) counter = new std::atomic<int>(1);
    }

    ~SharedPointer() {
        release();
    }

    // copy constructor 
    SharedPointer(const SharedPointer& other) : data(other.data), counter(other.counter) {
        if (counter) {
            counter->fetch_add(1, std::memory_order_acq_rel);
        }
    }

    // copy assignment
    SharedPointer& operator=(const SharedPointer& other) {
        if (this != &other) {
            release(); // release existing resources associated with this obj
            counter = other.counter;
            data = other.data;
            if (counter) counter->fetch_add(1, std::memory_order_acq_rel);
        }
        return *this;
    }

    // move constructor 
    SharedPointer(SharedPointer&& other) noexcept {
        data = other.data;
        counter = other.counter;
        other.data = nullptr;
        other.counter = nullptr;
    }

    // move assignment
    SharedPointer& operator=(SharedPointer&& other) noexcept {
        if (this != &other) {
            release();
            data = other.data;
            counter = other.counter;
            other.data = nullptr;
            other.counter = nullptr;
        }
        return *this;
    }

    // dereference operator returns a reference to object
    T& operator*() noexcept {
        return *data;
    }

    T* operator->() noexcept {
        return data;
    }
};

// constructs the object for you
// This uses template packing and perfect forwarding
template <typename T, typename ...Args>
SharedPointer<T> make_shared(Args&& ...args) {
    return SharedPointer<T>(new T(std::forward<Args>(args)...));
}

struct Factory {
    std::string name;
    int num_robots;
    Factory(std::string name, int num_robots) : name(name), num_robots(num_robots){}
};

void run(int thread_id, SharedPointer<Factory> p) {
    std::cout << p->name << "\n";
}

int main() {
    auto p1 = make_shared<Factory>("chicken",20);
    Factory f("cheese",100);
    auto p2 = make_shared<Factory>(f);

    std::jthread t1{run, 1, p1};
    std::jthread t2{run, 1, p2};
    return 0;
}