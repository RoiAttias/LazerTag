/**
 * @file Visualizer.hpp
 * @brief Manages NeoPixel strip animations with layering and timing control.
 *
 * Uses Adafruit_NeoPixel for LED control and a HyperList of Animation structs
 * for scheduling and running multiple concurrent animations with frame rate capping.
 */

 #ifndef VISUALIZER_HPP
 #define VISUALIZER_HPP
 
 #include <Adafruit_NeoPixel.h>
 #include "Utilities/HyperList.hpp"
 #include "Utilities/MoreMath.hpp"
 
 /**
  * @brief Represents a single animation on a NeoPixel strip.
  *
  * Stores the animation function pointer, layer ordering, LED range,
  * timing parameters (duration, repeat), and pause state.
  */
 struct Animation {
     /**
      * @brief Function performing animation for a segment of LEDs.
      *
      * @param strip Pointer to NeoPixel strip instance.
      * @param startIndex Starting pixel index for this animation.
      * @param length Number of pixels covered.
      * @param factor Normalized progress [0.0,1.0] through cycle.
      */
     void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float);
 
     byte layer;             ///< Layer order (0 = bottom, higher = drawn later)
     uint16_t startIndex;    ///< First LED index for this animation
     uint16_t length;        ///< Number of LEDs in the animation segment
 
     unsigned long durationMS;   ///< Duration of one cycle in milliseconds
     bool repeat;                ///< True to restart after duration elapses
 
     unsigned long startTimeMS;  ///< Timestamp when animation started
     bool paused;                ///< True if animation is currently paused
     float currentFactor;        ///< Last computed progress factor
     unsigned long pauseTimeMS;  ///< Timestamp when paused
 
     /**
      * @brief Constructs an Animation with optional parameters.
      *
      * @param animationFunc Animation callback function.
      * @param layer Animation drawing layer.
      * @param startIndex First LED index.
      * @param length Count of LEDs.
      * @param durationMS Cycle duration (ms).
      * @param repeat True to loop continuously.
      */
     Animation(
         void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float) = nullptr,
         byte layer = 0,
         uint16_t startIndex = 0,
         uint16_t length = 0,
         unsigned long durationMS = 0,
         bool repeat = false)
       : animationFunc(animationFunc), layer(layer), startIndex(startIndex), length(length),
         durationMS(durationMS), repeat(repeat), startTimeMS(-1), paused(false), currentFactor(0.0f), pauseTimeMS(0) {}
 
     /**
      * @brief Updates currentFactor based on currentTime and returns it.
      *
      * @param currentTime Current millis() timestamp.
      * @return Normalized progress factor [0.0,1.0].
      */
     float run(unsigned long currentTime) {
         if (durationMS > 0) {
             currentFactor = float((currentTime - startTimeMS) % durationMS) / durationMS;
         } else {
             currentFactor = 0.0f;
         }
         return currentFactor;
     }
 
     /**
      * @brief Determines if the animation should execute at currentTime.
      *
      * @param currentTime Current millis() timestamp.
      * @return True if started and within duration or if repeating.
      */
     bool shouldRun(unsigned long currentTime) const {
         return (startTimeMS != unsigned(-1)) && ((currentTime - startTimeMS < durationMS) || repeat);
     }
 
     /**
      * @brief Begins the animation cycle.
      *
      * @param currentTime Current millis() timestamp.
      */
     void start(unsigned long currentTime) {
         startTimeMS = currentTime;
         paused = false;
         currentFactor = 0.0f;
     }
 
     /**
      * @brief Pauses the animation, preserving progress.
      *
      * @param currentTime Current millis() timestamp.
      */
     void pause(unsigned long currentTime) {
         if (!paused) {
             pauseTimeMS = currentTime;
             paused = true;
         }
     }
 
     /**
      * @brief Resumes a paused animation, adjusting startTime.
      *
      * @param currentTime Current millis() timestamp.
      */
     void resume(unsigned long currentTime) {
         if (paused) {
             startTimeMS += (currentTime - pauseTimeMS);
             paused = false;
         }
     }
 
     /**
      * @brief Stops the animation and resets its state.
      */
     void stop() {
         startTimeMS = unsigned(-1);
         paused = false;
         currentFactor = 0.0f;
     }
 };
 
 /**
  * @class Visualizer
  * @brief Manages and runs layered animations on a NeoPixel strip.
  */
 class Visualizer {
 private:
     const neoPixelType type;        ///< Pixel color order and frequency
     const uint8_t gOffset;          ///< Green color offset
     const uint8_t rOffset;          ///< Red color offset
     const uint8_t bOffset;          ///< Blue color offset
 
     Adafruit_NeoPixel strip;        ///< NeoPixel strip driver instance
     unsigned long lastUpdateTime;   ///< Timestamp of last frame update
     unsigned long frameIntervalMS;  ///< Minimum interval between frames
 
 public:
     HyperList<Animation> animations;///< List of active animations
     static const byte MAX_LAYERS = 8;///< Maximum number of layers
 
     /**
      * @brief Constructs the Visualizer for a given strip.
      *
      * @param pin           Data pin connected to strip.
      * @param numPixels     Total pixel count.
      * @param frameIntervalMS Minimum time between updates.
      */
     Visualizer(uint8_t pin, uint16_t numPixels, unsigned long frameIntervalMS)
       : type(NEO_GRB + NEO_KHZ800), gOffset(0), rOffset(1), bOffset(2),
         strip(numPixels, pin, type), lastUpdateTime(0), frameIntervalMS(frameIntervalMS) {}
 
     /**
      * @brief Initializes the strip hardware and clears pixels.
      *
      * @param brightness Initial brightness (0-255).
      */
     void init(uint8_t brightness = 50) {
         strip.begin();
         strip.setBrightness(brightness);
         strip.show();
     }
 
     /**
      * @brief Adjust pixel brightness.
      * @param brightness Brightness level (0-255).
      */
     void setBrightness(uint8_t brightness) {
         strip.setBrightness(brightness);
     }
 
     /**
      * @brief Adds and starts a new Animation object.
      * @param anim Animation to schedule.
      */
     void addAnimation(const Animation& anim) {
         Animation a = anim;
         a.start(millis());
         animations.addend(a);
     }
 
     /**
      * @brief Convenience overload to create and add an animation.
      */
     void addAnimation(
         void (*animationFunc)(Adafruit_NeoPixel*, uint16_t, uint16_t, float),
         uint8_t layer, uint16_t startIndex, uint16_t length,
         unsigned long durationMS, bool repeat = false) {
         Animation anim(animationFunc, layer, startIndex, length, durationMS, repeat);
         addAnimation(anim);
     }
 
     /**
      * @brief Removes the animation at given list index.
      * @param index Position in animations list.
      */
     void removeAnimation(uint16_t index) {
         animations.remove(index);
     }
 
     /**
      * @brief Clears all scheduled animations.
      */
     void clearAnimations() {
         animations.clear();
     }
 
     /**
      * @brief Main update loop; call periodically (e.g., in Arduino loop()).
      *
      * Clears the strip, runs each active animation per layer order,
      * and shows the resulting frame at limited frame rate.
      */
     void loop() {
         unsigned long currentTime = millis();
         if (currentTime - lastUpdateTime < frameIntervalMS) return;
         lastUpdateTime = currentTime;
 
         strip.clear();
         for (uint8_t layer = 0; layer < MAX_LAYERS; ++layer) {
             for (int i = 0; i < animations.size(); ++i) {
                 Animation anim = animations[i];
                 if (anim.layer == layer && anim.shouldRun(currentTime)) {
                     float factor = anim.run(currentTime);
                     anim.animationFunc(&strip, anim.startIndex, anim.length, factor);
                 } else if (!anim.shouldRun(currentTime)) {
                     animations.remove(i);
                     --i;
                 }
             }
         }
         strip.show();
     }
 };
 
 #endif // VISUALIZER_HPP 