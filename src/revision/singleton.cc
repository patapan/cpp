// Lazy singleton

template <typename T>
class Singleton {
 private:
    static T* data;
    static Singleton* singleton = nullptr;
    Singleton() {
        singleton = this;
    }

 public:
    static Singleton& static get() {
        if (!singleton) {
            singleton = new Singleton();    
        } 
        return singleton;
    }
}

int main(){}