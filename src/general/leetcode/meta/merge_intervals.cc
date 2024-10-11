class Solution {
public:
    // variation if sorted
    // time: O(N)
    // space: O(1) if we dont count ret array
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>> vec;

        vec.push_back(intervals[0]);

        for (int i = 1; i < intervals.size(); i++) {
            if (vec[vec.size() - 1][1] >= pq.top()[0]){
                // merge
                vec[vec.size() - 1][1] = max(vec[vec.size() - 1][1], intervals[i][1]);
            } else {
                vec.push_back(intervals[i]);
            }
        }

        return vec;
    }

    // time: O(N lg N)
    // space: O(N)
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        vector<vector<int>> vec;

        auto cmp = [](const vector<int>& lhs, const vector<int>& rhs){ return lhs[0] > rhs[0];}; // sort by start time

        priority_queue<vector<int>, vector<vector<int>>, decltype(cmp)> pq(cmp);

        for (auto i : intervals) pq.push(i);

        vec.push_back(pq.top());
        pq.pop();

        while (pq.size() > 0) {
            if (vec[vec.size() - 1][1] >= pq.top()[0]){
                // merge
                vec[vec.size() - 1][1] = max(vec[vec.size() - 1][1], pq.top()[1]);
            } else {
                vec.push_back(pq.top());
            }
            pq.pop();
        }

        return vec;
    }
};