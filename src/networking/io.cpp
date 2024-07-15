#include <iostream>
#include <string>

int main() {
    int num;
    double d;
    std::string str;

    std::cout << "Enter an integer: ";
    std::cin >> num;

    std::cout << "Enter a double: ";
    std::cin >> d;

    std::cout << "Enter a string: ";
    std::cin >> str;

    std::cout << "You entered: " << num << ", " << d << ", and " << str << std::endl;

    return 0;
}
