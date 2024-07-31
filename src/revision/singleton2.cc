
/*
Singleton - Hold some data. 
*/

#include <iostream>

using namespace std;

template <typename T>
class Singleton {
private:

    static Singleton* instance;

    Singleton() {}

public:
    T data;
    void set(T new_data) {
        data = new_data;
    }

    static Singleton<T>* get() {
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
};

template <typename T>
Singleton<T>* Singleton<T>::instance = nullptr;

int main() {
    auto s = Singleton<int>::get();
    s->set(10);

    auto s2 = Singleton<int>::get();
    cout << s2->data << endl;

}


