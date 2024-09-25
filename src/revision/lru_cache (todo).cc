/* An LRU Cache is a cache which keeps the K most recently used elements.

The way this works is that it has 2 data structures. Importantly, there is a linkedlist of all of the elements in 
order of them being used. Every time we access or put a value in the hashamp, we move it to the front. The reason why we 
can do this efficiently is that we if we have a pointer to an element in a linkedlist, we can just update its surrounding pointers
to move the node to the front in O(1).

We also have a hashmap which points each key to it's associated node in the linked list. This is critcal aswell.

*/

#include <unordered_map>

<template typename K>
class LRUCache {
private:
    int capacity;

    <template typename V>
    struct Node {
        V value;
        Node* next;
        Node* prev;

        Node(V value) : value(value) {}
    };

    unordered_map<K, Node<V>> map;

public:
    LRUCache(int capacity) : capacity(capacity) {

    }

    // add a key-value pair into our cache, updating the key if needed.
    bool put(K key, V value) {
        // check if the node exists, else create it.
    }

    // remove a key from the cache
    bool remove(K key) {

    }

};

int main() {

}