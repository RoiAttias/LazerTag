#ifndef TOUCH_XPT2046_HPP
#define TOUCH_XPT2046_HPP

#include "Subsystems/TGUI/TGUI.hpp"
#include "Subsystems/TGUI/Touch.hpp"
#include "TFT_eSPI.h"
#include "Components/Pushbutton/Pushbutton.hpp"

unsigned long debounceThreshold = 5;

class Touch_XPT2046 : public Touch {
protected:
    int isr_pin;
    Pushbutton isr;
    
public:
    ivec2 lastPoint;
    // Constructors
    Touch_XPT2046(Screen * screen, int isr_pin) {
      this->isr_pin = isr_pin;
      isr = Pushbutton(isr_pin);
      Touch::Touch(screen);
    }

    virtual void init(EventHandler handler, int enable){
      instance_xpt = this;
      isr.enablePressEvent(true);
      isr.init(handler,true);
      Touch::init(enable);
    }

    virtual bool isTouched()
    {
      return isr.isPressed();
    }

    virtual ivec2 getPoint(int iterations = 1)
    {
        int x, y, w, x0, y0, x1, y1, x_sum, y_sum, xx, yy;
        for(int i = iterations; i > 0; i--)
        {
            //if (tft.getTouchRawZ() > 200) {
            tft_instance->getTouchRaw(&x0,&y0);

            if(i == iterations)
            {
                x1 = x0;
                y1 = y0;
            }

            if (sqrt(pow((x0 - x1) + (x1 - x0), 2) + pow((y0 - y1) + (y1 - y0), 2)) > 50) {
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

      xx = constrain(map(xx, 140, 4000, 0, 320), 0, 319);
      yy = constrain(480 - map(yy, 245, 4000, 0, 480), 0, 480);

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
      lastPoint = currentPosition;
      return currentPosition;
    }

    void handleInterrupt()
    {
      next(getPoint(), isTouched());
    }
};

#endif // TOUCH_XPT2046_HPP