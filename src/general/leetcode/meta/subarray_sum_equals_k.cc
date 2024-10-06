#include <vector>
#include <iostream>
#include <unordered_map>
 
using namespace std;

class Solution {
public:
    int subarraySum(vector<int>& nums, int k) {
        // We record all the prefix sums we've seen in a map
        // Then, if we see curr_sum - k, we know we can combine that to make a subarray of sum k
        // prefix[j] - prefix[i] = k
        // prefix[i] = prefix[j] - k

        // curr = j, prev = i

        int curr = 0;
        int count = 0;
        unordered_map<int,int> map; //  prefix_sums -> count of prefix sums

        map[0] = 1;

        for (int i = 0; i < nums.size(); i++) {
            curr += nums[i];
            count += map[curr - k];
            map[curr]++;
                        
        }
        return count;
    }

    // variation: return all subarrays (not just the count)
    // average time O(N), 
    // worst case: O(N^2) where we have something like [1,1,1,1,1,1]
    static vector<vector<int>> subarraySumVariation(vector<int>& nums, int k) {

        int curr = 0;
        vector<vector<int>> ret;

        unordered_map<int, vector<int>> map; // prefix_sum -> list of start_indices
        map[0].push_back(-1); // start from index -1 to handle cases starting from 0

        for (int i = 0; i < nums.size(); i++) {
            curr += nums[i];            

            for (auto& startIdx : map[curr - k]) {
                ret.push_back(substr(nums, startIdx + 1, i));
            }


            // Store the current index for the current prefix sum
            map[curr].push_back(i);
        }

        return ret;
    }
private:
    // O(E - S)
    static vector<int> substr(vector<int>& nums, int start, int end) {
        vector<int> ret;

        for (int i = start; i <= end; i++) {
            ret.push_back(nums[i]);
        }

        return ret;
    }
};

int main() {
    vector<int> arr = {1,2,3};
    int k = 3;

    auto subarrays = Solution::subarraySumVariation(arr, k);

    for (auto& it : subarrays) {
        for (auto& it2 : it) {
            cout << it2 << " ";
        }
        cout << endl;
    }
}