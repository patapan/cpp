#include <iostream>
#include <vector>
#include <algorithm> // For std::sort (if needed)

// Binary search function
int binary_search(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = arr.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if target is present at mid
        if (arr[mid] == target) {
            return mid;
        }

        // If target is greater, ignore the left half
        if (arr[mid] < target) {
            left = mid + 1;
        }
        // If target is smaller, ignore the right half
        else {
            right = mid - 1;
        }
    }

    // If the element is not present in the array
    return -1;
}

int main() {
    std::vector<int> arr = {2, 3, 4, 10, 40};
    int target = 10;

    // If the array is not sorted, sort it first
    // std::sort(arr.begin(), arr.end());

    int result = binary_search(arr, target);

    if (result != -1) {
        std::cout << "Element found at index " << result << std::endl;
    } else {
        std::cout << "Element not found in the array" << std::endl;
    }

    return 0;
}
