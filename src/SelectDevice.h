/**
 * @file SelectDevice.h
 * @brief Configuration header to specify the current device type and ID for the build.
 *
 * This file defines DEVICE_TYPE and DEVICE_ID, which determine which
 * main application module is compiled and run:
 *   - DEVICE_MANAGER (1)
 *   - DEVICE_GUN     (2)
 *   - DEVICE_VEST    (3)
 *
 * Also documents the mapping of IDs for managers, guns, and vests.
 */

 #ifndef SELECT_DEVICE_H
 #define SELECT_DEVICE_H
 
 #include "Common/Constants_Common.h"  ///< Provides DEVICE_MANAGER, DEVICE_GUN, DEVICE_VEST macros
 
 /**
  * @note Device Types:
  *   1 - DEVICE_MANAGER
  *   2 - DEVICE_GUN
  *   3 - DEVICE_VEST
  */
 
 /**
  * @note Device IDs:
  *   1 - Manager
  *   2 - Gun 1
  *   3 - Vest 1
  *   4 - Gun 2
  *   5 - Vest 2
  */
 
 /// @brief Specify which device this firmware is built for
 #define DEVICE_TYPE DEVICE_MANAGER
 
 /// @brief Unique identifier for this device instance
 #define DEVICE_ID   1
 
 #endif // SELECT_DEVICE_H