#ifndef HAVE_A_GOOD_WEEK_HPP
#define HAVE_A_GOOD_WEEK_HPP

#include "GUI_Manager.hpp"
#include <Arduino.h>

class HaveAGoodWeek : public Activity {
public:
    // Elements
    Background background;       // A calming gradient or solid background
    Text titleText;              // Title text displayed in the header
    Text subtitleText;           // Subtitle with "Have a Good Week"
    Text signatureText;          // Signature text
    Ellipse* cloud1[5];           // First cloud made of multiple ellipses
    Ellipse *cloud2[5];           // Second cloud made of multiple ellipses
    Ellipse* cloud3[5];           // Third cloud made of multiple ellipses
    Circle sun;                  // A sun element symbolizing positivity

    // Constructor
    HaveAGoodWeek() : Activity(),
        background(TFT_SKYBLUE),
        titleText(Element(ivec2(30, 20), LuminaUI_AUTO, ivec2(180,300)), String("Dear group:"), TFT_DARKCYAN, 1, TL_DATUM, 1, &FreeSansBold12pt7b),
        subtitleText(Element(ivec2(90, 90), LuminaUI_AUTO, ivec2(230,100)), String("Have a Good Week!"), TFT_WHITE, 1, TC_DATUM, 1, &FreeSansOblique18pt7b),
        signatureText(Element(ivec2(50, 175), LuminaUI_AUTO, LuminaUI_AUTO), String("From Roi"), TFT_DARKGREY, 1, TC_DATUM, 1, &FreeSans9pt7b),
        sun(Element(ivec2(230, 20), LuminaUI_AUTO, ivec2(30, 30)), TFT_YELLOW, TFT_ORANGE, true, true)
    {
        // Define clouds with 4-5 ellipses each
        cloud1[0] = new Ellipse(Element(ivec2(50, 150), LuminaUI_AUTO, ivec2(40, 25)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud1[1] = new Ellipse(Element(ivec2(70, 140), LuminaUI_AUTO, ivec2(50, 30)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud1[2] = new Ellipse(Element(ivec2(90, 150), LuminaUI_AUTO, ivec2(40, 25)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud1[3] = new Ellipse(Element(ivec2(70, 160), LuminaUI_AUTO, ivec2(60, 35)), TFT_WHITE, TFT_SKYBLUE, true, true);

        cloud2[0] = new Ellipse(Element(ivec2(110, 180), LuminaUI_AUTO, ivec2(50, 25)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud2[1] = new Ellipse(Element(ivec2(130, 170), LuminaUI_AUTO, ivec2(60, 30)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud2[2] = new Ellipse(Element(ivec2(150, 180), LuminaUI_AUTO, ivec2(50, 25)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud2[3] = new Ellipse(Element(ivec2(130, 190), LuminaUI_AUTO, ivec2(70, 35)), TFT_WHITE, TFT_SKYBLUE, true, true);

        cloud3[0] = new Ellipse(Element(ivec2(170, 130), LuminaUI_AUTO, ivec2(40, 25)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud3[1] = new Ellipse(Element(ivec2(190, 120), LuminaUI_AUTO, ivec2(50, 30)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud3[2] = new Ellipse(Element(ivec2(210, 130), LuminaUI_AUTO, ivec2(40, 25)), TFT_WHITE, TFT_SKYBLUE, true, true);
        cloud3[3] = new Ellipse(Element(ivec2(190, 140), LuminaUI_AUTO, ivec2(60, 35)), TFT_WHITE, TFT_SKYBLUE, true, true);

        ivec2 o1 = ivec2(-30, -50);
        ivec2 o2 = ivec2(100, 20);
        ivec2 o3 = ivec2(-110, 20);
        for (int i = 0; i < 4; i++) {
            cloud1[i]->origin+=o1;
            cloud2[i]->origin+=o2;
            cloud3[i]->origin+=o3;
        }

        // Add all elements to the `elements` HyperList using addFromArray
        Element* elems[] = {&background, 
                            cloud1[0], cloud1[1], cloud1[2], cloud1[3],
                            cloud2[0], cloud2[1], cloud2[2], cloud2[3],
                            cloud3[0], cloud3[1], cloud3[2], cloud3[3], &sun,
                            &titleText, &subtitleText, &signatureText,
                            };
        elements.addFromArray(elems, 17);
    }

    /**
     * @brief Override the onTouch behavior to handle interactions
     * @param point The point of touch.
     * @param touchStatus The status of the touch.
     */
    virtual void OnTouch_execute(ivec2 point, TouchStatus touchStatus) override {
        // No touch interactions for this activity.
    }
};

#endif // HAVE_A_GOOD_WEEK_HPP
