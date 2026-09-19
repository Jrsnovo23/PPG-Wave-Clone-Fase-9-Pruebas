#pragma once

#include "Drive.h"
#include "Chorus.h"
#include "Delay.h"
#include "Reverb.h"

/** Cadena de efectos post-mezcla: procesa el buffer estéreo final después
    de sumar todas las voces (no es por voz). Orden: Drive -> Chorus ->
    Delay -> Reverb. Cada uno se activa/desactiva individualmente. */
class EffectsChain
{
public:
    void prepare (double sampleRate, int samplesPerBlock, int numChannels);
    void reset();

    void setDriveParams (bool enabled, float amount, float tone, float mix);
    void setChorusParams (bool enabled, float rateHz, float depth, float mix);
    void setDelayParams (bool enabled, bool synced, int syncDivisionIndex, float timeSeconds,
                          double hostBpm, float feedback, float mix);
    void setReverbParams (bool enabled, float size, float damping, float mix);

    void process (juce::AudioBuffer<float>& buffer);

private:
    Drive drive;   bool driveEnabled = false;
    Chorus chorus; bool chorusEnabled = false;
    Delay delay;   bool delayEnabled = false;
    Reverb reverb; bool reverbEnabled = false;
};
