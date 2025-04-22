/**
 * @file Screen.hpp
 * @brief Manages application screens composed of multiple Activities, handling render and input.
 *
 * The Screen class maintains a list of Activity instances, handles switching between them,
 * and propagates touch and push (button) events. It supports render-on-demand and optional
 * automatic re-render after touch events.
 */

 #ifndef SCREEN_HPP
 #define SCREEN_HPP
 
 #include "LuminaUI.hpp"
 #include "Activity.hpp"
 
 /**
  * @class Screen
  * @brief Top-level UI controller managing Activities and input.
  *
  * Screen holds multiple Activities, each representing a full UI page or context. It
  * handles rendering the active Activity, touch events, and custom push events.
  */
 class Screen {
 public:
     HyperList<Activity*> activities; ///< List of Activities managed by the Screen
     ivec2 resolution;               ///< Screen resolution (width, height)
     bool renderAfterOnTouch;        ///< If true, re-render after non-drag touch events
 
     volatile bool _shouldRender;    ///< Flag indicating render is needed
     volatile int currentActivity;   ///< Index of the currently active Activity
     bool touchEnabled;              ///< Whether touch input is enabled
     bool pushEnabled;               ///< Whether push input is enabled
 
     void (*pushHandler)(void);      ///< Custom push (button) event handler
 
     /**
      * @brief Construct a Screen.
      * @param renderAfterOnTouch If true, call render() after touch events (except hold/drag).
      */
     Screen(bool renderAfterOnTouch = false)
       : resolution(LuminaUI_AUTO)
       , renderAfterOnTouch(renderAfterOnTouch)
       , _shouldRender(false)
       , currentActivity(-1)
       , touchEnabled(false)
       , pushEnabled(false)
       , pushHandler(nullptr)
     {}
 
     /**
      * @brief Destructor clears all Activity pointers.
      */
     ~Screen() {
         activities.clear();
     }
 
     /**
      * @brief Initialize the Screen with Activities and input settings.
      * @param resolution Screen resolution vector.
      * @param activity Array of Activity pointers.
      * @param amount Number of Activities in the array.
      * @param enTouch Enable touch input.
      * @param enPush Enable push input.
      */
     void init(ivec2 resolution, Activity* activity[], int amount,
               bool enTouch = false, bool enPush = false) {
         activities.clear();
         this->resolution = resolution;
         addActivities(activity, amount);
         enableTouch(enTouch);
         enablePush(enPush);
     }
 
     /**
      * @brief Select the active Activity by index.
      * @param index Index of the Activity to activate.
      */
     void selectActivity(int index) {
         if (index >= 0 && index < activities.size()) {
             currentActivity = index;
             activities[currentActivity]->callRender();
             callRender();
         }
     }
 
     /**
      * @brief Add a single Activity to the Screen.
      * @param activity Pointer to the Activity to add.
      */
     void addActivity(Activity* activity) {
         if (!activity) return;
         if (activity->origin == LuminaUI_AUTO) {
             activity->origin = ivec2(0,0);
         }
         if (activity->scale == LuminaUI_AUTO) {
             activity->scale = resolution;
         }
         activities.addend(activity);
         if (currentActivity == -1) currentActivity = 0;
     }
 
     /**
      * @brief Add multiple Activities from an array.
      * @param activity Array of Activity pointers.
      * @param amount Number of Activities to add.
      */
     void addActivities(Activity** activity, int amount) {
         if (!activity || amount <= 0) return;
         for (int i = 0; i < amount; ++i) {
             addActivity(activity[i]);
         }
     }
 
     /**
      * @brief Mark the screen for rendering in the next loop.
      */
     void callRender() {
         _shouldRender = true;
     }
 
     /**
      * @brief Query if rendering is pending.
      * @return True if render() should be called.
      */
     bool shouldRender() const {
         return _shouldRender;
     }
 
     /**
      * @brief Get the full screen viewport.
      * @return Viewport covering entire screen.
      */
     Viewport getViewport() const {
         return Viewport(ivec2(0,0), resolution);
     }
 
     /**
      * @brief Render the active Activity, if any.
      */
     void render() {
         if (currentActivity >= 0 && currentActivity < activities.size()) {
             Activity* activity = activities.get(currentActivity);
             if (activity && activity->visible && activity->shouldRender() &&
                 getViewport().inRange(activity->getViewport())) {
                 activity->render(getViewport());
             }
         }
         _shouldRender = false;
     }
 
     /**
      * @brief Execute custom push handler if enabled.
      */
     virtual void push() {
         executePush();
     }
 
     /**
      * @brief Render and then execute push handler.
      */
     void renderNpush() {
         render();
         push();
     }
 
     /**
      * @brief Enable or disable touch input.
      * @param enable True to enable touch processing.
      */
     void enableTouch(bool enable) {
         touchEnabled = enable;
     }
 
     /**
      * @brief Forward a touch event to the active Activity.
      * @param point Coordinates of the touch.
      * @param touchStatus Type of touch event.
      */
     void executeTouch(ivec2 point, TouchStatus touchStatus) {
         if (touchEnabled && currentActivity >= 0 && currentActivity < activities.size()) {
             Activity* activity = activities.get(currentActivity);
             if (activity) activity->OnTouch_execute(point, touchStatus);
         }
         if (renderAfterOnTouch && touchStatus != TouchStatus_HOLD && touchStatus != TouchStatus_DRAG) {
             callRender();
         }
     }
 
     /**
      * @brief Enable or disable push input.
      * @param enable True to enable push processing.
      */
     void enablePush(bool enable) {
         pushEnabled = enable;
     }
 
     /**
      * @brief Set the custom push handler function.
      * @param handler Function pointer to call on push.
      */
     void setPushHandler(void (*handler)(void)) {
         pushHandler = handler;
     }
 
     /**
      * @brief Execute the push handler if enabled.
      */
     void executePush() {
         if (pushEnabled && pushHandler) pushHandler();
     }
 
     /**
      * @brief Get the number of Activities on this Screen.
      * @return Count of managed Activities.
      */
     int getActivityCount() const {
         return activities.size();
     }
 
     /**
      * @brief Get the index of the currently active Activity.
      * @return Active Activity index, or -1 if none.
      */
     int getCurrentActivityIndex() const {
         return currentActivity;
     }
 };
 
 #endif // SCREEN_HPP 