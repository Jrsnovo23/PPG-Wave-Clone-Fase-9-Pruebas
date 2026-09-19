#include "EnvelopeVisualizer.h"
#include "PPGLookAndFeel.h"
#include <cmath>

EnvelopeVisualizer::EnvelopeVisualizer (juce::AudioProcessorValueTreeState& apvtsIn,
                                         juce::String attackParamID, juce::String decayParamID,
                                         juce::String sustainParamID, juce::String releaseParamID)
    : apvts (apvtsIn),
      attackID (std::move (attackParamID)), decayID (std::move (decayParamID)),
      sustainID (std::move (sustainParamID)), releaseID (std::move (releaseParamID))
{
    startTimerHz (15);
}

EnvelopeVisualizer::~EnvelopeVisualizer()
{
    stopTimer();
}

void EnvelopeVisualizer::timerCallback()
{
    const float a = apvts.getRawParameterValue (attackID)->load();
    const float d = apvts.getRawParameterValue (decayID)->load();
    const float s = apvts.getRawParameterValue (sustainID)->load();
    const float r = apvts.getRawParameterValue (releaseID)->load();

    if (a != lastAttack || d != lastDecay || s != lastSustain || r != lastRelease)
    {
        lastAttack = a; lastDecay = d; lastSustain = s; lastRelease = r;
        repaint();
    }
}

void EnvelopeVisualizer::paint (juce::Graphics& g)
{
    auto bounds = getLocalBounds().toFloat().reduced (1.0f);
    g.setColour (PPGLookAndFeel::panelBackground.darker (0.15f));
    g.fillRoundedRectangle (bounds, 3.0f);

    const float attack  = juce::jlimit (0.001f, 5.0f, apvts.getRawParameterValue (attackID)->load());
    const float decay   = juce::jlimit (0.001f, 5.0f, apvts.getRawParameterValue (decayID)->load());
    const float sustain = juce::jlimit (0.0f, 1.0f, apvts.getRawParameterValue (sustainID)->load());
    const float release = juce::jlimit (0.001f, 5.0f, apvts.getRawParameterValue (releaseID)->load());

    // Peso perceptual (log) del tiempo real de cada segmento -- así el
    // gráfico refleja de verdad Attack/Decay/Release, no solo Sustain.
    const auto timeWeight = [] (float seconds) { return std::log10 (1.0f + seconds * 20.0f); };
    const float aW = timeWeight (attack);
    const float dW = timeWeight (decay);
    const float rW = timeWeight (release);
    const float totalADR = juce::jmax (0.0001f, aW + dW + rW);

    const float w = bounds.getWidth();
    const float sustainW = w * 0.16f; // el plateau no tiene "tiempo", ancho fijo
    const float adrWidth = w - sustainW;

    const float attackW  = adrWidth * (aW / totalADR);
    const float decayW   = adrWidth * (dW / totalADR);
    const float releaseW = adrWidth * (rW / totalADR);

    const float baseY = bounds.getBottom() - 3.0f;
    const float topY = bounds.getY() + 3.0f;
    const float sustainY = juce::jmap (sustain, 0.0f, 1.0f, baseY, topY);

    juce::Path path;
    float x = bounds.getX();
    path.startNewSubPath (x, baseY);
    x += attackW;
    path.lineTo (x, topY);
    x += decayW;
    path.lineTo (x, sustainY);
    x += sustainW;
    path.lineTo (x, sustainY);
    x += releaseW;
    path.lineTo (x, baseY);

    g.setColour (PPGLookAndFeel::accent);
    g.strokePath (path, juce::PathStrokeType (1.6f, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
}
