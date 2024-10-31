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
    void push();
    void renderNpush();

    void updateTouch(int x, int y);
    void updateTouch(Point point);

    void enablePush(bool enable);
    void setPushHandler(void (*handler)(void));

protected:
    void executePush();

    int currentActivity;
    Touch touch;
    bool pushEnabled;
    void (*pushHandler)(void);
};

// Constructors
Screen::Screen(currentActivity = -1, pushEnabled = false, pushHandler = nullptr) {}

Screen::Screen(Activity* activity, currentActivity = 0, pushEnabled = false, pushHandler = nullptr) {
    addActivity(activity);
}

Screen::Screen(Activity* activities, int amount, currentActivity = 0, pushEnabled = false, pushHandler = nullptr) {
    addActivities(activities, amount);
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

// Render all visible activities
void Screen::renderAll() {
    for (int i = 0; i < activities.size(); i++) {
        if (activities.get(i)->isVisible()) {
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
void Screen::updateTouch(ivec2 point,TouchStatus touchStatus) {
    if (activities.size() > 0) {
        activities.get(currentActivity)->OnTouch_execute(point,touchStatus);
    }
}

#endif // SCREEN_HPP