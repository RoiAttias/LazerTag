#define DEVICE_TYPE 2

#ifndef DEVICE_TYPE
#error "DEVICE_TYPE is not defined! Define DEVICE_TYPE in your build flags (e.g., -DDEVICE_MANAGER)."
#endif

#if DEVICE_TYPE == 1
#include "manager_main.cpp"
#elif DEVICE_TYPE == 2
#include "gun_main.cpp"
#elif DEVICE_TYPE == 3
#include "vest_main.cpp"
#else
#error "Invalid DEVICE_TYPE specified!"
#endif