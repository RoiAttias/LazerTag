#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "TGUI.hpp"

class Screen {
public:
    HyperList<Activity*> activities;

    Screen(Activity* activity);
    Screen(Activity* activity, int amount);
    ~Screen();

    void addActivity(Activity* activity);
    void addActivities(Activity* activity, int amount);
    void switchToActivity(int index);

    void render();
    void renderAll();
    void renderAllVisible();
    void push();
    void renderNpush();

    void updateTouch(int x, int y);
    void updateTouch(Point point);

    void enablePush(bool enable);
    void setPushHandler(void (*handler)(void));

private:
    void executePush();

    int currentActivity;
    Touch touch;
    bool pushEnabled;
    void (*pushHandler)(void);
};

// Constructor with a single activity
Screen::Screen(Activity* activity) : currentActivity(0), pushEnabled(false), pushHandler(nullptr) {
    addActivity(activity);
}

// Constructor with multiple activities
Screen::Screen(Activity* activity, int amount) : currentActivity(0), pushEnabled(false), pushHandler(nullptr) {
    addActivities(activity, amount);
}


// Add a single activity
void Screen::addActivity(Activity* activity) {
    activities.add(activity);
}

// Add multiple activities
void Screen::addActivities(Activity* activity, int amount) {
    for (int i = 0; i < amount; i) {
        activities.add(activity + i);
    }
}

// Switch to a specific activity by index
void Screen::switchToActivity(int index) {
    if (index >= 0 && index < activities.size()) {
        currentActivity = index;
    }
}

// Render the current activity
void Screen::render() {
    if (activities.size() > 0) {
        activities.get(currentActivity)->render();
    }
}

// Render all activities
void Screen::renderAll() {
    for (int i = 0; i < activities.size(); i++) {
        activities.get(i)->render();
    }
}

// Render all visible activities
void Screen::renderAllVisible() {
    for (int i = 0; i < activities.size(); i++) {
        if (activities.get(i)->visible) {
            activities.get(i)->render();
        }
    }
}

// Execute the push handler if enabled
void Screen::push() {
    executePush();
}

// Render the current activity and execute the push handler
void Screen::renderNpush() {
    render();
    push();
}

// Enable or disable push functionality
void Screen::enablePush(bool enable) {
    pushEnabled = enable;
}

// Set the push handler function
void Screen::setPushHandler(void (*handler)(void)) {
    pushHandler = handler;
}

// Execute the push handler if enabled and set
void Screen::executePush() {
    if (pushEnabled && pushHandler != nullptr) {
        pushHandler();
    }
}

// Update touch input with coordinates
void Screen::updateTouch(int x, int y) {
    touch.setPoint(x, y);
    if (activities.size() > 0) {
        activities.get(currentActivity)->OnClick_execute(touch.getPoint());
    }
}

// Update touch input with a Point
void Screen::updateTouch(Point point) {
    updateTouch(point.x, point.y);
}

#endif // SCREEN_HPP