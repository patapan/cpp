#include <iostream>

/*
Order is:
1. Base class constructor
2. Member class constructor (from initialization list)
3. Derived class constructor 

Then for destruction, its the opposite ordering
4. Derived class destructor
5. Member class destructor
6. Base class destructor
*/

class Base {
public:
    Base() { std::cout << "Base constructor\n"; }
    ~Base() { std::cout << "Base destructor\n"; }
};

class Member {
public:
    Member() { std::cout << "Member constructor\n"; }
    ~Member() { std::cout << "Member destructor\n"; }
};

class Derived : public Base {
public:
    Derived() : member() { std::cout << "Derived constructor\n"; }
    ~Derived() { std::cout << "Derived destructor\n"; }

private:
    Member member;
};

int main() {
    Derived d;
    return 0;
}
