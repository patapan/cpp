class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        int left = 0;
        int right = nums.size() - 1;
        
        while (left < right) {
            int mid = left + (right - left) / 2;

            if (mid < nums.size() - 1 && nums[mid] < nums[mid + 1]) {
                // there is a peak on the right hand side
                left = mid + 1;
            } else {
                right = mid;
            }
        }

        return left;
    }
};