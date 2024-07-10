#include <iostream>

// Get the rank / dimension of an array at compile time

// primary template
template <class N>
struct rank { static constexpr int value = 0; };

// partial specialisation
template <class N, size_t T>
struct rank<N[T]> { static constexpr int value = 1 + rank<N>::value; } ;

// partial specialisation
template <class N>
struct rank<N[]> { static constexpr int value = 1 + rank<N>::value; } ;

int main() {
    using arrType = int[][1][2];
    std::cout << rank<arrType>::value << "\n";
}