class Solution {
public:
    // Heap solution
    // Time: O(N lg K)
    // Space: O(K)
    int findKthLargest(vector<int>& nums, int k) {
        // Min-heap of size k
        priority_queue<int, vector<int>, greater<int>> pq;

        for (int i : nums) {
            pq.push(i);
            // Ensure heap size is at most k
            if (pq.size() > k) pq.pop();
        }

        // The root of the heap is the k-th largest element
        return pq.top();
    }

    // quickselect
    // Time: O(N) best case (because every func call we divide N by 2 on average), O(N^2) worst case where we have N function calls which iterate over the arr
    // Space: O(N)
    int findKthLargest(vector<int>& nums, int k) {
        vector<int> smaller;
        vector<int> mid;
        vector<int> larger;

        int pivot = nums[rand() % nums.size()];

        for (int i : nums) {

            if (i < pivot) {
                smaller.push_back(i);
            } else if (i > pivot) {
                larger.push_back(i);
            } else {
                mid.push_back(i);
            }
        }

        if (larger.size() >= k) {
            return findKthLargest(larger, k);
        } else if (larger.size() + mid.size() >= k) {
            return mid[0];
        } else return findKthLargest(smaller, k - larger.size() - mid.size());

    }
};
