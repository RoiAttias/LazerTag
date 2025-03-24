#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include "Common/Constants_Common.h"
#include "Common/LazerTagPacket.hpp"
#include "Components/Nexus/Nexus.hpp"
#include "Modules/Game.hpp"
#include "GUI_Manager/GUI_Manager.hpp"

Game game;

void manager_setup()
{
    Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));
    GUI::init();

    Scanner *scanner = (Scanner*)GUI_Manager_Activities[GUI_Manager_Activity::SCANNER];
    DeviceBox *deviceBox = (DeviceBox*)scanner->elements[6];
    deviceBox->invertSelected();
}

void manager_loop()
{   
    Nexus::loop();
    GUI::loop();
}

#endif // MANAGER_MAIN_HPP