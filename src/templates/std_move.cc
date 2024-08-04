
/*
Move casts an L value ref to an R value ref
*/

// partial specializtion for L value
template <typename T>
struct remove_ref<T&> {
    constexpr static typedef type = T;
};

// partial specialization for R value
template <typename T>
struct remove_ref<T&> {
    constexpr static typedef type = T;
};

// full specialization
template <typename T>
struct remove_ref {
    using typedef 
};

template <typename T>
remove_ref<T>&& ref std::move(T&& ref) noexcept {
    return static_cast<>(ref);
}