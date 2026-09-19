#include "Delay.h"
#include "../LFO.h" // reutiliza LFOSyncDivisionChoices para las divisiones de tempo

void Delay::prepare (double sampleRateIn, int samplesPerBlock, int numChannels)
{
    sampleRate = sampleRateIn;
    juce::dsp::ProcessSpec spec { sampleRate, (juce::uint32) samplesPerBlock, (juce::uint32) numChannels };
    delayLine.prepare (spec);
}

void Delay::reset()
{
    delayLine.reset();
}

void Delay::process (juce::AudioBuffer<float>& buffer)
{
    const float timeSeconds = synced
        ? (float) (60.0 / hostBpm * (double) LFOSyncDivisionChoices::getQuarterNoteMultiplier (syncDivisionIndex))
        : freeTimeSeconds;

    const float delaySamples = juce::jlimit (1.0f, (float) delayLine.getMaximumDelayInSamples(),
                                              timeSeconds * (float) sampleRate);
    delayLine.setDelay (delaySamples);

    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        auto* data = buffer.getWritePointer (ch);
        for (int n = 0; n < buffer.getNumSamples(); ++n)
        {
            const float dry = data[n];
            const float delayed = delayLine.popSample (ch);
            delayLine.pushSample (ch, dry + delayed * feedback);
            data[n] = dry * (1.0f - mix) + delayed * mix;
        }
    }
}
