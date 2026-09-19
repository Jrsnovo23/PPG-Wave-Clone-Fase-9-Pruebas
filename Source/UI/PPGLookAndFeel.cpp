#include "PPGLookAndFeel.h"
#include <cmath>

const juce::Colour PPGLookAndFeel::background      { 0xff14161c };
const juce::Colour PPGLookAndFeel::panelBackground { 0xff1c1f28 };
const juce::Colour PPGLookAndFeel::panelBorder     { 0xff3a3f4d };
const juce::Colour PPGLookAndFeel::accent          { 0xff5ac8fa };
const juce::Colour PPGLookAndFeel::accentDim       { 0xff2c5566 };
const juce::Colour PPGLookAndFeel::ledAmber        { 0xffffb454 };
const juce::Colour PPGLookAndFeel::textPrimary     { 0xffe8eaf0 };
const juce::Colour PPGLookAndFeel::textDim         { 0xff8891a3 };

PPGLookAndFeel::PPGLookAndFeel()
{
    setColour (juce::ResizableWindow::backgroundColourId, background);
    setColour (juce::Label::textColourId, textPrimary);
    setColour (juce::Slider::textBoxTextColourId, textPrimary);
    setColour (juce::Slider::textBoxOutlineColourId, panelBorder);
    setColour (juce::Slider::textBoxBackgroundColourId, panelBackground);
    setColour (juce::ComboBox::backgroundColourId, panelBackground);
    setColour (juce::ComboBox::outlineColourId, panelBorder);
    setColour (juce::ComboBox::textColourId, textPrimary);
    setColour (juce::PopupMenu::backgroundColourId, panelBackground);
    setColour (juce::PopupMenu::textColourId, textPrimary);
    setColour (juce::PopupMenu::highlightedBackgroundColourId, accentDim);
    setColour (juce::TextButton::buttonColourId, panelBackground);
    setColour (juce::TextButton::textColourOffId, textDim);
    setColour (juce::TextButton::textColourOnId, accent);
    setColour (juce::ToggleButton::textColourId, textDim);
    setColour (juce::AlertWindow::backgroundColourId, panelBackground);
    setColour (juce::AlertWindow::textColourId, textPrimary);
}

void PPGLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height,
                                        float sliderPos, float rotaryStartAngle, float rotaryEndAngle,
                                        juce::Slider&)
{
    auto bounds = juce::Rectangle<float> ((float) x, (float) y, (float) width, (float) height).reduced (2.0f);
    const float radius = juce::jmin (bounds.getWidth(), bounds.getHeight()) * 0.5f;
    const float centreX = bounds.getCentreX();
    const float centreY = bounds.getCentreY();
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    auto knobBounds = juce::Rectangle<float> (radius * 1.5f, radius * 1.5f).withCentre ({ centreX, centreY });
    g.setColour (panelBackground.brighter (0.06f));
    g.fillEllipse (knobBounds);
    g.setColour (panelBorder);
    g.drawEllipse (knobBounds, 1.0f);

    juce::Path track;
    track.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
    g.setColour (panelBorder);
    g.strokePath (track, juce::PathStrokeType (2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path valueArc;
    valueArc.addCentredArc (centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
    g.setColour (accent);
    g.strokePath (valueArc, juce::PathStrokeType (2.0f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

    juce::Path pointer;
    const float pointerLength = radius * 0.65f;
    pointer.startNewSubPath (centreX, centreY);
    pointer.lineTo (centreX + pointerLength * std::sin (angle), centreY - pointerLength * std::cos (angle));
    g.setColour (accent);
    g.strokePath (pointer, juce::PathStrokeType (2.0f));
}

void PPGLookAndFeel::drawComboBox (juce::Graphics& g, int width, int height, bool,
                                    int buttonX, int buttonY, int buttonW, int buttonH,
                                    juce::ComboBox& box)
{
    auto bounds = juce::Rectangle<float> (0, 0, (float) width, (float) height);
    g.setColour (panelBackground);
    g.fillRoundedRectangle (bounds, 3.0f);
    g.setColour (box.hasKeyboardFocus (true) ? accent : panelBorder);
    g.drawRoundedRectangle (bounds.reduced (0.5f), 3.0f, 1.0f);

    juce::Rectangle<float> arrowZone ((float) buttonX, (float) buttonY, (float) buttonW, (float) buttonH);
    juce::Path arrow;
    arrow.startNewSubPath (arrowZone.getX() + 4.0f, arrowZone.getCentreY() - 2.0f);
    arrow.lineTo (arrowZone.getCentreX(), arrowZone.getCentreY() + 3.0f);
    arrow.lineTo (arrowZone.getRight() - 4.0f, arrowZone.getCentreY() - 2.0f);
    g.setColour (accent);
    g.strokePath (arrow, juce::PathStrokeType (1.5f));
}

void PPGLookAndFeel::drawToggleButton (juce::Graphics& g, juce::ToggleButton& button, bool, bool)
{
    auto bounds = button.getLocalBounds().toFloat();
    const float ledSize = juce::jmin (14.0f, bounds.getHeight());
    auto ledBounds = juce::Rectangle<float> (ledSize, ledSize)
                          .withCentre ({ bounds.getX() + ledSize * 0.5f + 2.0f, bounds.getCentreY() });

    g.setColour (panelBackground.darker (0.2f));
    g.fillEllipse (ledBounds);
    g.setColour (button.getToggleState() ? ledAmber : panelBorder);
    g.fillEllipse (ledBounds.reduced (2.2f));

    g.setColour (button.getToggleState() ? textPrimary : textDim);
    g.setFont (juce::Font (11.0f));
    g.drawText (button.getButtonText(), bounds.withTrimmedLeft (ledSize + 6.0f), juce::Justification::centredLeft);
}

void PPGLookAndFeel::drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour&,
                                            bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat().reduced (0.5f);
    g.setColour (shouldDrawButtonAsDown ? accentDim : panelBackground.brighter (shouldDrawButtonAsHighlighted ? 0.08f : 0.0f));
    g.fillRoundedRectangle (bounds, 3.0f);
    g.setColour (button.isEnabled() ? panelBorder : panelBorder.darker (0.3f));
    g.drawRoundedRectangle (bounds, 3.0f, 1.0f);
}

juce::Font PPGLookAndFeel::getComboBoxFont (juce::ComboBox&)
{
    return juce::Font (12.0f);
}

juce::Font PPGLookAndFeel::getTextButtonFont (juce::TextButton&, int buttonHeight)
{
    return juce::Font (juce::jmin (12.0f, (float) buttonHeight * 0.6f));
}
