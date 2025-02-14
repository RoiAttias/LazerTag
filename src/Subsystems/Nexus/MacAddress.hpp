#ifndef MACADDRESS_HPP
#define MACADDRESS_HPP

/**
 * @brief A struct representing a MAC address.
 * @details This struct provides methods for converting a MAC address to a string and etc.
 */
struct MacAddress {
    uint8_t mac[6]{}; // 6 bytes for the MAC address, initialized to 0

    MacAddress(const uint8_t* macArray) {
        memcpy(mac, macArray, 6);
    }

    String toString() const {
        char buf[18];
        sprintf(buf, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return String(buf);
    }

    void toBytes(uint8_t* buf) const
    {
        memcpy(buf, mac, 6);
    }

    uint8_t operator[](int index) const
    {
        return mac[index];
    }

    uint8_t& operator[](int index)
    {
        return mac[index];
    }

    MacAddress& operator=(const uint8_t* macArray)
    {
        memcpy(mac, macArray, 6);
        return *this;
    }

    bool operator==(const MacAddress& other) const
    {
        return memcmp(mac, other.mac, 6) == 0; // memcmp returns 0 if the two arrays are equal
    }

    bool operator!=(const MacAddress& other) const
    {
        return !(*this == other); // Use the == operator to implement the != operator
    }
};

#endif // MACADDRESS_HPP
