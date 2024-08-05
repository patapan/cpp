/*
Construct a print func at compile time.
*/
#include <iostream>

using namespace std;

// base case
void print() {
    cout << endl;
}

// recursive case
template <typename Head, typename ... Args>
void print(const Head& head, const Args& ...args) {
    cout << head << endl;
    print(args...);
}

int main() {
    print(1,2,3);
}