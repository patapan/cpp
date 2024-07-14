#include <iostream>

const int NUM_BUCKETS = 1000;

template <typename K, typename V>
class HashMap {
 private:
    struct Node {
        std::pair<K,V> data; // each node stores a key value pair
        Node* next;
        Node(std::pair<K,V> data) : data(data){}
    };
    Node* buckets[NUM_BUCKETS];

    int hash(K key) {
        return std::hash<K>{}(key) % NUM_BUCKETS;
    }

 public:
    HashMap() {
        for (int i = 0; i < NUM_BUCKETS; ++i) {
            buckets[i] = nullptr;
        }
    }

    ~HashMap() {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            Node *node = buckets[i];
            while (node) {
                Node* tmp = node->next;
                delete node;
                node = tmp;
            }
        }
    }

    // put key value pair into our hashmap
    void put(K key, V value) {
        int hash = this->hash(key);
        std::cout << hash << "\n";
        Node* new_node = new Node(std::make_pair(key,value));
        if (buckets[hash] == nullptr) {
            buckets[hash] = new_node;
        } else {
            // put it at the end
            Node* node = buckets[hash];
            while (node->next) {
                node = node->next;
            }
            node->next = new_node;
        }
    }

    // fetch from map
    V get(K key) {
        Node* node = buckets[hash(key)];
        while (node) {
            if (node->data.first == key) {
                return node->data.second;
            } else {
                node = node->next;
            }
        }

        throw std::runtime_error("No key found");
    }
};

int main() {
    HashMap<int,int> map;
    map.put(1,100);
    std::cout << map.get(1) << "\n";
}