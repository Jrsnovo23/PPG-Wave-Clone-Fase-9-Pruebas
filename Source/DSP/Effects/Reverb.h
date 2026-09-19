#pragma once

#include <juce_dsp/juce_dsp.h>

/** Envoltorio sobre juce::dsp::Reverb. Size y Damping son los dos
    parámetros reales del algoritmo (roomSize/damping) — no existe un
    "Decay" independiente en juce::dsp::Reverb, por eso no se expone por
    separado (ver README, Problemas conocidos de esta fase). */
class Reverb
{
public:
    void prepare (double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void setSize (float size01);
    void setDamping (float damping01);
    void setMix (float mix01);

    void process (juce::AudioBuffer<float>& buffer);

private:
    juce::dsp::Reverb reverb;
    juce::dsp::Reverb::Parameters parameters;
};
