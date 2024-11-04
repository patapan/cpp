/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;

    Node() {}

    Node(int _val) {
        val = _val;
        left = NULL;
        right = NULL;
    }

    Node(int _val, Node* _left, Node* _right) {
        val = _val;
        left = _left;
        right = _right;
    }
};
*/

class Solution {
public:
    Node* treeToDoublyList(Node* root) {
        // 3<-4
        // /
        // 2
        // \
        //  3->4

        // recurse(root) -> [leftmost, rightmost]

        if (!root) return nullptr;

        auto p = recurse(root);
        return p.first;
    }

    pair<Node*, Node*> recurse(Node* node){ 
        if (!node->left && !node->right) return make_pair(node,node);

        cout << "here\n Node val: " << node->val << endl;

        auto leftPair = recurse(node->left);
        auto rightPair = recurse(node->right);

        cout << "here1\n";
        if (leftPair.second) {
            node->left = leftPair.second;
            leftPair.second->right = node;
        }         

        cout << "here2\n";
        if (rightPair.first) {
            node->right = rightPair.first;
            rightPair.first->left = node;
        }         
        cout << "here3\n";

        auto leftMost = leftPair.first ? leftPair.first : node;
        auto rightMost = rightPair.second ? rightPair.second : node;

        cout << "here4\n";
        return make_pair(leftMost, rightMost);
    }
};


int main() {
    Solution sol;
    sol.treeToDoublyList()
}