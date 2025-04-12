#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <Arduino.h>
#include <TFT_eSPI.h>

#define LUMINAUI_USE_TFT_ESPI
#include "Components/LuminaUI/LuminaUI.hpp"

#include "Components/Nexus/Nexus.hpp"

namespace GUI {
    TFT_eSPI tft = TFT_eSPI();

    ivec2 screenDiamentions;

    Screen screen(true);
    Touch_XPT2046 touch(&screen);
    
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
    READYSETGO,
    GAMEPLAY,
    DASHBOARD,
    GUI_Manager_Activity_size
};

// Paths to Elements
#include "Elements/DeviceBox.hpp"
#include "Elements/HpBar.hpp"
#include "Elements/Gradient.hpp"

// Paths to Activities
#include "Activation.hpp"
#include "Scanner.hpp"
#include "PlayerSetup.hpp"
#include "Message.hpp"
#include "ReadySetGo.hpp"
#include "Gameplay.hpp"
#include "Dashboard.hpp"

Activity * GUI_Manager_Activities[] = {
    activation,
    scanner,
    playerSetup,
    readySetGoMessage,
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
        screenDiamentions = ivec2(LuminaUI::tft_instance->width(), LuminaUI::tft_instance->height());
        screen.init(screenDiamentions, GUI_Manager_Activities, GUI_Manager_Activity_size, true);
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