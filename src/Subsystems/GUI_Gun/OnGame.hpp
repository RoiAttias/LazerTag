#ifndef ON_GAME_HPP
#define ON_GAME_HPP

#include "GUI_Gun.hpp"
#include "Modules/Player.hpp"
#include <Arduino.h>

class OnGameActivity : public Activity {
public:
    // Variables
    Player *player; // Pointer to the player object
    int ammo; // Current ammo count
    int maxAmmo; // Maximum ammo capacity
    int health; // Player's health
    String str1; // String representation of current ammo
    String str2; // String representation of max ammo
    bool isReloading; // Flag to indicate if reloading is in progress

    // Constructor
    OnGameActivity() : Activity() {}

    Viewport render(const Viewport &viewport) override {
        update();

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

        // reloading
        if (isReloading) {
            u8g2.setFont(u8g2_font_fub11_tf);
            u8g2.setFontPosCenter();
            u8g2.setCursor(u8g2.getDisplayWidth() / 2, u8g2.getDisplayHeight() / 2 + 20);
            u8g2.println("Reloading...");
        }

        return Activity::render(viewport);
    }

    void update() {
        auto &gun = player->gun;
        ammo = gun.getAmmo();
        maxAmmo = gun.getMagazine();
        health = player->getHP();
        str1 = String(ammo);
        str2 = String(maxAmmo);
        isReloading = gun.getStatus() == RELOADING;
    }

    void setPlayer(Player *playerPtr) {
        player = playerPtr;
    }
};

OnGameActivity *onGame = new OnGameActivity();

#endif // ON_GAME_HPP
