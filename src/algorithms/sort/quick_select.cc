// https://leetcode.com/problems/kth-largest-element-in-an-array/
// Time complexity: O(n) on average, O(n^2) in the worst case
// Space complexity: O(n)
class Solution {
public:
    // This solution should be opimitzed to be O(1) space
    //  Other implementations of Quickselect can avoid creating these three in memory, 
    // but in the worst-case scenario, those implementations would still require O(n) space for the recursion call stack.
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

private:
    // Optimized quickselect with O(N) time and O(1) average case space.

    int partition(vector<int>& nums, int left, int right) {
        int pivotIndex = left + (right - left) / 2;
        int pivotValue = nums[pivotIndex];
        swap(nums[pivotIndex], nums[right]);  // Move pivot to end for ease
        int storeIndex = left;
        for (int i = left; i < right; i++) {
            if (nums[i] < pivotValue) {
                swap(nums[i], nums[storeIndex]);
                storeIndex++;
            }
        }
        swap(nums[storeIndex], nums[right]);  // Move pivot to its final place
        return storeIndex;
    }

    int quickselect(vector<int>& nums, int left, int right, int k) {
        if (left == right) return nums[left];  // If the list contains only one element,

        int pivotIndex = partition(nums, left, right);

        // The pivot is in its final sorted position
        if (k == pivotIndex) {
            return nums[k];
        } else if (k < pivotIndex) {
            return quickselect(nums, left, pivotIndex - 1, k);
        } else {
            return quickselect(nums, pivotIndex + 1, right, k);
        }
    }

};