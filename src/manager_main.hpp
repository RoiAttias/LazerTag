#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>

#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

void manager_setup()
{
    GUI::init();
}

void manager_loop()
{   
    GUI::loop();
}

#endif // MANAGER_MAIN_HPP