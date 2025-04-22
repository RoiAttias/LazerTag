/**
 * @file hyper_map.hpp
 * @brief Defines the HyperMap template, a simple key-value store built on HyperList.
 *
 * HyperMap uses two HyperList instances to maintain parallel lists of keys and values.
 * Provides typical map operations such as insertion, lookup, removal, and size queries.
 * Suitable for Arduino and embedded projects needing a lightweight associative container.
 */

 #ifndef HYPER_MAP_HPP
 #define HYPER_MAP_HPP
 
 #include "HyperList.hpp"
 
 /**
  * @brief Template class for a key-value store (map).
  *
  * Uses two HyperList instances to store keys and corresponding values in parallel.
  * Provides methods to add, retrieve, remove, and query entries by key.
  *
  * @tparam K Type of keys stored in the map.
  * @tparam V Type of values associated with the keys.
  */
 template <typename K, typename V>
 class HyperMap {
 public:
     /** @brief Constructs an empty HyperMap. */
     HyperMap();
 
     /** @brief Destroys the map and clears all entries. */
     ~HyperMap();
 
     /**
      * @brief Inserts or updates a key-value pair in the map.
      *
      * If the key already exists, its associated value is replaced.
      * Otherwise, a new key-value pair is appended.
      *
      * @param key Key to insert or update.
      * @param value Value to associate with the key.
      */
     void put(const K& key, const V& value);
 
     /**
      * @brief Checks whether the map contains a given key.
      *
      * @param key Key to search for.
      * @return True if the key exists, false otherwise.
      */
     bool containsKey(const K& key) const;
 
     /**
      * @brief Retrieves the value associated with a key.
      *
      * Returns the corresponding value if the key is found;
      * otherwise returns a default-constructed V.
      *
      * @param key Key whose value to retrieve.
      * @return Associated value or V() if not found.
      */
     V get(const K& key) const;
 
     /**
      * @brief Removes the key and its associated value from the map.
      *
      * If the key exists, both key and value are removed.
      *
      * @param key Key to remove.
      */
     void remove(const K& key);
 
     /**
      * @brief Returns the number of key-value pairs in the map.
      *
      * @return Current size of the map.
      */
     int size() const;
 
     /**
      * @brief Checks whether the map is empty.
      *
      * @return True if no entries exist, false otherwise.
      */
     bool isEmpty() const;
 
     /** @brief Removes all entries from the map. */
     void clear();
 
     /**
      * @brief Access or create value by key.
      *
      * If the key exists, returns a reference to its value.
      * Otherwise, inserts the key with a default-constructed V and returns a reference.
      *
      * @param key Key to access or insert.
      * @return Reference to the value associated with the key.
      */
     V& operator[](const K& key);
 
 private:
     HyperList<K> keys;    ///< List of keys in insertion order.
     HyperList<V> values;  ///< Parallel list of values corresponding to keys.
 
     /**
      * @brief Finds the index of a key in the keys list.
      *
      * @param key Key to locate.
      * @return Index of the key, or -1 if not found.
      */
     int findKeyIndex(const K& key) const;
 };
 
 // ---------------- Implementation ----------------
 
 template <typename K, typename V>
 HyperMap<K, V>::HyperMap() {
     // No explicit initialization needed; HyperList defaults to empty
 }
 
 template <typename K, typename V>
 HyperMap<K, V>::~HyperMap() {
     // Clear both lists to free nodes
     clear();
 }
 
 template <typename K, typename V>
 void HyperMap<K, V>::put(const K& key, const V& value) {
     int idx = findKeyIndex(key);
     if (idx != -1) {
         // Replace existing value
         values.replace(idx, value);
     } else {
         // Append new key and value
         keys.addend(key);
         values.addend(value);
     }
 }
 
 template <typename K, typename V>
 bool HyperMap<K, V>::containsKey(const K& key) const {
     return findKeyIndex(key) != -1;
 }
 
 template <typename K, typename V>
 V HyperMap<K, V>::get(const K& key) const {
     int idx = findKeyIndex(key);
     if (idx != -1) {
         return values.get(idx);
     }
     return V();
 }
 
 template <typename K, typename V>
 void HyperMap<K, V>::remove(const K& key) {
     int idx = findKeyIndex(key);
     if (idx != -1) {
         keys.remove(idx);
         values.remove(idx);
     }
 }
 
 template <typename K, typename V>
 int HyperMap<K, V>::size() const {
     return keys.size();
 }
 
 template <typename K, typename V>
 bool HyperMap<K, V>::isEmpty() const {
     return keys.isEmpty();
 }
 
 template <typename K, typename V>
 void HyperMap<K, V>::clear() {
     keys.clear();
     values.clear();
 }
 
 template <typename K, typename V>
 int HyperMap<K, V>::findKeyIndex(const K& key) const {
     return keys.indexOf(key);
 }
 
 template <typename K, typename V>
 V& HyperMap<K, V>::operator[](const K& key) {
     int idx = findKeyIndex(key);
     if (idx == -1) {
         // Insert new key with default value
         keys.addend(key);
         values.addend(V());
         idx = keys.size() - 1;
     }
     // Return reference to existing or newly inserted value
     return *values.getPointer(idx);
 }
 
 #endif // HYPER_MAP_HPP 