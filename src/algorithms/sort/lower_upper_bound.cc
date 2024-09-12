class Solution {
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int first = lower_bound(nums, target);
        if (first == nums.size() || nums[first] != target) return {-1,-1}; 
        int last = upper_bound(nums, target);

        return {first, last};
    }

    // Returns first index of value in array
    int firstPosition(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while (left < right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] < target) {
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        return left;
    }

    // Return the last index of target in array.
    // we do left <= right, because in the last loop the right is decremented below the left
    int lastPosition(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size() - 1;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (nums[mid] <= target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        } 
        return right;
    }
};