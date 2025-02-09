#ifndef STORY_HPP
#define STORY_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

class Story : public Activity {
public:
    // Elements
    Background background;   // Gradient sky background
    Text titleText;          // Story title
    Text storyText;          // Story content
    Text footerText;         // Footer with a small message or page number
    Rectangle textBox;       // Decorative box around the story
    Line separator;          // Separator line for aesthetics

    // Constructor
    Story() : Activity(),
        background(TFT_DARKCYAN),
        titleText(Element(ivec2(20, 30), LuminaUI_AUTO, LuminaUI_AUTO), 
                  String("\"The Wonderful Wizard of Oz\""), 
                  TFT_GOLD, 1, 1, 2.5f, &FreeSansBold12pt7b),
        textBox(Element(ivec2(10, 80), LuminaUI_AUTO, ivec2(300, 180)), 
                TFT_WHITE, TFT_DARKCYAN, true, false, 3),
        storyText(Element(ivec2(10, 80), LuminaUI_AUTO, ivec2(300, 180), true, true),
                  String(
                      "============================\n"
                      "Dorothy Gale lived in Kansas.\n"
                      "A tornado swept her house to Oz.\n\n"
                      "A girl, a dog, and a magical land.\n"
                      "That's all Dorothy needed for an unforgettable adventure.\n\n"
                      "The house landed with a thud.\n"
                      "Dorothy opened the door and discovered a whole new world."
                  ), TFT_BLACK, 1, 1, 1.3f, &FreeSerif9pt7b),
        separator(Element(ivec2(20, 270)), Line::Orientation::HORIZONTAL,TFT_GOLD, 200),
        footerText(Element(ivec2(200, 280), LuminaUI_AUTO, LuminaUI_AUTO), 
                   String("- Page 1 -"), 
                   TFT_WHITE, 1, 1, 1.5f, &FreeSansOblique9pt7b)
    {
        // Add all elements to the elements list
        Element* elems[] = {&background, &titleText, &textBox, &storyText, &separator, &footerText};
        elements.addFromArray(elems, 6);
    }

    /**
     * @brief Override the onTouch behavior to handle interactions
     * @param point The point of touch.
     * @param touchStatus The status of the touch.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        if (touchStatus == TouchStatus_PRESS) {
            switchActivity(0);
        }
    }
};

#endif // STORY_HPP
