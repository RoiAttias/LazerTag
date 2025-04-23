/**
 * @file main.cpp
 * @brief Unified entry point that dispatches to the manager, gun, or vest application.
 *
 * Depending on the DEVICE_TYPE macro, includes the appropriate main header
 * (manager_main.hpp, gun_main.hpp, or vest_main.hpp) and calls their
 * setup() and loop() functions in the Arduino framework.
 */

 #include "SelectDevice.h"  ///< Defines DEVICE_TYPE and DEVICE_ID

 #ifndef DEVICE_TYPE
 #error "DEVICE_TYPE is not defined! Define DEVICE_TYPE in your build flags."
 #endif
 
 // Select the correct module header based on DEVICE_TYPE
 #if DEVICE_TYPE == DEVICE_MANAGER
 #include "MANAGER/manager_main.hpp"
 #elif DEVICE_TYPE == DEVICE_GUN
 #include "GUN/gun_main.hpp"
 #elif DEVICE_TYPE == DEVICE_VEST
 #include "VEST/vest_main.hpp"
 #elif DEVICE_TYPE == 0
 #warning "DEVICE_TYPE is set to 0: no application code will be compiled."
 #else
 #error "Invalid DEVICE_TYPE specified!"
 #endif
 
 #include <Arduino.h>
 
 /**
  * @brief Arduino setup function.
  *
  * Calls manager_setup(), gun_setup(), or vest_setup() based on DEVICE_TYPE.
  */
 void setup() {
 #if DEVICE_TYPE == DEVICE_MANAGER
     manager_setup();
 #elif DEVICE_TYPE == DEVICE_GUN
     gun_setup();
 #elif DEVICE_TYPE == DEVICE_VEST
     vest_setup();
 #endif
 }
 
 /**
  * @brief Arduino loop function.
  *
  * Continuously calls manager_loop(), gun_loop(), or vest_loop()
  * based on DEVICE_TYPE.
  */
 void loop() {
 #if DEVICE_TYPE == DEVICE_MANAGER
     manager_loop();
 #elif DEVICE_TYPE == DEVICE_GUN
     gun_loop();
 #elif DEVICE_TYPE == DEVICE_VEST
     vest_loop();
 #endif
 } 