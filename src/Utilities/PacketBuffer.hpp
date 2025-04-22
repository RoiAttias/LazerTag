/**
 * @file packetbuffer.hpp
 * @brief Defines the PacketBuffer template, a simple fixed-size FIFO queue.
 *
 * Provides enqueue and dequeue operations with capacity limits.
 * Not thread-safe; suitable for embedded or single-threaded contexts.
 */

 #ifndef PACKETBUFFER_HPP
 #define PACKETBUFFER_HPP
 
 #include <queue>
 
 /**
  * @brief Template class for a fixed-capacity packet buffer (FIFO).
  *
  * Implements a first-in-first-out buffer with a maximum capacity.
  * Ideal for storing packets or messages in embedded applications.
  *
  * @tparam T Type of packet or data to store.
  */
 template <typename T>
 class PacketBuffer {
 public:
     /**
      * @brief Constructs a PacketBuffer with given maximum size.
      *
      * @param size Maximum number of elements the buffer can hold.
      */
     explicit PacketBuffer(size_t size) : maxSize(size) {}
 
     /**
      * @brief Adds a packet to the buffer.
      *
      * Fails if the buffer is already at maximum capacity.
      *
      * @param packet The packet to enqueue.
      * @return True if the packet was added; false if the buffer was full.
      */
     bool enqueue(const T& packet) {
         if (packetQueue.size() >= maxSize) {
             return false;
         }
         packetQueue.push(packet);
         return true;
     }
 
     /**
      * @brief Removes the oldest packet from the buffer.
      *
      * Retrieves and removes the front packet in FIFO order.
      *
      * @param packet Output reference to store the dequeued packet.
      * @return True if a packet was dequeued; false if the buffer was empty.
      */
     bool dequeue(T& packet) {
         if (packetQueue.empty()) {
             return false;
         }
         packet = packetQueue.front();
         packetQueue.pop();
         return true;
     }
 
     /**
      * @brief Checks whether the buffer is empty.
      *
      * @return True if no packets are stored, false otherwise.
      */
     bool isEmpty() const {
         return packetQueue.empty();
     }
 
     /**
      * @brief Checks whether the buffer is full.
      *
      * @return True if the number of stored packets equals the max size.
      */
     bool isFull() const {
         return packetQueue.size() >= maxSize;
     }
 
     /**
      * @brief Retrieves the current number of packets stored.
      *
      * @return Number of packets in the buffer.
      */
     size_t size() const {
         return packetQueue.size();
     }
 
     /**
      * @brief Clears all packets from the buffer.
      *
      * Empties the internal queue.
      */
     void clear() {
         while (!packetQueue.empty()) {
             packetQueue.pop();
         }
     }
 
 private:
     std::queue<T> packetQueue; ///< Underlying FIFO queue.
     size_t maxSize;            ///< Maximum number of packets the buffer can hold.
 };
 
 #endif // PACKETBUFFER_HPP 