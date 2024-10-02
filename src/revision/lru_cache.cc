/* An LRU Cache is a cache which keeps the K most recently used elements.

The way this works is that it has 2 data structures. Importantly, there is a linkedlist of all of the elements in 
order of them being used. Every time we access or put a value in the hashamp, we move it to the front. The reason why we 
can do this efficiently is that we if we have a pointer to an element in a linkedlist, we can just update its surrounding pointers
to move the node to the front in O(1).

We also have a hashmap which points each key to it's associated node in the linked list. This is critcal aswell.

*/

#include <unordered_map>
class LRUCache {
private:
    int capacity;

    unordered_map<int, std::list<pair<int,int>>::iterator> map;
    std::list<pair<int,int>> queue;

public:
    LRUCache(int capacity) : capacity(capacity) {
    }
    
    int get(int key) {
        if (map.contains(key)) {
            auto it = map[key];
            pair<int,int> p = *it;
            queue.erase(it);
            queue.push_front(p);
            map[key] = queue.begin();
            return p.second;
        }
        return -1;
    }
    
    void put(int key, int value) {
        if (map.contains(key)) {
            // remove the existing node
            auto it = map[key];
            queue.erase(it);
        }

        queue.push_front(make_pair(key,value));
        auto it = queue.begin();
        map[key] = it;

        if (queue.size() > capacity) {
            map.erase(std::prev(queue.end())->first);
            queue.pop_back();
        }
    }
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */

int main() {

}