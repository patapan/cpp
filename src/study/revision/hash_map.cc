// separate chaining

#include <stdexcept>
#include <iostream>
#include <utility>

constexpr int BUCKET_SIZE = 1000;

template <typename K, typename V>
class HashMap {
private:
    struct Node {
        std::pair<K,V> data;
        Node* next;

        Node(K key, V value) : data(std::make_pair(key,value)), next(nullptr) {}

    };    
    Node* buckets[BUCKET_SIZE];

    int get_index(K key) {
        return (std::hash<K>{}(key) % BUCKET_SIZE);
    }

public:
    HashMap() {
        for (int i = 0; i < BUCKET_SIZE; i++) {
            buckets[i] = nullptr;
        }
    }

    // put
    void put(K key, V value) {
        int index = get_index(key);
        Node* node = buckets[index];
        if (node == nullptr) {
            buckets[index] = new Node(key, value);
            return;
        }

        while (node->next != nullptr) {
            node = node->next;
        }
        node->next = new Node(key, value);
    }

    // access
    V operator[](K key) {
        Node* node = buckets[get_index(key)];

        if (node == nullptr) {
            throw std::runtime_error("No key for value");
        }
        while (node && node->data.first != key) {
            node = node->next;
        }

        if (node) {
            return node->data.second;
        }
        throw std::runtime_error("No key for value");
    }

    // copy, move constructors

    // copy, move operators
};

int main() {
    HashMap<std::string,int> map;
    map.put("bruh", 100);
    std::cout << map["bruh"] << std::endl;

}
