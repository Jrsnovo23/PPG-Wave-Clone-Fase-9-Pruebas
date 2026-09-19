#include "Drive.h"
#include <cmath>

void Drive::prepare (double sampleRate, int samplesPerBlock) noexcept
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = 2;
    toneFilter.prepare (spec);
    toneFilter.setType (juce::dsp::StateVariableTPTFilterType::lowpass);
    setTone (0.5f);
}

void Drive::reset() noexcept
{
    toneFilter.reset();
}

void Drive::setTone (float tone01) noexcept
{
    const float hz = juce::jmap (juce::jlimit (0.0f, 1.0f, tone01), 0.0f, 1.0f, 500.0f, 15000.0f);
    toneFilter.setCutoffFrequency (hz);
}

void Drive::process (juce::AudioBuffer<float>& buffer) noexcept
{
    const float driveGain = 1.0f + amount * 15.0f;
    const float normalise = std::tanh (driveGain);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const float dry = data[n];
            float wet = std::tanh (dry * driveGain) / normalise;
            wet = toneFilter.processSample (ch, wet);
            data[n] = dry * (1.0f - mix) + wet * mix;
        }
    }
}
