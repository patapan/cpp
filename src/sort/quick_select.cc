// https://leetcode.com/problems/kth-largest-element-in-an-array/

class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int pivot = nums[rand() % nums.size()];

        vector<int> left;
        vector<int> mid;
        vector<int> right;

        // sort into 3 vectors
        for (int num : nums) {
            if (num < pivot) {
                left.push_back(num);
            } else if (num > pivot) {
                right.push_back(num);
            } else {
                mid.push_back(num);
            }
        }

        // if largest in right hand side, search that vector
        if (right.size() >= k){
            return findKthLargest(right, k);
        }

        // if largest is in left hand side, search that vector
        if (mid.size() + right.size() < k) {
            return findKthLargest(left, k - (right.size() + mid.size()));
        }

        return pivot;
    }

};