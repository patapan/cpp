/*

Given an integer array nums and an integer k, return the kth largest element in the array.

Note that it is the kth largest element in the sorted order, not the kth distinct element.

Can you solve it without sorting?

 

Example 1:

Input: nums = [3,2,1,5,6,4], k = 2
Output: 5
Example 2:

[5,6]

Input: nums = [3,2,3,1,2,4,5,5,6], k = 4
Output: 4
*/

// 2 solutions, heap or quickselect 

// time: O(N * lg K) 
// space: O(k)
int findKthLargest(vector<int>& nums, int k) {
    priority_queue<int, greater<int>> minHeap;

    for (int i : nums) {
        minHeap.insert(i);
        if (minHeap.size() > k) minHeap.pop();
    }

    return minHeap.top();
}


// time: O(N) in best, O(N^2) in worst
// space: O(N^2)
int findKthLargest(vector<int>& nums, int k) {
    vector<int> smaller;
    vector<int> mid;
    vector<int> larger;

    int pivot = nums[rand() % nums.size()];

    for (int i : nums) {
        if (i < pivot) smaller.push_back(i);
        else if (i > pivot) larger.push_back(i);
        else mid.push_back(i);
    }

    if (larger.size() >= k) return larger[larger.size() - k];
    if (larger.size() + mid.size() >= k) return mid[0];
    return findKthLargest(smaller, k - larger.size() - mid.size());
}