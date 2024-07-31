#include <atomic>
#include <iostream>
#include <utility>

// 1 control block per raw pointer
template <typename T>
struct ControlBlock {
    T* data;
    std::atomic<int> strong_ref;
    std::atomic<int> weak_ref;

    // This should only be invoekd by sharedptr constructor
    ControlBlock(T* data_) : data(data_), strong_ref(1), weak_ref(0) {}

    void release() {
        std::cout << "release" << std::end;
        delete data;
    }

    // Disable copy and move semantics
    ControlBlock(const ControlBlock& other) = delete;
    ControlBlock& operator=(const ControlBlock& other) = delete;
    ControlBlock(ControlBlock&& other) noexcept = delete;
    ControlBlock& operator=(ControlBlock&& other) noexcept = delete;
};

template <typename T>
class WeakPtr;

template <typename T>
class SharedPtr {
    friend class WeakPtr<T>;
private:
    ControlBlock<T>* control_block;

public:
    SharedPtr(ControlBlock<T>& cb) : control_block(&cb){
        // weak ptr -> shared ptr
        cb.strong_ref.fetch_add(1);
        cb.weak_ref.fetch_sub(1);
    }
    SharedPtr(T* data_) : control_block(new ControlBlock(data_)) {}

    ~SharedPtr() {
        if (control_block && control_block->strong_ref.fetch_sub(1) == 1) {
            control_block->release();

            if (control_block && control_block->weak_ref.load(std::memory_order_acquire)) {
                delete control_block;
            }
        }
    }

    // move semantics on shared ptr 
};

template <typename T, typename... Args>
SharedPtr<T> make_shared(Args ...args) {
    return SharedPtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
class WeakPtr {
private:
    ControlBlock<T>* control_block;
public:
    ~WeakPtr() {
        control_block->weak_ref.fetch_sub(1);
    }

    // Constructed from shared ptr
    WeakPtr(const SharedPtr<T>& sp) {
        control_block = sp.control_block;
        control_block->weak_ref.fetch_add(1);
    }

    SharedPtr<T> lock() {
        if (control_block->data != nullptr) {
            return SharedPtr<T>(control_block);
        }
        return SharedPtr<T>(nullptr);
    }
};

struct Factory {
    std::string name;
    Factory(std::string name_) : name(name_) {}
};

int main() {
    auto sp = make_shared<Factory>("cheese");

    WeakPtr<Factory> wp = sp;

    auto sp2 = wp.lock();
}