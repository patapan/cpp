#include <iostream>

// A HashMap implementation which uses separate chaining collision resolution.

const int NUM_BUCKETS = 1000;

template <typename K, typename V>
class HashMap {
 private:
    struct Node {
        std::pair<K,V> data; // each node stores a key value pair
        Node* next;
        Node(std::pair<K,V> data) : data(data), next(nullptr) {}
    };
    Node* buckets[NUM_BUCKETS];

    int hash(K key) {
        return std::hash<K>{}(key) % NUM_BUCKETS;
    }

    void deep_copy(const HashMap& other) {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            if (other.buckets[i] == nullptr) {
                buckets[i] = nullptr;
            } else {
                // initialise bucket with head
                buckets[i] = new Node(other.buckets[i]->data);
                Node* node = buckets[i];
                Node* other_node = other.buckets[i];

                while (other_node->next) {
                    node->next = new Node(other_node->next->data);
                    node = node->next;
                    other_node = other_node->next;
                }
            }
        }
    }
    
    // Move ownership of the nodes from other to this HashMap.
    void deep_move(HashMap&& other) noexcept {
        for (int i = 0; i < NUM_BUCKETS; i++) {
            buckets[i] = other.buckets[i];
            other.buckets[i] = nullptr;
        }
    }

    void release() {
        // Delete every node
        for (int i = 0; i < NUM_BUCKETS; i++) {
            Node *node = buckets[i];
            while (node) {
                Node* tmp = node->next;
                delete node;
                node = tmp;
            }
        }
    }

 public:
    HashMap() {
        for (int i = 0; i < NUM_BUCKETS; ++i) {
            buckets[i] = nullptr;
        }
    }
    
    // Destructor
    ~HashMap() {
        release();
    }

    // Copy constructor 
    // Performs a deep copy.
    HashMap(const HashMap& other) {
        deep_copy(other);
    }

    // Copy assignment 
    HashMap& operator=(const HashMap& other) {
        if (this != &other) {
            // check for self assignment
            release();
            deep_copy(other);
        }
        return *this;
    }

    // Move constructor
    HashMap(HashMap&& other) noexcept {
        deep_move(other);
    }

    // Move assignment
    HashMap& operator=(HashMap&& other) noexcept {
        if (this != &other) {
            release();
            deep_move(other);
        }
        return *this;
    }

    // put key value pair into our hashmap
    void put(K key, V value) {
        int hash = this->hash(key);
        if (buckets[hash] == nullptr) {
            // first node in bucket.
            buckets[hash] = new Node(std::make_pair(key,value));;
        } else {
            // put it at the end (but also check if key already exists)
            Node* node = buckets[hash];
            while (node->next) {
                if (node->data.first == key) {
                    // if key already exists, update it.
                    node->data.second = value;
                    return;
                }
                node = node->next;
            }
            // check if key at last index
            if (node->data.first == key) {
                node->data.second = value;
            } else {
                // else we make a new node for it.
                node->next = new Node(std::make_pair(key,value));
            }
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

    auto map2 = map;

    std::cout << map2.get(1) << "\n";
}