#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include "Subsystems/GUI_Manager/GUI_Manager.hpp"
#include "Utilities/HyperList.hpp"

void manager_setup()
{
    Serial.begin(115200);
}

void manager_loop()
{
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    delay(10); // Optional: Small delay to stabilize processing
}

#endif // MANAGER_MAIN_HPP