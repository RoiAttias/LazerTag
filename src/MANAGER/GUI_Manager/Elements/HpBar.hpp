#ifndef HPBAR_HPP
#define HPBAR_HPP

#include "MANAGER/GUI_Manager/GUI_Manager.hpp"

class HpBar : public Element
{
    Rectangle border;
    Rectangle fill;
    Text text;

    int value = 100;
    int maxValue = 100;
    float percent;
    ivec2 fillScale;
    String textContent;

    uint32_t borderColor;
    uint32_t fillColor;
    uint32_t textColor;

    ivec2 margin = {10, 10};

public:
    HpBar(const Element &element, uint32_t borderColor = TFT_WHITE, uint32_t fillColor = TFT_WHITE, uint32_t textColor = TFT_GOLD)
        : Element(element), borderColor(borderColor), fillColor(fillColor), textColor(textColor),
            border(Element(), borderColor, borderColor, false, true),
            fill(Element(), fillColor, fillColor, true, false),
            text(Element(), "ABC/DEF", textColor, 1, MC_DATUM, 0.0f, &FreeMonoBold18pt7b)
    {
        fillScale = element.scale - (margin * 2);
        setValue(value);
    }

    void setValue(int value)
    {
        this->value = value;
        percent = (float)value / (float)maxValue;
        fill.scale = ivec2(int(fillScale.x * percent), fillScale.y);
        textContent = String(value) + "/" + String(maxValue);
        text.content= textContent;
    }

    Viewport render(const Viewport &viewport) override
    {
        border.origin = ivec2(0, 0);
        border.scale = scale;
        fill.origin = margin;
        // fill.scale is set in setValue
        text.origin = margin;
        text.scale = fill.scale;

        setValue(value);

        Viewport vp = Element::render(viewport);
        border.render(vp);
        fill.render(vp);
        text.render(vp);
        return vp;
    }
};


#endif // HPBAR_HPP