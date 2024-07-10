
#include <iostream>

template <int N>
struct getABS {
    static constexpr int value = N > 0 ? N : -N;
};

int main() {
    std::cout << getABS<5>::value << "\n";
}