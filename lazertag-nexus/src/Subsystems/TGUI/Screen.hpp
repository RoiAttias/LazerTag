#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "TGUI.hpp"

class Screen {
public:
    Screen::Screen(Activity* activity);
    Screen::Screen(Activity* activity, int amount);
    ~Screen();
    
    void addActivity(Activity* activity);
    void addActivity(Activity* activity, int amount);
    void switchToActivity(int index);

    void render();
    void push();
    void renderNpush();

    void updateTouch(int x, int y);
    void updateTouch(Point point);

private:
    HyperList<Activity*> activities;
    int currentActivity;
    Touch touch;
    bool Push_enable;
    void (*Push_handler)(void);
};

Screen::Screen(Activity* activity) {
    addActivity(activity);
    switchToActivity(0);
}

Screen::Screen(Activity* activity, int amount) {

    addActivity(activity, amount);
    switchToActivity(0);
}

Screen::~Screen() {
    activities.clear();
}

void Screen::addActivity(Activity* activity) {
    activities.add(activity);
}

void Screen::addActivity(Activity* activity, int amount) {
    activities.addFromArray(activity, amount);
}

void Screen::switchToActivity(int index) {
        if (index >= 0 && index < activities.size()) {
            currentActivity = index;
        }
}

void Screen::render() {
    activities.get(currentActivity)->render();
}

void Screen::renderAll() {
    for(int i = 0; i < activities.size(); i++) {
        activities.get(i)->render();
    }
}

void Screen::push() {
    Push_execute();
}

void Screen::renderNpush() {
    render();
    push();
}

void Screen::Push_enable(bool enable) {
    Push_enable = enable;
}

void Screen::Push_setHandler(void (*handler)(void)) {
    Push_handler = handler;
}

void Screen::Push_execute() {
    if (!Push_enable) return;
    if (Push_handler == nullptr) return;
    Push_handler();
}

void Screen::updateTouch(int x, int y) {
    touch.setPoint(x,y);
    activities.get(currentActivity)->OnClick_execute(touch.getPoint());
}

void Screen::updateTouch(Point point) {
    updateTouch(point.x, point.y);
}

#endif // SCREEN_HPP