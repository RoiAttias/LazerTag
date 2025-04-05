#ifndef CONSTANTS_COMMON_H
#define CONSTANTS_COMMON_H

#include <Arduino.h>
#include "SelectDevice.h"

// Define the version of the system
#define SYSTEM_VERSION_MAJOR 1
#define SYSTEM_VERSION_MINOR 0
#define SYSTEM_VERSION_PATCH 0

#define SYSTEM_VERSION {SYSTEM_VERSION_MAJOR, SYSTEM_VERSION_MINOR, SYSTEM_VERSION_PATCH}

// Nexus
#define NEXUS_PROJECT_ID 1
#define NEXUS_GROUPS 1 << (DEVICE_TYPE - 1)
#define NEXUS_DEVICE_ID DEVICE_ID

// Communication
#define NEXUS_GROUP_MANAGER 0x01
#define NEXUS_GROUP_GUN 0x02
#define NEXUS_GROUP_VEST 0x04


// Type Conversions
inline uint8_t deviceTypeToGroup(uint8_t deviceType) {
    switch (deviceType) {
        case DEVICE_MANAGER:
            return NEXUS_GROUP_MANAGER;
        case DEVICE_GUN:
            return NEXUS_GROUP_GUN;
        case DEVICE_VEST:
            return NEXUS_GROUP_VEST;
        default:
            return 0;
    }
}

inline uint8_t groupToDeviceType(uint8_t group) {
    switch (group) {
        case NEXUS_GROUP_MANAGER:
            return DEVICE_MANAGER;
        case NEXUS_GROUP_GUN:
            return DEVICE_GUN;
        case NEXUS_GROUP_VEST:
            return DEVICE_VEST;
        default:
            return 0;
    }
}

inline String deviceTypeString(int deviceType) {
    switch (deviceType) {
        case DEVICE_MANAGER:
            return "Manager";
        case DEVICE_GUN:
            return "Gun";
        case DEVICE_VEST:
            return "Vest";
        default:
            return "Unknown";
    }
}

inline String deviceGroupString(uint8_t deviceType) {
    switch (deviceType) {
        case NEXUS_GROUP_MANAGER:
            return "Manager";
        case NEXUS_GROUP_GUN:
            return "Gun";
        case NEXUS_GROUP_VEST:
            return "Vest";
        default:
            return "Unknown";
    }
}

#endif // CONSTANTS_COMMON_H