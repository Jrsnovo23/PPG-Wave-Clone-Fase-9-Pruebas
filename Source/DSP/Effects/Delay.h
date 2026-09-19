#pragma once

#include <juce_dsp/juce_dsp.h>

/** Delay estéreo con feedback y sync opcional al tempo del host (reutiliza
    las mismas divisiones que LFOSyncDivisionChoices — ver LFO.h). */
class Delay
{
public:
    void prepare (double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void setTimeSeconds (float seconds) noexcept { freeTimeSeconds = juce::jmax (0.001f, seconds); }
    void setTempoSynced (bool shouldSync) noexcept { synced = shouldSync; }
    void setSyncDivisionIndex (int index) noexcept { syncDivisionIndex = index; }
    void setHostBpm (double bpm) noexcept { hostBpm = bpm; }
    void setFeedback (float fb01) noexcept { feedback = juce::jlimit (0.0f, 0.95f, fb01); }
    void setMix (float mix01) noexcept { mix = juce::jlimit (0.0f, 1.0f, mix01); }

    void process (juce::AudioBuffer<float>& buffer);

private:
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine { 192000 };
    double sampleRate = 44100.0;
    float freeTimeSeconds = 0.3f;
    bool synced = false;
    int syncDivisionIndex = 2;
    double hostBpm = 120.0;
    float feedback = 0.3f;
    float mix = 0.3f;
};
