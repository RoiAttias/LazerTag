#ifndef PACKETBUFFER_HPP
#define PACKETBUFFER_HPP

#include <queue> // Include the standard queue library

/**
 * Class template for a packet buffer.
 * @tparam T Type of packet to be stored in the buffer.
 * @tparam maxSize Maximum number of packets that can be stored in the buffer.
 * @warning The buffer is not thread-safe and should not be accessed concurrently from multiple threads.
 */
template <typename T>
class PacketBuffer {
private:
    std::queue<T> packetQueue; // Queue to store the packets
    size_t maxSize; // Maximum size of the queue

public:
    // Constructor to initialize the buffer with a maximum size
    PacketBuffer(size_t size) : maxSize(size) {}

    // Method to add a packet to the queue
    bool enqueue(const T& packet) {
        // Check if the queue is full
        if (packetQueue.size() >= maxSize) return false;
        // Add the packet to the queue
        packetQueue.push(packet);
        return true;
    }

    // Method to remove a packet from the queue
    bool dequeue(T& packet) {
        // Check if the queue is empty
        if (packetQueue.empty()) return false;
        // Get the packet at the front of the queue
        packet = packetQueue.front();
        // Remove the packet from the queue
        packetQueue.pop();
        return true;
    }

    // Method to check if the queue is empty
    bool isEmpty() const {
        return packetQueue.empty();
    }

    // Method to check if the queue is full
    bool isFull() const {
        return packetQueue.size() >= maxSize;
    }

    // Method to get the current size of the queue
    size_t size() const {
        return packetQueue.size();
    }

    // Method to clear all packets from the queue
    void clear() {
        // Remove all packets from the queue
        while (!packetQueue.empty()) {
            packetQueue.pop();
        }
    }
};

#endif // PACKETBUFFER_HPP