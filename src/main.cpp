#define DEVICE_MANAGER 1
#define DEVICE_GUN 2
#define DEVICE_VEST 3

// Select device here
#define DEVICE_TYPE DEVICE_VEST

#ifndef DEVICE_TYPE
#error "DEVICE_TYPE is not defined! Define DEVICE_TYPE in your build flags (e.g., -DDEVICE_MANAGER)."
#endif

#if DEVICE_TYPE == DEVICE_MANAGER
#include "manager_main.hpp"
#elif DEVICE_TYPE == DEVICE_GUN
#include "gun_main.hpp"
#elif DEVICE_TYPE == DEVICE_VEST
#include "vest_main.hpp"
#elif DEVICE_TYPE == 0
#warning "DEVICE_TYPE is set to 0! Code will be empty."
#else
#error "Invalid DEVICE_TYPE specified!"
#endif

#include <Arduino.h>

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
