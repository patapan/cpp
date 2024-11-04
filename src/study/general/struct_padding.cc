#include <iostream>

/*
Alignment is defined by largest type (int which is 8 bytes)

1. Each type needs to start on a multiple of it's size
2. Structure size is a multiple of the largest alignment requirement.
*/

// Size 8 bytes
struct Foo {
    char c; // size 1
    char d; // size 1
    // insert 2 padding here
    int x; // size 4
};

// Size 12 bytes
struct Bar {
    char c;
    // 3 byte padding inserted here    
    int x;
    char d;
    // 3 byte padding inserted here to ensure struct is multiple of largest alignment requirement.
};

// Size 6 bytes (forced to pack struct)
#pragma pack(1)
struct NoPadding {
    char c; // 1
    int x;  // 4
    char d; // 1
};

struct NoPadding2 {
    char c; // 1
    int x;  // 4
    char d; // 1
}__attribute__((__packed__));

int main(){
    std::cout << sizeof(Foo) << std::endl;
    std::cout << sizeof(Bar) << std::endl;
    std::cout << sizeof(NoPadding) << std::endl;
    std::cout << sizeof(NoPadding2) << std::endl;
}