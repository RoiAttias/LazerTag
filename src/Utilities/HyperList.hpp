#ifndef HYPER_LIST_HPP
#define HYPER_LIST_HPP

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
     * Adds a new node with the given value at the end of the list.
     * @param value Data to be stored in the new node.
     */
    void add(T value);

    /**
     * Adds a new node with the data from the given pointer at the end of the list.
     * @param valuePtr Pointer to the data to be stored in the new node.
     */
    void add(T* valuePtr);

    /**
     * Adds multiple nodes with data from an array at the end of the list.
     * @param array Pointer to the array of data to be added.
     * @param size Number of elements in the array.
     */
    void addFromArray(T* array, int size);

    /**
     * Checks if the list contains a node with the given value.
     * @param value Data to check for in the list.
     * @return True if the value is found, false otherwise.
     */
    bool contains(T value) const;

    /**
     * Checks if the list contains a node with the data pointed to by the given pointer.
     * @param valuePtr Pointer to the data to check for in the list.
     * @return True if the value is found, false otherwise.
     */
    bool contains(T* valuePtr) const;

    /**
     * Counts the number of nodes that contain the given value.
     * @param value Data to count in the list.
     * @return Number of nodes containing the value.
     */
    int count(T value) const;

    /**
     * Counts the number of nodes that contain the data pointed to by the given pointer.
     * @param valuePtr Pointer to the data to count in the list.
     * @return Number of nodes containing the value.
     */
    int count(T* valuePtr) const;

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
     * Retrieves the address of the data from the node at the specified index.
     * @param index Position of the node to retrieve the address from.
     * @return Pointer to the data stored in the node at the specified index, or nullptr if the index is out of bounds.
     */
    T* getAddress(int index);

    /**
     * Gets the number of nodes in the list.
     * @return Number of nodes in the list.
     */
    int size() const;

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
     */
    void insert(int index, T value);

    /**
     * Inserts a new node with the data from the given pointer at the specified index.
     * @param index Position where the new node should be inserted.
     * @param valuePtr Pointer to the data to be stored in the new node.
     */
    void insert(int index, T* valuePtr);

    /**
     * Inserts multiple nodes from an array at the specified index.
     * @param index Position where the nodes should be inserted.
     * @param array Pointer to the array of data to be inserted.
     * @param size Number of elements in the array.
     */
    void insertFromArray(int index, T* array, int size);
    /**
     * Checks if the list is empty.
     * @return True if the list is empty (contains no nodes), false otherwise.
     */
    bool isEmpty();

    /**
     * Replaces the data in the node at the specified index with the given value.
     * @param index Position of the node to replace.
     * @param value New data to be stored in the node.
     */
    void replace(int index, T value);

    /**
     * Replaces the data in the node at the specified index with the data from the given pointer.
     * @param index Position of the node to replace.
     * @param valuePtr Pointer to the new data.
     */
    void replace(int index, T* valuePtr);

    /**
     * Removes the node at the specified index from the list.
     * @param index Position of the node to remove.
     */
    void remove(int index);

    /**
     * Removes the node at the specified index if the data matches the data pointed to by the given pointer.
     * @param index Position of the node to remove.
     * @param valuePtr Pointer to the data to match.
     */
    void remove(int index, T* valuePtr);

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
     */
    Node<T>* getNode(int index) const;
};

template <typename T>
HyperList<T>::HyperList() : head(nullptr), tail(nullptr), listSize(0), lastNode(nullptr), lastIndex(-1) {}

template <typename T>
HyperList<T>::~HyperList() {
    clear();
}

template <typename T>
void HyperList<T>::add(T value) {
    insert(listSize, value);
}

template <typename T>
void HyperList<T>::add(T* valuePtr) {
    insert(listSize, valuePtr);
}

template <typename T>
void HyperList<T>::addFromArray(T* array, int size) {
    if (array == nullptr || size <= 0) return;
    for (int i = 0; i < size; ++i) {
        add(array[i]);
    }
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
bool HyperList<T>::contains(T value) const {
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
bool HyperList<T>::contains(T* valuePtr) const {
    if (valuePtr == nullptr) return false;

    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == *valuePtr) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template <typename T>
int HyperList<T>::count(T value) const {
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
int HyperList<T>::count(T* valuePtr) const {
    if (valuePtr == nullptr) return 0;

    int count = 0;
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == *valuePtr) {
            count++;
        }
        current = current->next;
    }
    return count;
}

template <typename T>
T HyperList<T>::get(int index) {
    Node<T>* node = getNode(index);
    if (node != nullptr) {
        return node->data;
    }
    return T(); // Return default-constructed T if index is out of bounds
}

template <typename T>
T* HyperList<T>::getAddress(int index) {
    Node<T>* node = getNode(index);
    if (node != nullptr) {
        return &(node->data);
    }
    return nullptr; // Return nullptr if index is out of bounds
}

template <typename T>
int HyperList<T>::indexOf(T value) {
    Node<T>* current = head;
    int index = 0;

    while (current != nullptr) {
        if (current->data == value) {
            return index;  // Return the index if value is found
        }
        current = current->next;
        index++;
    }

    return -1;  // Return -1 if the value is not found
}

template <typename T>
void HyperList<T>::insert(int index, T value) {
    if (index < 0 || index > listSize) return;

    Node<T>* newNode = new Node<T>(value);

    if (index == 0) {
        newNode->next = head;
        if (head != nullptr) head->prev = newNode;
        head = newNode;
        if (tail == nullptr) tail = newNode;
    } else if (index == listSize) {
        newNode->prev = tail;
        if (tail != nullptr) tail->next = newNode;
        tail = newNode;
        if (head == nullptr) head = newNode;
    } else {
        Node<T>* prevNode = getNode(index - 1);
        if (prevNode != nullptr) {
            newNode->next = prevNode->next;
            newNode->prev = prevNode;
            if (prevNode->next != nullptr) prevNode->next->prev = newNode;
            prevNode->next = newNode;
        }
    }

    listSize++;
}

template <typename T>
void HyperList<T>::insert(int index, T* valuePtr) {
    if (valuePtr != nullptr) {
        insert(index, *valuePtr);
    }
}

template <typename T>
void HyperList<T>::insertFromArray(int index, T* array, int size) {
    if (array == nullptr || size <= 0) return;
    for (int i = 0; i < size; ++i) {
        insert(index + i, array[i]);
    }
}

template <typename T>
bool HyperList<T>::isEmpty() {
    return listSize == 0;  // Return true if the size is 0, false otherwise
}


template <typename T>
void HyperList<T>::replace(int index, T value) {
    Node<T>* node = getNode(index);
    if (node != nullptr) {
        node->data = value;
    }
}

template <typename T>
void HyperList<T>::replace(int index, T* valuePtr) {
    if (valuePtr != nullptr) {
        replace(index, *valuePtr);
    }
}

template <typename T>
void HyperList<T>::remove(int index) {
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
void HyperList<T>::remove(int index, T* valuePtr) {
    if (valuePtr == nullptr) return;

    Node<T>* node = getNode(index);
    if (node != nullptr && node->data == *valuePtr) {
        remove(index);
    }
}

template <typename T>
int HyperList<T>::storeToArray(T* array, int size) const {
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
void HyperList<T>::switchNodes(int index1, int index2) {
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
Node<T>* HyperList<T>::getNode(int index) const {
    if (index < 0 || index >= listSize) return nullptr;

    if (index == lastIndex && lastNode != nullptr) {
        return lastNode;
    }

    Node<T>* current = head;
    if (index < listSize / 2) {
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
    } else {
        current = tail;
        for (int i = listSize - 1; i > index; --i) {
            current = current->prev;
        }
    }

    lastNode = current;
    lastIndex = index;
    return current;
}

#endif // HYPER_LIST_HPP