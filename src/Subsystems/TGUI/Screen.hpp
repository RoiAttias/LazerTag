#ifndef SCREEN_HPP
#define SCREEN_HPP

#include "TGUI.hpp"
#if defined(LUMINAUI_USE_TFT_ESPI)
#include "TFT_eSPI_Elements/TFT_eSPI_Elements.hpp"
#endif

/*
framerate mode
rendertime pushtime variables
framerate enable cap
*/

/**
 * @brief Screen class manages multiple Activities and handles their rendering and input interactions.
 *
 * Key Features:
 * - **Activity Management**: Allows adding, removing, and switching between Activities.
 * - **Rendering**: Renders the current active Activity.
 * - **Touch and Push Events**: Handles touch interactions and custom push events.
 */
class Screen {
public:
    HyperList<Activity*> activities; ///< List of Activities managed by the Screen.
    ivec2 resolution;
    bool renderAfterOnTouch;

    // Constructors and Destructor
    /**
     * @brief Constructor to initialize a Screen with multiple Activities.
     *
     * @param activity Pointer to the first Activity in the array.
     * @param amount Number of Activities to add.
     */
    Screen(Activity** activity, int amount, bool renderAfterOnTouch = false)
        : _shouldRender(false), resolution(TGUI_AUTO), renderAfterOnTouch(renderAfterOnTouch),
        currentActivity(0), touchEnabled(false), pushEnabled(false), pushHandler(nullptr) { 
        if (activity && amount > 0) {
            addActivities(activity, amount);
        }
    }

    /**
     * @brief Constructor to initialize a Screen with a single Activity.
     *
     * @param activity Pointer to the initial Activity.
     */
    Screen(Activity** activity, bool renderAfterOnTouch = false)
        : Screen(activity, 1){}

    /**
     * @brief Destructor to clean up resources.
     */
    ~Screen() {
        activities.clear();
    }

    void init(ivec2 resolution)
    {
        this->resolution = resolution;
    }

    // Activity Management
    /**
     * @brief Add a single Activity to the Screen.
     *
     * @param activity Pointer to the Activity to add.
     */
    void addActivity(Activity * activity) {
        if (activity) {
            activities.addend(activity);
            if(activity->origin == TGUI_AUTO)
            {
                activity->origin = 0;
            }
            if (activity->scale == TGUI_AUTO)
            {
                activity->scale == resolution;
            }
        }
    }

    /**
     * @brief Add multiple Activities to the Screen.
     *
     * @param activity Pointer to the first Activity in the array.
     * @param amount Number of Activities to add.
     */
    void addActivities(Activity * activity[], int amount) {
        for (int i = 0; i < amount; i++) {
            addActivity(activity[i]);
        }
    }

    /**
     * @brief Switch to a specific Activity by its index.
     *
     * @param index Index of the Activity to switch to.
     */
    void selectActivity(int index) {
        if (index >= 0 && index < activities.size()) {
            currentActivity = index;
        }
    }

    // Rendering and Push Handling

    void callRender() {
        _shouldRender = true;
    }

    bool shouldRender() {
        return _shouldRender;
    }

    /**
     * @brief Render the current active Activity.
     */
    void render() {
        if (currentActivity >= 0 && currentActivity < activities.size()) {
            activities.get(currentActivity)->render();
        }
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
            activities.get(currentActivity)->OnTouch_execute(point, touchStatus);
        }

        if (renderAfterOnTouch)
        {
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

protected:
    /**
     * @brief Execute the push handler function if enabled.
     */
    void executePush() {
        if (pushEnabled && pushHandler) {
            pushHandler();
        }
    }
    bool _shouldRender; ///< Flag for rendering inside a loop
    int currentActivity; ///< Index of the currently active Activity.
    bool touchEnabled;   ///< Indicates whether touch is enabled.
    bool pushEnabled;    ///< Indicates whether push is enabled.
    
    #if defined(LUMINAUI_USE_TFT_ESPI)
        void (*pushHandler)(void) = push_TFT_eSPI; ///< Function pointer to the sprite push handler.
    #else
        void (*pushHandler)(void); ///< Function pointer to the custom push handler.
    #endif
};

#endif // SCREEN_HPP
