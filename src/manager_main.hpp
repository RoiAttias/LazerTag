#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

static void IRAM_ATTR PushbuttonISR();

TFT_eSPI tft = TFT_eSPI();   // Invoke library

ivec2 screenDiamentions;
Screen screen(GUI_Manager_Activities,GUI_Manager_Activity::GUI_Manager_Activity_size, true);
Pushbutton isr(34, 100, false, PushbuttonISR);
Touch_XPT2046 touch(&screen, &isr);

bool lastMillis = false;

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
    tft.fillScreen(TFT_YELLOW);
    tft.getTouchRawZ();
    Serial.println("TFT initialized.");

    // TGUI Initialization
    TGUI::tft_instance = &tft;
    screenDiamentions = ivec2(TGUI::tft_instance->width(), TGUI::tft_instance->height());
    screen.init(screenDiamentions ,true);
    touch.init(ENABLE_ALL|ENABLE_PRESS);
    //screen.selectActivity(GUI_Manager_Activity::ACTIVATION);
    //screen.callRender();
    delay(1000);
    digitalWrite(2, LOW);
    Serial.println("TGUI initialized.");
}

void manager_loop()
{   
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
    /*
    touch.loop();
    
    touch.getLastPoint().display();
    tft.drawPixel(touch.getLastPoint().x, touch.getLastPoint().y, TFT_BLACK);
    */
    
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