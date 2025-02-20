#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "LuminaUI.hpp"
#include "Activity.hpp"

/**
 * @brief Screen class manages multiple Activities and handles their rendering and input interactions.
 *
 * Key Features:
 * - **Activity Management**: Allows adding, removing, and switching between Activities.
 * - **Rendering**: Renders the current active Activity.
 * - **Touch and Push Events**: Handles touch interactions and custom push events.
 */

// Add renderOnlyElementCalls
class Screen {
public:
    HyperList<Activity*> activities; ///< List of Activities managed by the Screen.
    ivec2 resolution;
    bool renderAfterOnTouch;
    
    volatile bool _shouldRender; ///< Flag for rendering inside a loop
    volatile int currentActivity; ///< Index of the currently active Activity.
    bool touchEnabled;   ///< Indicates whether touch is enabled.
    bool pushEnabled;    ///< Indicates whether push is enabled.
    
    void (*pushHandler)(void); ///< Function pointer to the custom push handler.
    // Constructors and Destructor
    /**
     * @brief Constructor to initialize a Screen with multiple Activities.
     *
     * @param activity Pointer to the first Activity in the array.
     * @param amount Number of Activities to add.
     */
    Screen(bool renderAfterOnTouch = false)
        : resolution(LuminaUI_AUTO), renderAfterOnTouch(renderAfterOnTouch), _shouldRender(false),
        currentActivity(-1), touchEnabled(false), pushEnabled(false), pushHandler(nullptr) {}    

    /**
     * @brief Destructor to clean up resources.
     */
    ~Screen() {
        activities.clear();
    }

    void init(ivec2 resolution, Activity * activity[], int amount, bool enTouch = false, bool enPush = false)
    {
        activities.clear();

        this->resolution = resolution;
        addActivities(activity, amount);
        enableTouch(enTouch);
        enablePush(enPush);
    }

    /**
     * @brief Switch to a specific Activity by its index.
     *
     * @param index Index of the Activity to switch to.
     */
    void selectActivity(int index) {
        if (index >= 0 && index < activities.size()) {
            currentActivity = index;
            activities[currentActivity]->callRender();
            callRender();
        }
    }

    // Activity Management
    /**
     * @brief Add a single Activity to the Screen.
     *
     * @param activity Pointer to the Activity to add.
     */
    void addActivity(Activity * activity) {
        if (activity) {
            if(activity->origin == LuminaUI_AUTO)
            {
                activity->origin = ivec2(0,0);
            }
            if (activity->scale == LuminaUI_AUTO)
            {
                activity->scale = resolution;
            }
            activities.addend(activity);
            if (currentActivity == -1) {
                currentActivity = 0;
            }
        }
    }

    /**
     * @brief Add multiple Activities to the Screen.
     *
     * @param activity Pointer to the first Activity in the array.
     * @param amount Number of Activities to add.
     */
    void addActivities(Activity ** activity, int amount) {
        if (activity && amount > 0) {
            for (int i = 0; i < amount; i++) {
                addActivity(activity[i]);
            }
        }
    }

    // Rendering and Push Handling

    void callRender() {
        _shouldRender = true;
    }

    bool shouldRender() {
        return _shouldRender;
    }

    Viewport getViewport() {
        return Viewport(ivec2(0,0), resolution);
    }

    /**
     * @brief Render the current active Activity.
     */
    void render() {
        if (currentActivity >= 0 && currentActivity < activities.size()) {
            Activity *activity = activities.get(currentActivity);
            if (activity != nullptr) {
                if (activity->visible && activity->shouldRender() && getViewport().inRange(activity->getViewport())) {
                    activity->render(getViewport());
                }
            }
        }
        _shouldRender = false;
    }

    /**
     * @brief Execute the push handler if enabled.
     */
    virtual void push() {
        executePush();
    }

    /**
     * @brief Render the current Activity and execute the push handler.
     */
    void renderNpush() {
        render();
        push();
    }

    // Touch and Push Functionality
    /**
     * @brief Enable or disable touch functionality.
     *
     * @param enable True to enable, false to disable.
     */
    void enableTouch(bool enable) {
        touchEnabled = enable;
    }

    /**
     * @brief Execute touch events on the current active Activity.
     *
     * @param point Coordinates of the touch input.
     * @param touchStatus The status of the touch event (e.g., press, drag, release).
     */
    void executeTouch(ivec2 point, TouchStatus touchStatus) {
        if (touchEnabled && currentActivity >= 0 && currentActivity < activities.size()) {
            Activity *activity = activities[currentActivity];
            if (activity != nullptr) {
                activity->OnTouch_execute(point, touchStatus);
            }
        }
        if (renderAfterOnTouch) {
            callRender();
        }
    }

    /**
     * @brief Enable or disable push functionality.
     *
     * @param enable True to enable, false to disable.
     */
    void enablePush(bool enable) {
        pushEnabled = enable;
    }

    /**
     * @brief Set the push handler function.
     *
     * @param handler Function pointer to the custom push handler.
     */
    void setPushHandler(void (*handler)(void)) {
        pushHandler = handler;
    }

    /**
     * @brief Execute the push handler function if enabled.
     */
    void executePush() {
        if (pushEnabled && pushHandler) {
            pushHandler();
        }
    }

    // Getters and Setters
    /**
     * @brief Get the number of Activities managed by the Screen.
     *
     * @return Number of Activities.
     */
    int getActivityCount() {
        return activities.size();
    }

    /**
     * @brief Get the index of the current active Activity.
     *
     * @return Index of the current active Activity.
     */
    int getCurrentActivityIndex() {
        return currentActivity;
    }
};

#endif // SCREEN_HPP
