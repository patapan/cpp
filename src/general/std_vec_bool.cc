#include <vector>
#include <iostream>

/*
std::vector<bool> packs each bool together (rather than as a separate byte) to optimize space.
*/

int main() {
    std::vector<bool> v(10, false);
    auto b = v[0]; // auto is not bool
    std::cout << b << "\n";
}