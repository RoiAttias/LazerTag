#ifndef MACADDRESS_HPP
#define MACADDRESS_HPP

#include <Arduino.h>

/**
 * @brief A struct representing a MAC address.
 * @details This struct provides methods for converting a MAC address to a string and etc.
 */
struct MacAddress {
    uint8_t addr[6]{}; // 6 bytes for the MAC address

    /**
     * @brief Default constructor for a MAC address.
     */
    MacAddress() = default;

    /**
     * @brief Constructor with a MAC address array input (6 Bytes).
     * @param macAddr The MAC address array (6 Bytes).
     */
    MacAddress(const uint8_t* macAddr) {
        memcpy(addr, macAddr, 6);
    }

    /**
     * @brief MAC address to string conversion.
     * @return The MAC address as a string.
     */
    String toString() const {
        char buf[18];
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
        return String(buf);
    }

    /**
     * @brief Get the MAC address to an array of bytes.
     * @param buf The buffer to store the MAC address bytes.
     */
    void toBuffer(uint8_t* buf) const
    {
        memcpy(buf, addr, 6);
    }

    /**
     * @brief Get the value of one byte in the MAC address.
     * @param index The index of the byte to get.
     * @return The value of the byte at the specified index, or 0 if the index is out of bounds.
     */
    uint8_t operator[](int index) const
    {
        if (index < 0 || index >= 6) {
            // Handle out-of-bounds access, e.g., return 0 or throw an exception
            return 0; // or throw std::out_of_range("Index out of bounds");
        }
        return addr[index];
    }

    /**
     * @brief Set one byte of the MAC address.
     * @param index The index of the byte to set.
     * @return A reference to the byte at the specified index.
     */
    uint8_t& operator[](int index)
    {
        return addr[index];
    }

    /**
     * @brief Assignment operator for a MAC address array.
     * @param macAddr The MAC address array to assign.
     * @return A reference to the MAC address.
     */
    MacAddress& operator=(const uint8_t* macAddr)
    {
        memcpy(addr, macAddr, 6);
        return *this;
    }

    /**
     * @brief Equality operator for MAC addresses.
     * @param other The MAC address to compare.
     * @return True if the MAC addresses are equal, false otherwise.
     */
    bool operator==(const MacAddress& other) const
    {
        return memcmp(addr, other.addr, 6) == 0; // memcmp returns 0 if the two arrays are equal
    }

    /**
     * @brief Inequality operator for MAC addresses.
     * @param other The MAC address to compare.
     * @return True if the MAC addresses are not equal, false otherwise.
     */
    bool operator!=(const MacAddress& other) const
    {
        return !(*this == other); // Use the == operator to implement the != operator
    }
};

#endif // MACADDRESS_HPP
