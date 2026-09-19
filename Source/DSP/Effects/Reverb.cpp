#include "Reverb.h"

void Reverb::prepare (double sampleRate, int samplesPerBlock, int numChannels)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32) samplesPerBlock, (juce::uint32) numChannels };
    reverb.prepare (spec);
    parameters.width = 1.0f;
    parameters.freezeMode = 0.0f;
    reverb.setParameters (parameters);
}

void Reverb::reset() { reverb.reset(); }

void Reverb::setSize (float size01)
{
    parameters.roomSize = juce::jlimit (0.0f, 1.0f, size01);
    reverb.setParameters (parameters);
}

void Reverb::setDamping (float damping01)
{
    parameters.damping = juce::jlimit (0.0f, 1.0f, damping01);
    reverb.setParameters (parameters);
}

void Reverb::setMix (float mix01)
{
    const float m = juce::jlimit (0.0f, 1.0f, mix01);
    parameters.wetLevel = m;
    parameters.dryLevel = 1.0f - m;
    reverb.setParameters (parameters);
}

void Reverb::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    reverb.process (context);
}
