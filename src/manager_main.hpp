#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
//#include "Subsystems/GUI_Manager/GUI_Manager.hpp"
#include <TFT_eSPI.h>
#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

TFT_eSPI tft = TFT_eSPI();   // Invoke library
TFT_eSprite img = TFT_eSprite(&tft);


Screen screen(GUI_Manager_Activities,GUI_Manager_Activity::GUI_Manager_Activity_size, true);
//Touch_XPT2046 touch(&screen, 34);

// Event handlers
void IRAM_ATTR xpt2046_Handler(EventType type, uint32_t time)
{
    //touch.handleInterrupt();
}

void manager_init()
{
    tft_instance = &tft;
    img_instance = &img;
    screen.setPushHandler(push_TFT_eSPI);
    screen.init(ivec2(tft_instance->width(),tft_instance->height()),true,true);
    //touch.init(xpt2046_Handler,true);
}

void manager_setup()
{
    Serial.begin(115200);
    Serial.println("Hello");
        Serial.println("hi");

    tft.begin();     // initialize a ST7789 chip    
    tft.setRotation(0);
    tft.fillScreen(TFT_YELLOW);

    delay(2000);

    //img.createSprite(240, 250); //This works       
    img.createSprite(100, 100); //Blank screen
    img.fillSprite(TFT_BLACK); 
    Serial.println("there");

    manager_init();

    img.drawCircle(50,50,10,TFT_CYAN);
    screen.renderNpush();
    delay(2000);
    
    Serial.println("World!");
}

void manager_loop()
{
    
    if (screen.shouldRender())
    {
        img.fillSprite(TFT_BLACK);
        //img.drawString(touch.lastPoint.toString(),0,0);
        img.setTextColor(rand());
        img.drawString(String(millis()),100, 100);
        screen.renderNpush();
    }
    
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    //delay(10); // Optional: Small delay to stabilize processing

    if(millis() % 1000 == 0)
    {
        screen.callRender();
        Serial.printf("LOOP - %d", millis());
    }
}

#endif // MANAGER_MAIN_HPP