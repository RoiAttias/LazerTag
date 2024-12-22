#ifndef MANAGER_MAIN_HPP
#define MANAGER_MAIN_HPP

#include <Arduino.h>
//#include "Subsystems/GUI_Manager/GUI_Manager.hpp"

#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();   // Invoke library
TFT_eSprite img = TFT_eSprite(&tft);

void manager_setup()
{
    tft.begin();     // initialize a ST7789 chip
    tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness
    
    tft.fillScreen(TFT_BLACK);
    tft.setRotation(0);

    //img.createSprite(240, 250); //This works       
    img.createSprite(240, 320); //Blank screen
    img.fillSprite(TFT_BLACK); 

}

void manager_loop()
{
    img.drawRect(0, 0, 240, 240, TFT_WHITE);
    img.pushSprite(0, 0);
    // The rest of the program primarily relies on interrupts, so no logic is needed here
    delay(10); // Optional: Small delay to stabilize processing
}

#endif // MANAGER_MAIN_HPP