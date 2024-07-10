#include <atomic>
#include <iostream>

int main() {
    std::atomic_int count;
    std::cout << count << "\n";
    count.fetch_sub(3);
    std::cout << count << "\n";
}