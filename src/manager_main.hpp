#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

TFT_eSPI tft = TFT_eSPI();   // Invoke library

ivec2 screenDiamentions;

Screen screen(GUI_Manager_Activities,GUI_Manager_Activity::GUI_Manager_Activity_size, true);
//Screen screen(nullptr,0);
Touch_XPT2046 touch(&screen, Pushbutton(34, 10));
unsigned long lastMillis = 0;

void manager_setup()
{
    Serial.begin(115200);
    Serial.println("Setup started.");

    // TFT Initialization
    tft.begin();  
    tft.setRotation(0);
    tft.fillScreen(TFT_YELLOW);
    Serial.println("TFT initialized.");

    // TGUI Initialization
    TGUI::tft_instance = &tft;
    screenDiamentions = ivec2(TGUI::tft_instance->width(), TGUI::tft_instance->height());
    screen.init(screenDiamentions ,true);
    touch.init(ENABLE_ALL|ENABLE_PRESS);
    Serial.println("TGUI initialized.");
}

void manager_loop()
{
    touch.loop();
    if (screen.shouldRender())
    {
        screen.render();
    }
    
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    //delay(10); // Optional: Small delay to stabilize processing
/*
    if (millis() - lastMillis >= 1000) {
        lastMillis = millis();
        screen.callRender();
        Serial.printf("LOOP - %lu\n", millis());
    }
    */

}

#endif // MANAGER_MAIN_HPP