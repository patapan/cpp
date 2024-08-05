#include <memory>

// 1. Virtual function impl
struct FooInterface {
    [[nodiscard]] virtual auto func() const -> int = 0;
};

struct Foo final : public FooInterface {
    [[nodiscard]] auto func() const -> int override {
        return 42;
    }
};

std::unique_ptr<FooInterface> foo = std::make_unique<Foo>();

auto func(std::unique_ptr<FooInterface> foo2) {
    // Implementation here
}

// 2. Concept impl
template <typename T>
concept CFoo = requires(T foo) {
    { foo.func() } -> std::integral;
};

Foo foo1;

auto func(CFoo& foo2) {
    // Implementation here
}
