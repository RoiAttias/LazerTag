#include "SelectDevice.h"

#ifndef DEVICE_TYPE
#error "DEVICE_TYPE is not defined! Define DEVICE_TYPE in your build flags (DEVICE_MANAGER/DEVICE_GUN/DEVICE_VEST)."
#endif

// Include the appropriate header file based on the device type
#if DEVICE_TYPE == DEVICE_MANAGER
#include "MANAGER/manager_main.hpp"
#elif DEVICE_TYPE == DEVICE_GUN
#include "GUN/gun_main.hpp"
#elif DEVICE_TYPE == DEVICE_VEST
#include "VEST/vest_main.hpp"
#elif DEVICE_TYPE == 0
#warning "DEVICE_TYPE is set to 0! Code will be empty."
#else
#error "Invalid DEVICE_TYPE specified!"
#endif

#include <Arduino.h>

// Setup calls the setup function for the selected device type
void setup()
{
#if DEVICE_TYPE == DEVICE_MANAGER
    manager_setup();
#elif DEVICE_TYPE == DEVICE_GUN
    gun_setup();
#elif DEVICE_TYPE == DEVICE_VEST
    vest_setup();
#endif
}

// Loop calls the loop function for the selected device type
void loop()
{
#if DEVICE_TYPE == DEVICE_MANAGER
    manager_loop();
#elif DEVICE_TYPE == DEVICE_GUN
    gun_loop();
#elif DEVICE_TYPE == DEVICE_VEST
    vest_loop();
#endif
}
