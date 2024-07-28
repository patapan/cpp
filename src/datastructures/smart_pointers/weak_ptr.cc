/*
WeakPtrs are used to break the recursive chain of 2 shared pointers pointing at eachother.

When using weakptr. The control block is the class which manages the references. When all the references are gone, we delete control block.

Each shared pointer and weak ptr has a ref to the shared control block
*/

#include <utility>
#include <atomic>

// 1 control block
template <typename T>
struct ControlBlock {
    T* data;
    std::atomic<int> strong_ref;
    std::atomic<int> weak_ref;

    // Must be initialized from shared ptr.
    ControlBlock(T* data_) : data(data_) {
        strong_ref.store(1);
        weak_ref.store(0);
    }

    // Delete copy and move semantics
    ControlBlock(const ControlBlock& other) = delete;
    ControlBlock& operator=(const ControlBlock& other) = delete;
    ControlBlock(ControlBlock&& other) noexcept = delete;
    ControlBlock& operator=(ControlBlock&& other) noexcept = delete;
};

template <typename T>
class WeakPointer;

template <typename T>
class SharedPointer {
    friend class WeakPointer<T>;

    ControlBlock<T>* control_block;

    // Regular constructor
    SharedPointer(T* data_) : control_block(new ControlBlock(data_)) {}

    // Constructor from weakptr
    SharedPointer(ControlBlock<T>* control_block_) : control_block(control_block_) {}

    // Remove our ref from the control_block
    void decrement() {
        if (control_block->strong_ref.fetch_sub(1) == 1) {
            delete control_block->data;

            if (control_block->weak_ref.load() == 0) {
                delete control_block;
            }
        }
    }

    ~SharedPointer() {
        decrement();
    }

    // Copy constructor
    SharedPointer(const SharedPointer& other) {
        other.control_block->strong_ref.fetch_add(1);
        control_block = other.control_block;
    }

    // Copy assignment operator
    SharedPointer& operator=(const SharedPointer& other) {
        if (this != &other) {
            // clean up resourcess
            decrement();
            other.control_block->strong_ref.fetch_add(1);
            control_block = other.control_block;
        }
        return *this;
    } 

    // Move constructor
    SharedPointer(SharedPointer&& other) noexcept {
        control_block = other.control_block;
        other.control_block = nullptr;
    }

    // Move assignment operator
    SharedPointer& operator=(SharedPointer&& other) noexcept {
        if (this != &other) {
            decrement();
            control_block = other.control_block;
            other.control_block = nullptr;
        }
    }
};

template <typename T>
class WeakPointer {
    ControlBlock<T>* control_block;

    // We can only construct a weak ptr with a ref from a shared pointer
    WeakPointer(const SharedPointer<T>& shared_ptr) {
        shared_ptr.control_block->weak_ref.fetch_add(1);
        control_block = shared_ptr.control_block;
    }

    // Convert weak ptr to shared if data still exists
    SharedPointer<T> lock() {
        if (control_block->strong_ref.fetch_add(1) > 1) {
            return SharedPointer<T>(control_block);
        } else {
            return SharedPointer<T>(nullptr);
        }
    }
};


struct Factory {
    std::string data;

    Factory(std::string str) : data(str) {}
};

int main(){
    Factory* f = new Factory("cheese");
    auto sp = SharedPointer(f);
    auto sp2 = sp;
}