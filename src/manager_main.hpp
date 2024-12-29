#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
//#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();   // Invoke library
TFT_eSprite img = TFT_eSprite(&tft);

#include "Subsystems/GUI_Manager/GUI_Manager.hpp"


Screen screen(GUI_Manager_Activities,GUI_Manager_Activity::GUI_Manager_Activity_size, true);
Touch_XPT2046 touch(&screen, 34);

// Event handlers
void xpt2046_Handler(EventType type, uint32_t time)
{
    touch.handleInterrupt();
}

void manager_init()
{
    tft_instance = &tft;
    img_instance = &img;
    screen.init(ivec2(tft_instance->width(),tft_instance->height()));
    touch.init(xpt2046_Handler,true);
}

void manager_setup()
{
    tft.begin();     // initialize a ST7789 chip
    tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness
    
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);

    //img.createSprite(240, 250); //This works       
    img.createSprite(tft.width(), tft.height()); //Blank screen
    img.fillSprite(TFT_BLACK); 

    manager_init();
}

void manager_loop()
{
    if (screen.shouldRender())
    {
        img.fillSprite(TFT_BLACK);
        img.drawString(touch.lastPoint.toString(),0,0);
        screen.renderNpush();
    }
    
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    delay(10); // Optional: Small delay to stabilize processing
}

#endif // MANAGER_MAIN_HPP