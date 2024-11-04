class Solution {
public:
    // time: O(N)
    // space: O(1)
    int longestOnes(vector<int>& nums, int k) {
        int left = 0;
        int right = 0;
        int curr = 0;
        int max = 0;

        // [0,0,0] k=2
        // left=1, right=2, curr=1

        while (right < nums.size()) {
            if (nums[right] == 1 && curr <= k) {
                max = std::max(max, right - left + 1);
                right++;
            } else if (nums[right] == 0 && curr < k) { // move forward as far as we can
                max = std::max(max, right - left + 1);
                curr++;
                right++;
            } else {
                if (nums[left] == 0) curr--;
                left++;
            }
        }

        return max;

    }
};