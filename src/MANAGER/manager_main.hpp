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

    Dashboard *dashboard = (Dashboard*)GUI_Manager_Activities[GUI_Manager_Activity::DASHBOARD];
    DeviceBox *deviceBox;
    deviceBox = &dashboard->deviceBox1;
    deviceBox->invertSelected();
    deviceBox = &dashboard->deviceBox4;
    deviceBox->invertSelected();
}

void manager_loop()
{   
    Nexus::loop();
    GUI::loop();

    Dashboard *dashboard = (Dashboard*)GUI_Manager_Activities[GUI_Manager_Activity::DASHBOARD];
    DeviceBox *deviceBox;
    deviceBox = &dashboard->deviceBox1;
    deviceBox->invertSelected();
    deviceBox = &dashboard->deviceBox2;
    deviceBox->invertSelected();
    deviceBox = &dashboard->deviceBox3;
    deviceBox->invertSelected();
    deviceBox = &dashboard->deviceBox4;
    deviceBox->invertSelected();
    
    GUI::callRender();
    delay(1000);
}

#endif // MANAGER_MAIN_HPP