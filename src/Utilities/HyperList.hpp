#ifndef HYPER_LIST_HPP
#define HYPER_LIST_HPP

/*
duplicate
reserve
removeValue
findAll
*/

/**
 * Node struct template for the doubly linked list.
 * @tparam T Type of data stored in the node.
 */
template <typename T>
struct Node {
    T data;           ///< Data stored in the node.
    Node<T>* next;    ///< Pointer to the next node in the list.
    Node<T>* prev;    ///< Pointer to the previous node in the list.

    /**
     * Constructor to initialize a node with a given value.
     * @param value Data to be stored in the node.
     */
    Node(T value) : data(value), next(nullptr), prev(nullptr) {}
};

/**
 * HyperList class template representing a doubly linked list.
 * @tparam T Type of data stored in the list.
 */
template <typename T>
class HyperList {
public:
    HyperList();                ///< Constructor to initialize the list.
    ~HyperList();               ///< Destructor to clear the list.

    /**
     * Adds a new node with the given value at the beginning of the list.
     * @param value Data to be stored in the new node.
     * @return The index of the new node inserted to the list (returns -1 when index out of range).
     */
    int addfront(T value);

    /**
     * Adds a new node with the given value at the end of the list.
     * @param value Data to be stored in the new node.
     * @return The index of the new node inserted to the list (returns -1 when index out of range).
     */
    int addend(T value);

    /**
     * Adds multiple nodes with data from an array at the end of the list.
     * @param array Pointer to the array of data to be added.
     * @param size Number of elements in the array.
     * @return The index of the first new node inserted to the list.
     */
    int addFromArray(T array[], int size);

    /**
     * Checks if the list contains a node with the given value.
     * @param value Data to check for in the list.
     * @return True if the value is found, false otherwise.
     */
    bool contains(T value) const;

    /**
     * Counts the number of nodes that contain the given value.
     * @param value Data to count in the list.
     * @return Number of nodes containing the value.
     */
    int count(T value) const;

    /**
     * Clears all nodes from the list.
     */
    void clear();

    /**
     * Retrieves the data from the node at the specified index.
     * @param index Position of the node to retrieve data from.
     * @return Data stored in the node at the specified index.
     */
    T get(int index);

    /**
     * Retrieves the pointer to the data from the node at the specified index.
     * @param index Position of the node to retrieve data from.
     * @return Pointer to the data stored in the node at the specified index.
     */
    T* getPointer(int index);

    /**
     * Finds the index of the first occurrence of the given value in the list.
     * @param value Data to find in the list.
     * @return The index of the first occurrence of the value, or -1 if the value is not found.
     */
    int indexOf(T value);

    /**
     * Inserts a new node with the given value at the specified index.
     * @param index Position where the new node should be inserted.
     * @param value Data to be stored in the new node.
     * @return The index of the new node inserted to the list (returns -1 when index out of range).
     */
    int insert(int index, T value);

    /**
     * Inserts multiple nodes from an array at the specified index.
     * @param index Position where the nodes should be inserted.
     * @param array Pointer to the array of data to be inserted.
     * @param size Number of elements in the array.
     */
    void insertFromArray(int index, const T* array, const int size);
    
    /**
     * Checks if the list is empty.
     * @return True if the list is empty (contains no nodes), false otherwise.
     */
    bool isEmpty();

    /**
     * Finds the index of the last occurrence of the given value in the list.
     * @param value Data to find in the list.
     * @return The index of the last occurrence of the value, or -1 if the value is not found.
     */
    int lastIndexOf(const T value);


    /**
     * Overloaded subscript operator to retrieve the data from the node at the specified index.
     * @param index Position of the node to retrieve data from.
     * @return Data stored in the node at the specified index.
     */
    T operator[](int index);

    /**
     * Replaces the data in the node at the specified index with the given value.
     * @param index Position of the node to replace.
     * @param value New data to be stored in the node.
     */
    void replace(const int index, const T value);

    /**
     * Removes the node at the specified index from the list.
     * @param index Position of the node to remove.
     */
    void remove(int index);

    /**
     * Reserves memory for the specified number of additional nodes.
     * @param additionalNodes Number of additional nodes to reserve memory for.
     * @param defaultValue Default value to initialize the new nodes with.
     */
    bool reserveMore(int additionalNodes, const T& defaultValue);

    /**
     * Reserves memory for the specified total number of nodes.
     * @param totalNodes Total number of nodes to reserve memory for.
     * @param defaultValue Default value to initialize the new nodes with.
     */
    bool reserveTotal(int totalNodes, const T& defaultValue);

    /**
     * Used the replace function to replace the value at the index with the value passed in.
     * @param index Position of the node to replace.
     * @param value New data to be stored in the node.
     */
    void set(const int index, const T value);

    /**
     * Sets the data of all nodes in the list to the given value.
     * @param value New data to be stored in all nodes.
     */
    void setAll(const T value);

    /**
     * Gets the number of nodes in the list.
     * @return Number of nodes in the list.
     */
    int size() const;

    /**
     * Stores the data of the list into an array.
     * @param array Pointer to the array where the data will be stored.
     * @param size Number of elements to store in the array.
     * @return Number of elements actually stored in the array.
     */
    int storeToArray(T* array, int size) const;

    /**
     * Switches the positions of the nodes at the specified indices.
     * @param index1 Position of the first node to switch.
     * @param index2 Position of the second node to switch.
     */
    void switchNodes(int index1, int index2);

private:
    Node<T>* head;       ///< Pointer to the first node in the list.
    Node<T>* tail;       ///< Pointer to the last node in the list.
    int listSize;        ///< Number of nodes in the list.
    Node<T>* lastNode;  ///< Cache for the most recently accessed node.
    int lastIndex;      ///< Index of the most recently accessed node.

    /**
     * Retrieves the node at the specified index.
     * @param index Position of the node to retrieve.
     * @return Pointer to the node at the specified index, or nullptr if the index is out of bounds.
     * @warning You should not store the returned pointer for future use, as it may become invalid after further operations on the list.
     */
    Node<T>* getNode(int index);
};

template <typename T>
HyperList<T>::HyperList() : head(nullptr), tail(nullptr), listSize(0), lastNode(nullptr), lastIndex(-1) {}

template <typename T>
HyperList<T>::~HyperList() {
    clear();
}

template <typename T>
int HyperList<T>::addfront(const T value) {
    return insert(0, value);
}

template <typename T>
int HyperList<T>::addend(const T value) {
    return insert(listSize, value);
}

template <typename T>
int HyperList<T>::addFromArray(T array[], int size) {
    // Check if the array pointer is null or if the size is non-positive
    if (!array || size <= 0) {
        return -1;
    }
    int result;
    // Loop through the array and add each element to the HyperList
    for (int i = 0; i < size; i++) {
        result = addend(array[i]);
    }
    return result - size + 1; // Return the index of the first new node inserted
}

template <typename T>
void HyperList<T>::clear() {
    while (head != nullptr) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
    tail = nullptr;
    listSize = 0;
    lastNode = nullptr;
    lastIndex = -1;
}

template <typename T>
bool HyperList<T>::contains(const T value) const {
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
int HyperList<T>::count(const T value) const {
    int count = 0;
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            count++;
        }
        current = current->next;
    }
    return count;
}

template <typename T>
T HyperList<T>::get(const int index) {
    Node<T>* node = getNode(index);
    if (node != nullptr) {
        return node->data;
    }
    return T(); // Return default-constructed T if index is out of bounds
}

template <typename T>
T* HyperList<T>::getPointer(const int index) {
    Node<T>* node = getNode(index);
    if (node != nullptr) {
        return &node->data;
    }
    return nullptr;
}

template <typename T>
int HyperList<T>::indexOf(const T value) {
    Node<T>* current = head;
    int index = 0;

    while (current != nullptr) {
        if (current->data == value) {
            return index;  // Return the index if value is found
        }
        current = current->next; // Go toward the tail
        index++;
    }

    return -1;  // Return -1 if the value is not found
}

template <typename T>
int HyperList<T>::insert(const int index, const T value) {
    if (index < 0 || index > listSize) {
        return -1; // Index out of bounds
    }

    Node<T>* newNode = new Node<T>(value);

    if (listSize == 0) {
        // Case: Empty list
        head = tail = newNode;
    } else if (index == 0) {
        // Case: Insert at the head
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    } else if (index == listSize) {
        // Case: Insert at the tail
        newNode->prev = tail;
        tail->next = newNode;
        tail = newNode;
    } else {
        // Case: Insert in the middle
        Node<T>* prevNode = getNode(index - 1); // Retrieve previous node
        if (prevNode != nullptr) {
            newNode->next = prevNode->next;
            newNode->prev = prevNode;
            if (prevNode->next != nullptr) {
                prevNode->next->prev = newNode;
            }
            prevNode->next = newNode;
        }
    }
    listSize++;
    return index;
}

template <typename T>
void HyperList<T>::insertFromArray(const int index, const T* array, const int size) {
    // Check if the array pointer is null or if the size is non-positive
    if (!array || size <= 0) {
        return;
    }
    
    // Loop through the array and add each element to the HyperList
    for (int i = 0; i < size; ++i) {
        insert(index + i, array[i]);
    }
}

template <typename T>
bool HyperList<T>::isEmpty() {
    return listSize == 0;  // Return true if the size is 0, false otherwise
}

template <typename T>
int HyperList<T>::lastIndexOf(const T value) {
    Node<T>* current = tail;
    int index = 0;

    while (current != nullptr) {
        if (current->data == value) {
            return index;  // Return the index if value is found
        }
        current = current->prev; // Go toward the head
        index--;
    }

    return -1;  // Return -1 if the value is not found
}

template <typename T>
T HyperList<T>::operator[](int index) {
    return get(index);
}

template <typename T>
void HyperList<T>::replace(const int index, const T value) {
    Node<T>* node = getNode(index);
    if (node != nullptr) {
        node->data = value;
    }
}

template <typename T>
void HyperList<T>::remove(const int index) {
    Node<T>* node = getNode(index);
    if (node == nullptr) return;

    if (node->prev != nullptr) node->prev->next = node->next;
    if (node->next != nullptr) node->next->prev = node->prev;
    if (node == head) head = node->next;
    if (node == tail) tail = node->prev;

    delete node;
    listSize--;
}

template <typename T>
bool HyperList<T>::reserveMore(int additionalNodes, const T& defaultValue) {
    if (additionalNodes <= 0) {
        return false; // Invalid request for non-positive node addition
    }

    // Allocate memory for additional nodes using new
    Node<T>* newNodes = new(std::nothrow) Node<T>[additionalNodes];
    if (newNodes == nullptr) {
        return false; // Memory allocation failed
    }

    // Initialize the new nodes with the default value
    for (int i = 0; i < additionalNodes; ++i) {
        newNodes[i].data = defaultValue;
        newNodes[i].next = nullptr;
        newNodes[i].prev = tail;

        if (tail != nullptr) {
            tail->next = &newNodes[i]; // Link the new node to the current tail
        }

        tail = &newNodes[i]; // Update the tail to the new node
    }

    // If the list was empty, set the head to the first new node
    if (head == nullptr) {
        head = &newNodes[0];
    }

    listSize += additionalNodes; // Update the size by the number of additional nodes
    return true; // Successful allocation and addition of nodes
}

template <typename T>
bool HyperList<T>::reserveTotal(int totalNodes, const T& defaultValue) {
    if (totalNodes <= 0) {
        return false; // Invalid total size
    }

    // If current list size is already greater than or equal to the total size, no need to reserve more
    if (listSize >= totalNodes) {
        return true; // No action needed
    }

    // Calculate how many more nodes are required
    int additionalNodes = totalNodes - listSize;

    // Use reserveMore to allocate and initialize the additional nodes
    return reserveMore(additionalNodes, defaultValue);
}

template <typename T>
void HyperList<T>::set(const int index, const T value) {
    replace(index, value);
}

template <typename T>
void HyperList<T>::setAll(const T value) {
    Node<T>* current = head;
    while (current != nullptr) {
        current->data = value;
        current = current->next;
    }
}

template <typename T>
int HyperList<T>::size() const {
    return listSize;
}

template <typename T>
int HyperList<T>::storeToArray(T* array, const int size) const {
    if (array == nullptr || size < listSize) return 0;

    Node<T>* current = head;
    int count = 0;
    while (current != nullptr && count < size) {
        array[count++] = current->data;
        current = current->next;
    }
    return count;
}

template <typename T>
void HyperList<T>::switchNodes(const int index1, const int index2) {
    if (index1 < 0 || index2 < 0 || index1 >= listSize || index2 >= listSize || index1 == index2) return;

    Node<T>* node1 = getNode(index1);
    Node<T>* node2 = getNode(index2);

    if (node1 == nullptr || node2 == nullptr) return;

    // Swap data between nodes
    T temp = node1->data;
    node1->data = node2->data;
    node2->data = temp;
}

template <typename T>
Node<T>* HyperList<T>::getNode(const int index) {
    // Checks if list is empty
    if (listSize == 0)
    {
        return nullptr;
    }

    // Check if the index is out of bounds
    if (index < 0 || index >= listSize) {
        return nullptr;
    }

    // Calculate distances from key points
    int distanceToHead = index;
    int distanceToTail = listSize - index - 1;
    int distanceToLastNode = (lastNode != nullptr) ? abs(index - lastIndex) : INT_MAX;

    // Choose the optimal direction for traversal
    Node<T>* current = nullptr;
    if (distanceToLastNode < distanceToHead && distanceToLastNode < distanceToTail) {
        // Use the lastNode as the starting point
        current = lastNode;
        if (index < lastIndex) {
            // Move backwards from lastNode
            for (int i = lastIndex; i > index; --i) {
                current = current->prev;
            }
        } else {
            // Move forwards from lastNode
            for (int i = lastIndex; i < index; ++i) {
                current = current->next;
            }
        }
    } else if (distanceToHead <= distanceToTail) {
        // Traverse from the head
        current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        // Traverse from the tail
        current = tail;
        for (int i = listSize - 1; i > index; --i) {
            current = current->prev;
        }
    }

    // Update lastNode and lastIndex for future access
    lastNode = current;
    lastIndex = index;
    return current;
}

#endif // HYPER_LIST_HPP