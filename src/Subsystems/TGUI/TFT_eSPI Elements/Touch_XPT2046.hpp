#ifndef TOUCH_XPT2046_HPP
#define TOUCH_XPT2046_HPP

#include "TGUI.hpp"
#include "TFT_eSPI.h"

unsigned long debounceThreshold = 5;

class Touch_XPT2046 : public Touch {
protected:
    TFT_eSPI *tft;
    int isr_pin;
    
    unsigned long lastTime;

public:
    // Constructors
    Touch_XPT2046(Screen *screen, TFT_eSPI *tft, int isr_pin) {
        this->tft = tft;
        this->isr_pin = isr_pin;
        Touch::Touch(screen);
    }

    virtual void init(){
        // Configure touch interrupt pin and setup ISR
        pinMode(isr_pin, INPUT);
    }

    virtual bool outOfDebounceThreshold()
    {
      unsigned long newTime = millis();
      bool ans = newTime - lastTime > debounceThreshold;
      lastTime = newTime;
      return ans;
    }

    virtual ivec2 getPoint()
    {
        ivec2 point;
        int x, y, w, x0, y0, x1, y1, x_sum, y_sum, xx, yy;
        int iterations = 3;
        for(int i = iterations; i > 0; i--){
            //if (tft.getTouchRawZ() > 200) {
            tft.getTouchRaw(&x0, &y0);

            if(i == iterations)
            {
                x1 = x0;
                y1 = y0;
            }

            if (pow(pow((x0 - x1) + (x1 - x0), 2) + pow((y0 - y1) + (y1 - y0), 2), 0.5) > 50) {
                iterations--;
                continue;
            }
            else {
                x_sum += x0;
                y_sum += y0;
                x1 = x0;
                y1 = y0;
            }
                
        }
      xx = x_sum / iterations;
      yy = y_sum / iterations;

      xx = constrain(map(x0, 140, 4000, 0, 320), 0, 319);
      yy = constrain(480 - map(y0, 245, 4000, 0, 480), 0, 480);

      switch (tft.getRotation()) {
        case 1:
          x = yy;
          y = 319 - xx;
          break;
        case 2:
          x = 319 - xx;
          y = 479 - yy;
          break;
        case 3:
          x = 479 - yy;
          y = xx;
          break;
      }
      currentPosition = ivec2(x,y);
      return currentPosition;
    }
};

HyperMap<int,Touch_XPT2046*> xptMap;

void registerXPT(int isrPin, Touch_XPT2046 *xpt)
{   
    xptMap.put(isr_pin, xpt);
    void* pinValue = reinterpret_cast<void*>(ist_pin);
    attachInterruptArg(digitalPinToInterrupt(isr_pin), xpt2046_Handler, pinValue, CHANGE);  // CHANGE triggers on both press and release
}

void IRAM_ATTR xpt2046_Handler(void *arg)
{
  int pinValue = reinterpret_cast<int>(arg);
  Touch_XPT2046 *xpt = xptMap.get(pinValue);
  if (xpt->outOfDebounceThreshold())
  {
    xpt->getPoint();
    
  }
  xpt->next();
}

#endif // TOUCH_XPT2046_HPP