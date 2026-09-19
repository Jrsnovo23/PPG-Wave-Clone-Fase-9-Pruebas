#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PPGLookAndFeel.h"

/** LED simple: un círculo que se enciende (ámbar) o apaga (gris oscuro). */
class StatusLED : public juce::Component
{
public:
    void setLit (bool shouldBeLit) noexcept
    {
        if (lit != shouldBeLit)
        {
            lit = shouldBeLit;
            repaint();
        }
    }

    void paint (juce::Graphics& g) override
    {
        auto bounds = getLocalBounds().toFloat().reduced (1.0f);
        g.setColour (PPGLookAndFeel::panelBackground.darker (0.3f));
        g.fillEllipse (bounds);
        g.setColour (lit ? PPGLookAndFeel::ledAmber : PPGLookAndFeel::panelBorder);
        g.fillEllipse (bounds.reduced (bounds.getWidth() * 0.22f));
    }

private:
    bool lit = false;
};
