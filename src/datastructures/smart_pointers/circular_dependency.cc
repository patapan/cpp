// Circular Dependency Example

struct Foo {
    std::shared_ptr<Bar> bar;
};

struct Bar {
    std::shared_ptr<Foo> foo;
};