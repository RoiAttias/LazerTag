#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include "Common/Constants_Common.h"
#include "Common/LazerTagPacket.hpp"
#include "Components/Nexus/Nexus.hpp"
#include "Modules/Game.hpp"
#include "GUI_Manager/GUI_Manager.hpp"

Game game;

void scanCompletedCallback()
{
    Serial.println("Scan completed");
    for (int i = 0; i < Nexus::scanResults.size(); i++)
        Serial.println(Nexus::scanResults[i].toString());
    
    scanner->setScannedDevices(Nexus::scanResults);
    Serial.println("Devices scanned");
    GUI::callRender();
}


void manager_setup()
{
    Serial.begin(115200);
    Nexus::begin(NexusAddress(NEXUS_PROJECT_ID, NEXUS_GROUPS, NEXUS_DEVICE_ID));
    Nexus::onScanComplete = scanCompletedCallback;
    GUI::init();
    Serial.println(Nexus::THIS_ADDRESS.toString());
    Serial.println("Setup complete");
}

void manager_loop()
{   
    Nexus::loop();
    GUI::loop();
}

#endif // MANAGER_MAIN_HPP