#pragma once

#include "WavetableSet.h"
#include <cmath>

/**
    Oscilador wavetable. No posee sus propias tablas — lee de un
    WavetableSet compartido (una instancia por plugin, referenciada por
    todas las voces), para no duplicar la biblioteca de wavetables en cada
    una de las voces de polifonía.
*/
class Oscillator
{
public:
    void setWavetableSet (const WavetableSet* set) noexcept { wavetableSet = set; }
    void setSampleRate (double newSampleRate) noexcept { sampleRate = newSampleRate; }
    void setWavetableIndex (int index) noexcept { tableIndex = index; }
    void setWavePosition (float pos01) noexcept { position = juce::jlimit (0.0f, 1.0f, pos01); }
    void setLevel (float level01) noexcept { level = level01; }

    /** 0 = sin efecto (interpolación normal). 1 = resolución de lectura
        muy reducida — aliasing deliberado, carácter "vintage" (Fase 8). */
    void setAliasingAmount (float amt01) noexcept { aliasingAmount = juce::jlimit (0.0f, 1.0f, amt01); }

    void setFrequency (float freqHz) noexcept
    {
        if (sampleRate > 0.0)
            phaseIncrement = (double) freqHz / sampleRate;
    }

    void resetPhase() noexcept { phase = 0.0; }

    float renderSample() noexcept
    {
        if (wavetableSet == nullptr || wavetableSet->getNumWavetables() == 0)
            return 0.0f;

        double readPhase = phase;
        if (aliasingAmount > 0.0f)
        {
            // Reduce la resolución efectiva de lectura del ciclo, anulando
            // la interpolación y produciendo aliasing deliberado.
            const double steps = juce::jmap ((double) aliasingAmount, 0.0, 1.0, 2048.0, 32.0);
            readPhase = std::floor (phase * steps) / steps;
        }

        const auto& table = wavetableSet->getWavetable (tableIndex);
        const float sample = table.getSample (readPhase, position) * level;

        phase += phaseIncrement;
        if (phase >= 1.0)
            phase -= 1.0;

        return sample;
    }

private:
    const WavetableSet* wavetableSet = nullptr;
    int tableIndex = 0;
    float position = 0.0f;
    float level = 1.0f;
    float aliasingAmount = 0.0f;

    double sampleRate = 44100.0;
    double phase = 0.0;
    double phaseIncrement = 0.0;
};
