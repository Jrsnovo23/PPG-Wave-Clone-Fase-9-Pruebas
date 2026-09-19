#include "EffectsChain.h"

void EffectsChain::prepare (double sampleRate, int samplesPerBlock, int numChannels)
{
    drive.prepare (sampleRate, samplesPerBlock);
    chorus.prepare (sampleRate, samplesPerBlock, numChannels);
    delay.prepare (sampleRate, samplesPerBlock, numChannels);
    reverb.prepare (sampleRate, samplesPerBlock, numChannels);
}

void EffectsChain::reset()
{
    drive.reset();
    chorus.reset();
    delay.reset();
    reverb.reset();
}

void EffectsChain::setDriveParams (bool enabled, float amount, float tone, float mix)
{
    driveEnabled = enabled;
    drive.setAmount (amount);
    drive.setTone (tone);
    drive.setMix (mix);
}

void EffectsChain::setChorusParams (bool enabled, float rateHz, float depth, float mix)
{
    chorusEnabled = enabled;
    chorus.setRateHz (rateHz);
    chorus.setDepth (depth);
    chorus.setMix (mix);
}

void EffectsChain::setDelayParams (bool enabled, bool synced, int syncDivisionIndex, float timeSeconds,
                                    double hostBpm, float feedback, float mix)
{
    delayEnabled = enabled;
    delay.setTempoSynced (synced);
    delay.setSyncDivisionIndex (syncDivisionIndex);
    delay.setTimeSeconds (timeSeconds);
    delay.setHostBpm (hostBpm);
    delay.setFeedback (feedback);
    delay.setMix (mix);
}

void EffectsChain::setReverbParams (bool enabled, float size, float damping, float mix)
{
    reverbEnabled = enabled;
    reverb.setSize (size);
    reverb.setDamping (damping);
    reverb.setMix (mix);
}

void EffectsChain::process (juce::AudioBuffer<float>& buffer)
{
    if (driveEnabled)  drive.process (buffer);
    if (chorusEnabled) chorus.process (buffer);
    if (delayEnabled)  delay.process (buffer);
    if (reverbEnabled) reverb.process (buffer);
}
