
#include <iostream>

// A class that can only have 1 instance in the entire program.
template <typename T>
class Singleton {
 private:
    static Singleton* instance;
    T data;

    Singleton(){}

 public:
    void set_data(T data_) {
        data = data_;
    }

    T& get_data() {
        return data;
    }

    static Singleton* get() {
        if (instance == nullptr) {
            instance = new Singleton<T>();
        }
        return instance;
    }

    Singleton(const Singleton& other) = delete;
    Singleton& operator=(const Singleton& other) = delete;
    Singleton(Singleton&& other) noexcept = delete;
    Singleton& operator=(Singleton&& other) noexcept = delete;
};

// We must define it out here to make sure it's value initialized.
template <typename T>
Singleton<T>* Singleton<T>::instance;

int main(){
    Singleton<int>* s = Singleton<int>::get();
    s->set_data(50);
    std::cout << s->get_data() << "\n";
}