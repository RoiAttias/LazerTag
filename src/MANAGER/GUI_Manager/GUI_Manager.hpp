#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>

#define LUMINAUI_USE_TFT_ESPI
#include "Components/LuminaUI/LuminaUI.hpp"

#include "Components/Nexus/Nexus.hpp"

TFT_eSPI tft = TFT_eSPI();   // Invoke library

ivec2 screenDiamentions1;

Screen screen(true);
Touch_XPT2046 touch(&screen);

namespace GUI {
    // Shared variables
    HyperList<NexusAddress> gameDevices;
    
    void init();
    void selectActivity(int activity);
    void callRender();
    void loop();
};

enum GUI_Manager_Activity : uint8_t {
    ACTIVATION,
    SCANNER,
    PLAYERSETUP,
    GAMEPLAY,
    DASHBOARD,
    GUI_Manager_Activity_size
};

// Paths to Elements
#include "Elements/DeviceBox.hpp"
#include "Elements/HpBar.hpp"

// Paths to Activities
#include "Activation.hpp"
#include "Scanner.hpp"
#include "PlayerSetup.hpp"
#include "Gameplay.hpp"
#include "Dashboard.hpp"

Activity * GUI_Manager_Activities[] = {
    activation,
    scanner,
    playerSetup,
    gameplay,
    new Dashboard(),
};

namespace GUI {
    // Declears
    void init(int);
    void selectActivity(int);
    void callRender();
    void loop();

    void init(int startActivity = GUI_Manager_Activity::ACTIVATION){
        // TFT Initialization
        tft.begin();  
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);
        tft.getTouchRawZ();

        // LuminaUI Initialization
        LuminaUI::tft_instance = &tft;
        screenDiamentions1 = ivec2(LuminaUI::tft_instance->width(), LuminaUI::tft_instance->height());
        screen.init(screenDiamentions1, GUI_Manager_Activities, GUI_Manager_Activity_size, true);
        touch.init(ENABLE_PRESS | ENABLE_RELEASE);

        // Select the first activity
        selectActivity(startActivity);
    }

    void selectActivity(int activity)
    {
        screen.selectActivity(activity);
    }

    void callRender()
    {
        screen.callRender();
    }

    void loop()
    {
        touch.loop();
        if (screen.shouldRender()) {
            screen.render();
        }
    }
};

#endif // GUI_MANAGER_HPP