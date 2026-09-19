#pragma once

#include <juce_dsp/juce_dsp.h>

/** Distorsión/saturación: waveshaper (tanh) seguido de un filtro de tono
    (low-pass), mezclado con la señal seca. */
class Drive
{
public:
    void prepare (double sampleRate, int samplesPerBlock) noexcept;
    void reset() noexcept;

    void setAmount (float amount01) noexcept { amount = juce::jlimit (0.0f, 1.0f, amount01); }
    void setTone (float tone01) noexcept;
    void setMix (float mix01) noexcept { mix = juce::jlimit (0.0f, 1.0f, mix01); }

    void process (juce::AudioBuffer<float>& buffer) noexcept;

private:
    juce::dsp::StateVariableTPTFilter<float> toneFilter;
    float amount = 0.3f;
    float mix = 1.0f;
};
