#ifndef GUN_MAIN_HPP
#define GUN_MAIN_HPP

#include <Arduino.h>
#include "Components/IRremoteESP32/IRsender.hpp"
#include "Components/Pushbutton/Pushbutton.hpp"
#include "Components/Visualizer/Visualizer.hpp"
#include "Modules/Gun.hpp"
#include <U8g2lib.h>

#include "Utilities/HyperList.hpp"
#include "Utilities/MoreMath.hpp"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

// Define pin configurations
const uint8_t buttonPin = 27; // Replace with your button pin
const uint8_t irPin = 4;     // Replace with your IR LED pin

// Define the IR signal
const uint32_t fireSignal = 0x20DF10EF; // Example NEC signal for "fire"

volatile bool shotNow = false;
volatile int counter = 0;

// Instances of classes
//Pushbutton trigger(buttonPin);
IRsender irSender(irPin, 0, 38000); // Using channel 0 and 38kHz frequency

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);

String str1, str2;
int ammo, maxAmmo = 20;
int health = 100;
int w1, w2;

void gun_trigger_interrupt();
Sidearm gun;
Pushbutton trigger(buttonPin, 0, false, gun_trigger_interrupt);
Visualizer visualizer(10, 26, 50);

void gun_trigger_interrupt()
{
    trigger.handleInterrupt();
}

void gun_setup()
{
    Serial.begin(115200);
    trigger.init();
    irSender.init();
    visualizer.init(255);
    pinMode(2, OUTPUT);
    visualizer.addAnimation(rainbowAnimation);
/*
    // Initialize the trigger button
    trigger.init(handleButtonEvent, true); // Enable all events
    trigger.enablePressEvent(true);
    trigger.enableReleaseEvent(true);
*/
    u8g2.begin();
    ammo = maxAmmo;

    trigger.enablePressEvent(true);
    gun.start();

    // Additional setup logic if needed
    Serial.println("Laser Tag Gun Ready!");
}

void gun_loop()
{
    //irSender.sendNEC(fireSignal); // Send fire signal when the trigger is pressed
    //irSender.mark(1E6);

    ///irSender.space(3e6);
    //digitalWrite(irPin, HIGH);

    gun.loop();
    visualizer.loop();

    if (gun.ammo == 0)
    {
        gun.reload();
    }

    if (trigger.hasPressed())
    {
        digitalWrite(2, HIGH);
        if (gun.shoot())
        {
            digitalWrite(2, LOW);
            // irSender.sendNEC(fireSignal); // Send fire signal when the trigger is pressed
            shotNow = false; // Reset the shot flag

            ammo = gun.getAmmo();
            maxAmmo = gun.getMagazine();

            health = random(100);

            str1 = String(ammo);
            str2 = String(maxAmmo);

            u8g2.clearBuffer();

            // hp bar
            u8g2.setFont(u8g2_font_unifont_t_0_78_79);
            u8g2.drawGlyph(112, 9, 0x2764);
            u8g2.drawHLine(16, 8, 96);
            u8g2.drawBox(16, 0, map(health, 0, 100, 0, 96), 8);

            // ammo
            u8g2.setFont(u8g2_font_fub25_tf);
            u8g2.setFontPosCenter();
            int h = 20;
            u8g2.drawVLine(u8g2.getDisplayWidth() / 2, (u8g2.getDisplayHeight() - h) / 2, h);
            u8g2.setCursor(u8g2.getDisplayWidth() / 2 - u8g2.getStrWidth(str1.c_str()) - 6, u8g2.getDisplayHeight() / 2);
            u8g2.println(str1.c_str());
            u8g2.setFont(u8g2_font_fub11_tf);
            u8g2.setCursor(u8g2.getDisplayWidth() / 2 + 6, u8g2.getDisplayHeight() / 2);
            u8g2.println(str2.c_str());

            u8g2.sendBuffer();
        }
    }
    

    // The rest of the program primarily relies on interrupts, so no logic is needed here
    // delay(10); // Optional: Small delay to stabilize processing
}

#endif // GUN_MAIN_HPP