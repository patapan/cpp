
#include <thread>
#include <iostream>

void run() {
    std::cout << "hello world\n";
}

int main() {
    std::jthread t1{run};
}