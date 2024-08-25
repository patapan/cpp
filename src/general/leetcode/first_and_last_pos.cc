#include <iostream>

using namespace std;

class Solution {
public:
    vector<int> searchRange(const vector<int>& nums, int target) {
        vector<int> ret;
        
        // binary search to find first index.
        // then we could binary search to find end

        int start_index = binary_search(nums, target);
        cout << start_index << endl;
        if (start_index < nums.size() - 1 && nums[start_index + 1] != target) return {-1,-1};
        int end_index = binary_search(nums, target+1);

        return {start_index,end_index-1};
    }
private:
    // return the largest index smaller than target
    int binary_search(const vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while (left < right) {
            int mid = (left + right) / 2;
            // cout << mid << endl;
            if (nums[mid] >= target) {
                right = mid - 1;
            } else { // nums[mid] < target
                left = mid + 1;
            }
        }

        return left;
    }
};

int main() {
    Solution sol;

    auto i = sol.searchRange({1}, 0);
    cout << i[0] << " " << i[1] << endl;
}