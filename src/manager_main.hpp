#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

static void IRAM_ATTR PushbuttonISR();

TFT_eSPI tft = TFT_eSPI();   // Invoke library

ivec2 screenDiamentions;
Pushbutton isr(34, 100, false, PushbuttonISR);

Screen screen(true);
Touch_XPT2046 touch(&screen);

static void IRAM_ATTR PushbuttonISR()
{
    isr.handleInterrupt();
}

void manager_setup()
{
    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);
    Serial.begin(115200);
    Serial.println("Setup started.");

    // TFT Initialization
    tft.begin();  
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    tft.getTouchRawZ();
    Serial.println("TFT initialized.");

    // TGUI Initialization
    TGUI::tft_instance = &tft;
    screenDiamentions = ivec2(TGUI::tft_instance->width(), TGUI::tft_instance->height());
    screen.init(screenDiamentions, GUI_Manager_Activities, GUI_Manager_Activity_size, true);
    touch.init(ENABLE_PRESS | ENABLE_RELEASE);
    delay(1000);
    digitalWrite(2, LOW);
    Serial.println("TGUI initialized.");
    
    screen.selectActivity(GUI_Manager_Activity::ACTIVATION);
    Serial.println("Activation selected.");
    screen.callRender();
    screen.render();
    delay(3000);
    
    screen.selectActivity(GUI_Manager_Activity::DASHBOARD);
    Serial.println("Dashboard selected.");
    screen.callRender();
    Serial.println("Start render - Dashboard.");
    screen.render();
}

void manager_loop()
{   
    /*
    if (isr.hasPressed())
    {
        digitalWrite(2, HIGH);
        Serial.println("Pressed");
    }
    if (isr.hasReleased())
    {
        digitalWrite(2, LOW);
        Serial.println("Released");
    }
    */
    touch.loop();
    
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    delay(10); // Optional: Small delay to stabilize processing
/*
    if (millis() - lastMillis >= 1000) {
        lastMillis = millis();
        screen.callRender();
        Serial.printf("LOOP - %lu\n", millis());
    }
    */

}

#endif // MANAGER_MAIN_HPP