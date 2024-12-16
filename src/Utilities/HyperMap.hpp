#ifndef HYPER_MAP_HPP
#define HYPER_MAP_HPP

#include "HyperList.hpp"

/**
 * HyperMap class template representing a key-value store.
 * Utilizes two HyperList instances to manage keys and their associated values.
 * 
 * @tparam K Type of keys stored in the map.
 * @tparam V Type of values associated with the keys.
 */
template <typename K, typename V>
class HyperMap {
public:
    HyperMap();                   ///< Constructor to initialize the map.
    ~HyperMap();                  ///< Destructor to clear the map.

    /**
     * Adds a new key-value pair to the map.
     * @param key Key to be added.
     * @param value Value to be associated with the key.
     */
    void put(K key, V value);

    /**
     * Checks if the map contains the given key.
     * @param key Key to check for in the map.
     * @return True if the key is found, false otherwise.
     */
    bool containsKey(K key);

    /**
     * Retrieves the value associated with the given key.
     * @param key Key whose associated value is to be returned.
     * @return Value associated with the specified key.
     */
    V get(K key);

    /**
     * Removes the key-value pair associated with the given key.
     * @param key Key whose key-value pair is to be removed.
     */
    void remove(K key);

    /**
     * Gets the number of key-value pairs in the map.
     * @return Number of key-value pairs in the map.
     */
    int size();

    /**
     * Checks if the map is empty.
     * @return True if the map is empty, false otherwise.
     */
    bool isEmpty();

    /**
     * Clears all key-value pairs from the map.
     */
    void clear();

    /**
     * Provides access to the value associated with the given key.
     * If the key doesn't exist, it inserts a default value and returns a reference to it.
     * 
     * @param key Key whose associated value is to be accessed or inserted.
     * @return Reference to the value associated with the key.
     */
    V& operator[](K key);

private:
    HyperList<K> keys;      ///< List to store the keys.
    HyperList<V> values;    ///< List to store the values.

    /**
     * Finds the index of the given key in the keys list.
     * @param key Key to find in the keys list.
     * @return Index of the key, or -1 if not found.
     */
    int findKeyIndex(K key);
};

template <typename K, typename V>
HyperMap<K, V>::HyperMap() {}

template <typename K, typename V>
HyperMap<K, V>::~HyperMap() {
    clear();
}

template <typename K, typename V>
void HyperMap<K, V>::put(K key, V value) {
    int index = findKeyIndex(key);
    if (index != -1) {
        values.replace(index, value); // Replace value if key exists
    } else {
        keys.addend(key);
        values.addend(value);
    }
}

template <typename K, typename V>
bool HyperMap<K, V>::containsKey(K key) {
    return findKeyIndex(key) != -1;
}

template <typename K, typename V>
V HyperMap<K, V>::get(K key) {
    int index = findKeyIndex(key);
    if (index != -1) {
        return values.get(index);
    }
    return V(); // Return default-constructed value if key is not found
}

template <typename K, typename V>
void HyperMap<K, V>::remove(K key) {
    int index = findKeyIndex(key);
    if (index != -1) {
        keys.remove(index);
        values.remove(index);
    }
}

template <typename K, typename V>
int HyperMap<K, V>::size() {
    return keys.size();
}

template <typename K, typename V>
bool HyperMap<K, V>::isEmpty() {
    return keys.isEmpty();
}

template <typename K, typename V>
void HyperMap<K, V>::clear() {
    keys.clear();
    values.clear();
}

template <typename K, typename V>
int HyperMap<K, V>::findKeyIndex(K key) {
    return keys.indexOf(key);
}

template <typename K, typename V>
V& HyperMap<K, V>::operator[](K key) {
    int index = findKeyIndex(key);
    if (index == -1) {
        // Key not found, add it with a default value
        keys.add(key);
        values.add(V()); // Add a default-constructed value for the new key
        index = keys.size() - 1;
    }
    return values.get(index); // Return a reference to the value
}

#endif // HYPER_MAP_HPP