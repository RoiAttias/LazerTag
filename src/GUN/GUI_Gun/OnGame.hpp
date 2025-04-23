/**
 * @file OnGame.hpp
 * @brief Primary gameplay Activity showing health bar, ammo count, and reload status.
 *
 * Inherits from LuminaUI::Activity. Reads Player and Gun state pointers each render.
 */

 #ifndef ON_GAME_HPP
 #define ON_GAME_HPP
 
 #include "GUI_Gun.hpp"         ///< Screen & Activity definitions
 #include "Modules/Player.hpp"  ///< Player model for HP
 #include <Arduino.h>           ///< String, map(), etc.
 
 /**
  * @class OnGameActivity
  * @brief Primary gameplay display: shows health bar, ammo count, and reload status.
  *
  * Inherits from Activity. Reads Player and Gun state pointers each render.
  */
 class OnGameActivity : public Activity {
 public:
     // Pointers to shared game models
     Player* player;    ///< Player data (HP, etc.)
     Gun* gun;          ///< Gun logic (ammo, reload status)
 
     // Cached display values
     int ammo;          ///< Current ammo
     int maxAmmo;       ///< Magazine capacity
     int health;        ///< Current player HP
     String str1;       ///< String for ammo count
     String str2;       ///< String for max ammo
     bool isReloading;  ///< Whether a reload is in progress
 
     /**
      * @brief Default constructor.
      *        Activity() base initializes origin/scale.
      */
     OnGameActivity() : Activity() {}
 
     /**
      * @brief Render the HUD elements.
      * @param viewport LuminaUI viewport
      * @return Clamped viewport used.
      */
     Viewport render(const Viewport &viewport) override {
         // Refresh local values from Player and Gun
         update();
 
         // ----- Draw health bar -----
         // Heart icon (Unicode U+2764) at top-right
         u8g2.setFont(u8g2_font_unifont_t_0_78_79);
         u8g2.drawGlyph(112, 9, 0x2764);
 
         // Outline a horizontal bar
         u8g2.drawHLine(16, 8, 96);
         // Filled portion proportional to HP
         u8g2.drawBox(
             16,      ///< x
             0,       ///< y
             map(health, 0, 100, 0, 96), ///< width
             8        ///< height
         );
 
         // ----- Draw ammo count -----
         // Large digits separated by a vertical divider
         u8g2.setFont(u8g2_font_fub25_tf);
         u8g2.setFontPosCenter();
         const int h = 20;
         int midX = u8g2.getDisplayWidth() / 2;
         int midY = u8g2.getDisplayHeight() / 2;
         // Center line
         u8g2.drawVLine(midX, (u8g2.getDisplayHeight() - h) / 2, h);
 
         // Left number = current ammo
         u8g2.setCursor(
             midX - u8g2.getStrWidth(str1.c_str()) - 6,
             midY
         );
         u8g2.println(str1.c_str());
 
         // Smaller font for maximum ammo
         u8g2.setFont(u8g2_font_fub11_tf);
         u8g2.setCursor(midX + 6, midY);
         u8g2.println(str2.c_str());
 
         // ----- Draw reload message -----
         if (isReloading) {
             u8g2.setFont(u8g2_font_fub11_tf);
             u8g2.setFontPosCenter();
             u8g2.setCursor(midX, midY + 20);
             u8g2.println("Reloading...");
         }
 
         // Clear render flag and clamp viewport
         return Activity::render(viewport);
     }
 
     /**
      * @brief Pull latest values from Player/Gun into locals.
      */
     void update() {
         ammo        = gun->getAmmo();
         maxAmmo     = gun->getMagazine();
         health      = player->getHP();
         str1        = String(ammo);
         str2        = String(maxAmmo);
         isReloading = (gun->getStatus() == RELOADING);
     }
 
     /**
      * @brief Provide pointers to the shared Player and Gun objects.
      * @param playerPtr Pointer to Player model
      * @param gunPtr    Pointer to Gun logic
      */
     void setPointers(Player* playerPtr, Gun* gunPtr) {
         player = playerPtr;
         gun    = gunPtr;
     }
 };
 
 // Instantiate the OnGameActivity on the heap
OnGameActivity* onGame = new OnGameActivity();
 
 #endif // ON_GAME_HPP 