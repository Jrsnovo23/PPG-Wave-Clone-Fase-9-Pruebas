#pragma once

#include <juce_dsp/juce_dsp.h>

/** Envoltorio sobre juce::dsp::Chorus — implementación ya probada y
    estable, en vez de reinventar líneas de delay moduladas desde cero. */
class Chorus
{
public:
    void prepare (double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void setRateHz (float hz);
    void setDepth (float depth01);
    void setMix (float mix01);

    void process (juce::AudioBuffer<float>& buffer);

private:
    juce::dsp::Chorus<float> chorus;
};
