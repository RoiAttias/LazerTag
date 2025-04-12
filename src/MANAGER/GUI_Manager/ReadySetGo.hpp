#ifndef READYSETGO_HPP
#define READYSETGO_HPP

#include "GUI_Manager.hpp"
#include "Utilities/Countdowner.hpp"

void countdownHandler(int parameter);
void readySetGoHandler(ivec2 point, TouchStatus status);

Message *readySetGoMessage = new Message(
    "Are you ready?",
    "",
    "YES",
    readySetGoHandler,
    TFT_NAVY,
    TFT_YELLOW,
    TFT_BLACK
);

String readySetGoText[] = {
    "3",
    "2",
    "1",
    "GO!"
};

void countdownHandler(int parameter) {
    if (parameter < 4) {
        readySetGoMessage->setMessage(readySetGoText[parameter]);
        readySetGoMessage->setButtonVisible(false);
        GUI::callRender();
        countdowner->addEvent(1000, countdownHandler, parameter + 1);
    } else {
        GUI::selectActivity(GUI_Manager_Activity::GAMEPLAY);
    }
}

void readySetGoHandler(ivec2 point, TouchStatus status) {
    if (status == TouchStatus_RELEASE) {
        countdowner->addEvent(100, countdownHandler, 0);
        readySetGoMessage->okButton.OnTouch_setEnable(false);
        status = TouchStatus_READY;
    }
    else if (status == TouchStatus_PRESS) {
        readySetGoMessage->okButton.background.fillColor = TFT_MAROON;
        readySetGoMessage->okButton.background.borderColor = TFT_YELLOW;
        readySetGoMessage->okButton.text.textColor = TFT_YELLOW;
        GUI::callRender();
    }
    
    if (status == TouchStatus_READY) {
        readySetGoMessage->okButton.background.fillColor = TFT_YELLOW;
        readySetGoMessage->okButton.background.borderColor = TFT_BLACK;
        readySetGoMessage->okButton.text.textColor = TFT_BLACK;
        GUI::callRender();
    }
}

#endif // READYSETGO_HPP