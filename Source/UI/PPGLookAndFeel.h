#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

/**
    LookAndFeel propio del plugin: estética de instrumento digital de los
    años 80 (paneles oscuros, acentos cian/ámbar, knobs con arco de valor).
    Interpretación original — sin logotipos, texturas ni gráficos de
    terceros, y sin copiar el panel del hardware original.

    Se aplica una sola vez al editor completo vía setLookAndFeel(), así que
    todos los Slider/ComboBox/Button/ToggleButton ya existentes se
    redibujan solos con este estilo, sin tocar su código de configuración.
*/
class PPGLookAndFeel : public juce::LookAndFeel_V4
{
public:
    PPGLookAndFeel();

    void drawRotarySlider (juce::Graphics&, int x, int y, int width, int height,
                            float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                            juce::Slider&) override;

    void drawComboBox (juce::Graphics&, int width, int height, bool isButtonDown,
                        int buttonX, int buttonY, int buttonW, int buttonH,
                        juce::ComboBox&) override;

    void drawToggleButton (juce::Graphics&, juce::ToggleButton&,
                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawButtonBackground (juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
                                bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    juce::Font getComboBoxFont (juce::ComboBox&) override;
    juce::Font getTextButtonFont (juce::TextButton&, int buttonHeight) override;

    static const juce::Colour background;
    static const juce::Colour panelBackground;
    static const juce::Colour panelBorder;
    static const juce::Colour accent;
    static const juce::Colour accentDim;
    static const juce::Colour ledAmber;
    static const juce::Colour textPrimary;
    static const juce::Colour textDim;
};
