#ifndef CONSTANTS_COMMON_H
#define CONSTANTS_COMMON_H

#include <Arduino.h>

// Device Types
#define DEVICE_MANAGER 1
#define DEVICE_GUN 2
#define DEVICE_VEST 3

// Select device here
#define DEVICE_TYPE 1
#define DEVICE_ID 1

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

String deviceTypeString(int deviceType) {
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

String deviceGroupString(uint8_t deviceType) {
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