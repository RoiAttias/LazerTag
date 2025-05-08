/**
 * @file Constants_Common.h
 * @brief Common constants and utility functions for device configuration and Nexus communication.
 *
 * Defines system version macros, device type identifiers, Nexus project/group/device ID macros,
 * and inline conversion functions between device types, groups, and human-readable strings.
 */

 #ifndef CONSTANTS_COMMON_H
 #define CONSTANTS_COMMON_H
 
 #include <Arduino.h>
 #include "SelectDevice.h"  ///< Defines DEVICE_TYPE and DEVICE_ID prior to inclusion
 
 // -----------------------------------------------------------------------------
 // System Version
 // -----------------------------------------------------------------------------
 /// Major version of the system
 #define SYSTEM_VERSION_MAJOR 1
 /// Minor version of the system
 #define SYSTEM_VERSION_MINOR 0
 /// Patch version of the system
 #define SYSTEM_VERSION_PATCH 1
 
 /// Combined version as an array: {major, minor, patch}
 #define SYSTEM_VERSION {SYSTEM_VERSION_MAJOR, SYSTEM_VERSION_MINOR, SYSTEM_VERSION_PATCH}

// Combined version as a string: "vX.Y.Z"  
#define SYSTEM_VERSION_STRING \
String("v") + SYSTEM_VERSION_MAJOR + "." + \
SYSTEM_VERSION_MINOR + "." + \
SYSTEM_VERSION_PATCH
 
 // -----------------------------------------------------------------------------
 // Device Types
 // -----------------------------------------------------------------------------
 /// Manager device type identifier
 #define DEVICE_MANAGER 1
 /// Gun device type identifier
 #define DEVICE_GUN     2
 /// Vest device type identifier
 #define DEVICE_VEST    3
 
 // -----------------------------------------------------------------------------
 // Nexus Configuration
 // -----------------------------------------------------------------------------
 /// Project ID for Nexus network
 #define NEXUS_PROJECT_ID 1
 /// Bitmask of groups for this device (1 << (DEVICE_TYPE - 1))
 #define NEXUS_GROUPS     (1 << (DEVICE_TYPE - 1))
 /// Device ID within the Nexus network (provided by SelectDevice.h)
 #define NEXUS_DEVICE_ID  DEVICE_ID
 
 // Group identifiers for manager, gun, and vest
 #define NEXUS_GROUP_MANAGER 0x01
 #define NEXUS_GROUP_GUN     0x02
 #define NEXUS_GROUP_VEST    0x04
 
 // -----------------------------------------------------------------------------
 // Conversion Functions
 // -----------------------------------------------------------------------------
 /**
  * @brief Convert a device type constant to its corresponding Nexus group bitmask.
  *
  * @param deviceType One of DEVICE_MANAGER, DEVICE_GUN, DEVICE_VEST
  * @return Nexus group mask (NEXUS_GROUP_*) or 0 if invalid
  */
 inline uint8_t deviceTypeToGroup(uint8_t deviceType) {
     switch (deviceType) {
         case DEVICE_MANAGER: return NEXUS_GROUP_MANAGER;
         case DEVICE_GUN:     return NEXUS_GROUP_GUN;
         case DEVICE_VEST:    return NEXUS_GROUP_VEST;
         default:             return 0;
     }
 }
 
 /**
  * @brief Convert a Nexus group bitmask to its corresponding device type constant.
  *
  * @param group One of NEXUS_GROUP_MANAGER, NEXUS_GROUP_GUN, NEXUS_GROUP_VEST
  * @return DEVICE_* constant or 0 if invalid
  */
 inline uint8_t groupToDeviceType(uint8_t group) {
     switch (group) {
         case NEXUS_GROUP_MANAGER: return DEVICE_MANAGER;
         case NEXUS_GROUP_GUN:     return DEVICE_GUN;
         case NEXUS_GROUP_VEST:    return DEVICE_VEST;
         default:                  return 0;
     }
 }
 
 /**
  * @brief Get a human-readable string for a device type constant.
  *
  * @param deviceType DEVICE_MANAGER, DEVICE_GUN, or DEVICE_VEST
  * @return "Manager", "Gun", "Vest", or "Unknown"
  */
 inline String deviceTypeString(int deviceType) {
     switch (deviceType) {
         case DEVICE_MANAGER: return "Manager";
         case DEVICE_GUN:     return "Gun";
         case DEVICE_VEST:    return "Vest";
         default:             return "Unknown";
     }
 }
 
 /**
  * @brief Get a human-readable string for a Nexus group bitmask.
  *
  * @param group NEXUS_GROUP_MANAGER, NEXUS_GROUP_GUN, or NEXUS_GROUP_VEST
  * @return "Manager", "Gun", "Vest", or "Unknown"
  */
 inline String deviceGroupString(uint8_t group) {
     switch (group) {
         case NEXUS_GROUP_MANAGER: return "Manager";
         case NEXUS_GROUP_GUN:     return "Gun";
         case NEXUS_GROUP_VEST:    return "Vest";
         default:                  return "Unknown";
     }
 }
 
 #endif // CONSTANTS_COMMON_H 