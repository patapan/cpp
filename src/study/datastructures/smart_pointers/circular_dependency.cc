// Circular Dependency Example

#include <utility>
#include <memory>
#include <iostream>

struct Bar;

struct Foo {
    std::shared_ptr<Bar> bar;

    ~Foo() {
        std::cout << "Foo destructor called" << std::endl;
    }    
};

struct Bar {
    std::shared_ptr<Foo> foo;
    
    ~Bar() {
        std::cout << "Bar destructor called" << std::endl;
    }
};

int main() {
    {
        auto f = std::make_shared<Foo>();
        auto b = std::make_shared<Bar>();

        f->bar = b;
        b->foo = f;
    }
    // No deallocation here!

    std::cout << "end of main\n";
}