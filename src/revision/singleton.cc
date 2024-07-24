
template <typename T>
class Singleton {
 private:
    T data;

    Singleton() = default;

 public:
    static Singleton* instance;

    static Singleton* get() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }

    // delete copy and move constructors
};

template <typename T>
Singleton<T>* Singleton<T>::instance = nullptr;

int main(){
    auto singleton = Singleton<int>::get();
}