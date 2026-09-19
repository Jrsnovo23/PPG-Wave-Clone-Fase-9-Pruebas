#include "Chorus.h"

void Chorus::prepare (double sampleRate, int samplesPerBlock, int numChannels)
{
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32) samplesPerBlock, (juce::uint32) numChannels };
    chorus.prepare (spec);
    chorus.setCentreDelay (7.0f);
    chorus.setFeedback (0.0f);
}

void Chorus::reset() { chorus.reset(); }

void Chorus::setRateHz (float hz) { chorus.setRate (juce::jmax (0.01f, hz)); }
void Chorus::setDepth (float depth01) { chorus.setDepth (juce::jlimit (0.0f, 1.0f, depth01)); }
void Chorus::setMix (float mix01) { chorus.setMix (juce::jlimit (0.0f, 1.0f, mix01)); }

void Chorus::process (juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> block (buffer);
    juce::dsp::ProcessContextReplacing<float> context (block);
    chorus.process (context);
}
