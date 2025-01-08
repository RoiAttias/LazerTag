#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

TFT_eSPI tft = TFT_eSPI();   // Invoke library
TFT_eSprite img = TFT_eSprite(&tft);

ivec2 screenDiamentions;

//Screen screen(GUI_Manager_Activities,GUI_Manager_Activity::GUI_Manager_Activity_size, true);
Screen screen(nullptr,0);
Touch_XPT2046 touch(&screen, 34);
unsigned long lastMillis = 0;
// Event handlers
void IRAM_ATTR xpt2046_Handler(EventType type, uint32_t time)
{
    //touch.handleInterrupt();
}

void manager_init()
{
    tft_instance = &tft;
    img_instance = &img;
    screenDiamentions = ivec2(tft_instance->width(),tft_instance->height());
    screen.setPushHandler(push_TFT_eSPI);
    screen.init(screenDiamentions ,true,true);
    //touch.init(xpt2046_Handler,true);
}

void manager_setup()
{
    Serial.begin(115200);
    Serial.println("Setup started.");

    // TFT Initialization
    tft.begin();  
    tft.setRotation(0);
    tft.fillScreen(TFT_YELLOW);

    delay(2000);
    Serial.println("TFT initialized.");

    // Sprite Initialization
    if (img.createSprite(100,100)) {
        img.fillSprite(TFT_BLACK); 
        Serial.println("Sprite created successfully.");
    } else {
        Serial.println("Error: Sprite creation failed.");
    }

    manager_init();
    Serial.println("Manager initialized.");

    img.drawCircle(50, 50, 10, TFT_CYAN);
    screenDiamentions.display();
    delay(2000);
    screen.renderNpush();
    delay(2000);

    Serial.println("Setup completed.");
}

void manager_loop()
{
    
    if (screen.shouldRender())
    {
        Serial.print(" Rendering");
        img.fillSprite(TFT_BLACK);
        //img.drawString(touch.lastPoint.toString(),0,0);
        img.setTextSize(1); // Ensure text size is set (default is 1).
        img.setTextFont(1); // Use a valid font ID (1-4 for built-in fonts in TFT_eSPI).
        img.setTextColor(TFT_WHITE, TFT_BLACK); // White text on black background.
        img.fillCircle(50, 50, 10, random(0xFFFF));
        String text = String(millis());
        img.setCursor(20, 30);
        screen.renderNpush();
        tft.print(text);
        Serial.println(" is Finished");

        //screen.renderNpush();
    }
    
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    //delay(10); // Optional: Small delay to stabilize processing

    if (millis() - lastMillis >= 1000) {
        lastMillis = millis();
        screen.callRender();
        Serial.printf("LOOP - %lu\n", millis());
    }

}

#endif // MANAGER_MAIN_HPP