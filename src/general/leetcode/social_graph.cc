

// return the shortest path in a directed graph from person start to person end
int social_graph(unordered_map<int, vector<int>> adjList, int start, int end) {
    // bfs --> linear to num nodes + edges
    // time: O(N + E)
    // space: O(N)

    /*
    1 -> 2          1 -> 3 ans = 2

    2 -> 1, 3
    
    
    */

    unordered_set<int> seen; // we only want to go to a person once --> the first time we get there is the shortest
    std::list<pair<int,int>> queue; // queue of <person, length> 

    queue.push_back(make_pair(start, 1));
    /*
    q = [(2,2)]
    seen = 1,2
    */

    while (queue.size() > 0) {
        auto p = queue.front();
        int person = p->first;
        int length = p->second;
        queue.pop_front();
        seen.insert(person);

        for (int friend_ : adjList[person]) {
            if (!seen.contains(friend_)) {
                if (friend_ == end) return length;
                queue.push_back(make_pair(friend, length + 1));
            }
        }
    }

    return -1;
}