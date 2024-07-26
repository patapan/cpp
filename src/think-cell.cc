#include <map>
#include <iostream>

template<typename K, typename V>
class interval_map {
	friend void IntervalMapTest();
	V m_valBegin;
	std::map<K,V> m_map;
public:
	// constructor associates whole range of K with val
	interval_map(V const& val)
	: m_valBegin(val)
	{}

	// Assign value val to interval [keyBegin, keyEnd).
	// Overwrite previous values in this interval.
	// Conforming to the C++ Standard Library conventions, the interval
	// includes keyBegin, but excludes keyEnd.
	// If !( keyBegin < keyEnd ), this designates an empty interval,
	// and assign must do nothing.
	void assign( K const& keyBegin, K const& keyEnd, V const& val ) {
        // Empty interval
        if (!(keyBegin < keyEnd)) return;

        auto nextKey = m_map.upper_bound(keyBegin); // fetch the next key in the map.
        auto prevKey = nextKey--;

        if (prevKey->second == val) return; // We don't need a new key as the previous key handles this value
        m_map[keyBegin] = val;
        
        if (nextKey != m_map.end() && nextKey->first < keyEnd) {
            // we need to move the old key from it->first to keyEnd.
            m_map.erase(nextKey->first);

            // If the old_key val is different to our new value,
            // add it back into the map.
            if (nextKey->second != val) m_map[keyEnd] = nextKey->second;
        }

        /*
        - If we have keys already present in the interval, we need to move them
        - 2 scenarios:
            1. Something from earlier than keyBegin overlaps into keyBegin interval.
            2. Something from later than keyEnd start before keyEnd.
        - If consecutive keys have same value we need to merge them

        Constraints:
        - Only one operation with running time of amortized complexity O(log N), 
            should be executed per call to assign, where N is the number of elements in m_map.
        */
    }

	// look-up of the value associated with key
	V const& operator[]( K const& key ) const {
		auto it=m_map.upper_bound(key);
		if(it==m_map.begin()) {
			return m_valBegin;
		} else {
			return (--it)->second;
		}
	}
};

// Many solutions we receive are incorrect. Consider using a randomized test
// to discover the cases that your implementation does not handle correctly.
// We recommend to implement a test function that tests the functionality of
// the interval_map, for example using a map of int intervals to char.

void test() {
    interval_map<int,char> imap('A');

    imap.assign(1, 5, 'B');
    imap.assign(3, 6, 'A');

    for (int i = -2; i < 8; i++) {
        std::cout << "Key: " << i << " Value: " << imap[i] << std::endl;
    }
}

int main() {
    test();
}