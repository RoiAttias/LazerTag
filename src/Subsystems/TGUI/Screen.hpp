#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "TGUI.hpp"

/*
*onpush
rendertime pushtime
framerate enable cap
time functionality
animation
*/

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
    void push();
    void renderNpush();

    void enableTouch(bool enable);
    void executeTouch(); // Add return executed

    void enablePush(bool enable);
    void setPushHandler(void (*handler)(void));

protected:
    void executePush();

    int currentActivity;

    bool touchEnabled;

    bool pushEnabled;
    void (*pushHandler)(void);
};

// Constructors
Screen::Screen(currentActivity = -1, pushEnabled = false, pushHandler = nullptr) :
    currentActivity(currentActivity), pushEnabled(pushEnabled), pushHandler(pushHandler) {}

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
    if (activities.size() > 0 && currentActivity > -1 && currentActivity < activities.size()) {
        activities.get(currentActivity)->render();
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


// Enable or disable features 0, 1
void Screen::enable(bool touch, bool push) {
    enableTouch(touch);
    enablePush(push);
}

// Enable or disable touch functionality
void Screen::enableTouch(bool enable) {
    touchEnabled = enable;
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
void Screen::executeTouch(ivec2 point,TouchStatus touchStatus) {
    if (activities.size() > 0 && currentActivity > -1 && currentActivity < activities.size()) {
        activities.get(currentActivity)->OnTouch_execute(point,touchStatus);
    }
}

#endif // SCREEN_HPP