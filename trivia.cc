#include <vector>
#include <iostream>

int main() {
    std::vector<bool> v(10, false);
    auto b = v[0]; // auto is not bool
    std::cout << b << "\n";
}