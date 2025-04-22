/**
 * @file macaddress.hpp
 * @brief Defines the MacAddress struct for representing and manipulating 6-byte MAC addresses.
 *
 * Provides constructors, string conversion, element access, comparison, and buffer utilities
 * for handling MAC addresses in Arduino projects.
 */

 #ifndef MACADDRESS_HPP
 #define MACADDRESS_HPP
 
 #include <Arduino.h>
 
 /**
  * @brief Struct representing a 6-byte MAC address.
  *
  * Stores MAC address bytes and provides utility methods for conversion,
  * buffer extraction, element access, and comparison.
  */
 struct MacAddress {
     uint8_t addr[6];  ///< Array storing the 6 bytes of the MAC address.
 
     /**
      * @brief Default constructor; initializes address to 00:00:00:00:00:00.
      */
     MacAddress() {
         memset(addr, 0, sizeof(addr));
     }
 
     /**
      * @brief Constructs a MAC address from a byte array.
      *
      * Copies 6 bytes from the input array into this address.
      *
      * @param macAddr Pointer to a 6-byte array containing the MAC address.
      */
     MacAddress(const uint8_t* macAddr) {
         memcpy(addr, macAddr, sizeof(addr));
     }
 
     /**
      * @brief Converts the MAC address to a human-readable string.
      *
      * Formats the 6 bytes as uppercase hexadecimal pairs separated by colons.
      * Example: "AA:BB:CC:11:22:33".
      *
      * @return Arduino String representation of the MAC address.
      */
     String toString() const {
         char buf[18];
         sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X",
                 addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
         return String(buf);
     }
 
     /**
      * @brief Copies the MAC address bytes into an external buffer.
      *
      * @param buf Pointer to a buffer of at least 6 bytes where the address is stored.
      */
     void toBuffer(uint8_t* buf) const {
         memcpy(buf, addr, sizeof(addr));
     }
 
     /**
      * @brief Read-only element access by byte index.
      *
      * Returns the byte at the specified index (0-5), or 0 if out of bounds.
      *
      * @param index Byte index (0 for first byte, 5 for last).
      * @return Byte value at the index, or 0 if index is invalid.
      */
     uint8_t operator[](int index) const {
         if (index < 0 || index >= 6) {
             return 0;
         }
         return addr[index];
     }
 
     /**
      * @brief Mutable element access by byte index.
      *
      * Provides a reference to the specified byte for modification.
      * No bounds checking is performed.
      *
      * @param index Byte index (0-5).
      * @return Reference to the byte at the specified index.
      */
     uint8_t& operator[](int index) {
         return addr[index];
     }
 
     /**
      * @brief Assigns a new MAC address from a byte array.
      *
      * Copies 6 bytes into this address.
      *
      * @param macAddr Pointer to a 6-byte array to copy from.
      * @return Reference to this MacAddress instance.
      */
     MacAddress& operator=(const uint8_t* macAddr) {
         memcpy(addr, macAddr, sizeof(addr));
         return *this;
     }
 
     /**
      * @brief Compares two MAC addresses for equality.
      *
      * @param other MacAddress to compare against.
      * @return True if all 6 bytes match, false otherwise.
      */
     bool operator==(const MacAddress& other) const {
         return memcmp(addr, other.addr, sizeof(addr)) == 0;
     }
 
     /**
      * @brief Compares two MAC addresses for inequality.
      *
      * @param other MacAddress to compare against.
      * @return True if any byte differs, false if identical.
      */
     bool operator!=(const MacAddress& other) const {
         return !(*this == other);
     }
 };
 
 #endif // MACADDRESS_HPP 