#ifndef GUI_MANAGER_HPP
#define GUI_MANAGER_HPP

#include <Arduino.h>
#include "Manager_TFT_eSPI_user_setup.h"
#include <TFT_eSPI.h>

#define LUMINAUI_USE_TFT_ESPI
#include "Components/LuminaUI/LuminaUI.hpp"

TFT_eSPI tft = TFT_eSPI();   // Invoke library

ivec2 screenDiamentions;

Screen screen(true);
Touch_XPT2046 touch(&screen);

namespace GUI {
    void init();
    void selectActivity(int activity);
    void callRender();
    void loop();
};

enum GUI_Manager_Activity : uint8_t {
    ACTIVATION,
    DASHBOARD,
    HAVE_A_GOOD_WEEK,
    STORY,
    SCANNER,
    GUI_Manager_Activity_size
};

// Paths to Elements
#include "Elements/DeviceBox.hpp"

// Paths to Activities
#include "Activation.hpp"
#include "Dashboard.hpp"
#include "HaveAGoodWeek.hpp"
#include "Story.hpp"

#include "Scanner.hpp"

Activity * GUI_Manager_Activities[] = {
    activation,
    new Dashboard(),
    new HaveAGoodWeek(),
    new Story(),
    new Scanner()
};

namespace GUI {
    void init(){
        // TFT Initialization
        tft.begin();  
        tft.setRotation(1);
        tft.fillScreen(TFT_BLACK);
        tft.getTouchRawZ();

        // LuminaUI Initialization
        LuminaUI::tft_instance = &tft;
        screenDiamentions = ivec2(LuminaUI::tft_instance->width(), LuminaUI::tft_instance->height());
        screen.init(screenDiamentions, GUI_Manager_Activities, GUI_Manager_Activity_size, true);
        touch.init(ENABLE_ALL);

        selectActivity(GUI_Manager_Activity::SCANNER);
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