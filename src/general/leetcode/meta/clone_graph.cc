/*
// Definition for a Node.
class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() {
        val = 0;
        neighbors = vector<Node*>();
    }
    Node(int _val) {
        val = _val;
        neighbors = vector<Node*>();
    }
    Node(int _val, vector<Node*> _neighbors) {
        val = _val;
        neighbors = _neighbors;
    }
};
*/

class Solution {
public:
    Node* cloneGraph(Node* node) {
        unordered_map<Node*, Node*> map;

        return clone(node, map);
    }

    // O(N + E) (nodes + edges)
    Node* clone(Node* node, unordered_map<Node*, Node*>& map) {
        if (!node) return nullptr;

        Node* new_node;
        if (map[node]) {
            new_node = map[node];
        } else {
            new_node = new Node(node->val);
            map[node] = new_node;

            // populate neighbors
            for (auto it : node->neighbors) {
                new_node->neighbors.push_back(clone(it, map));
            }

        }

        return new_node;
    }
};