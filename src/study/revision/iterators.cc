#include <iostream>
#include <list>

template <typename T>
void iterate(T& nums) {
    for (auto it = nums.begin(); it < nums.end();) {
        *it *= 2; 
        std::advance(it, 2);
    }
}

int main() {
    std::list<int> nums = {1,2,3};
    iterate(nums);
    for (auto it : nums) std::cout << it << std::endl;
}