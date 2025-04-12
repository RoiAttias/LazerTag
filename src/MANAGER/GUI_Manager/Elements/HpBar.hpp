#ifndef HPBAR_HPP
#define HPBAR_HPP

#include "MANAGER/GUI_Manager/GUI_Manager.hpp"
#include "Utilities/MoreMath.hpp"

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

    uint32_t color1;
    uint32_t color2;

    ivec2 margin = {5,5};

public:
    HpBar(const Element &element)
        : Element(element),
            border(Element(), TFT_WHITE, TFT_WHITE, false, true),
            fill(Element(), TFT_WHITE, TFT_WHITE, true, false),
            text(Element(), "ABC/DEF", TFT_BLACK, 1, MC_DATUM, 0.0f, &FreeMonoBold12pt7b)
    {
        setValue(value);
    }

    void setValue(int value)
    {
        this->value = value;
        callRender();
    }

    Viewport render(const Viewport &viewport) override
    {
        fillScale = scale - (margin * 2);
        percent = (float)value / (float)maxValue;
        textContent = String(value) + "/" + String(maxValue);

        byte r,g,b;
        float hue = max(percent-0.2f, 0.0f) / 0.8f * 0.333f;
        hueToRgb(hue, &r, &g, &b);
        color1 = LuminaUI::tft_instance->color565(r, g, b);
        byte v = 255 - (percent * 255);
        color2 = LuminaUI::tft_instance->color565(v, v, v);

        border.origin = origin;
        border.scale = scale;
        border.borderColor = color1;
        border.fillColor = color1;

        fill.origin = origin + margin;
        fill.scale = ivec2(int(fillScale.x * percent), fillScale.y);
        fill.borderColor = color1;
        fill.fillColor = color1;

        text.origin = origin + margin;
        text.scale = fillScale;
        text.content= textContent;
        text.textColor = color2;

        Viewport vp = Element::render(viewport);
        border.render(vp);
        fill.render(vp);
        text.render(vp);
        return vp;
    }
};

#endif // HPBAR_HPP