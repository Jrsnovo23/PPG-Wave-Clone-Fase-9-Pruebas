#pragma once

#include <juce_core/juce_core.h>

/**
    Encapsula el "carácter digital vintage" de una voz: reducción de bit
    depth, reducción de sample rate (decimación tipo sample & hold), ruido
    digital, deriva de pitch (drift), y variación fija por nota de
    filtro/voz — simulando imperfecciones de hardware digital clásico.

    Todo se escala por el control global Vintage/Modern (0 = Modern, sin
    carácter añadido; 1 = la cantidad completa configurada en cada
    parámetro individual). El aliasing deliberado (reducción de resolución
    de lectura de la wavetable) vive en Oscillator, no aquí — ver
    Oscillator::setAliasingAmount().

    Instancia por voz (no compartida), para que "Voice Variation" sea una
    diferencia real entre voces, tal como pide el documento de diseño.
*/
class VintageCharacter
{
public:
    void setSampleRate (double sr) noexcept { sampleRate = sr; }

    void setParameters (float vintageAmountIn, float bitDepthIn, float sampleRateReductionIn,
                         float noiseAmountIn, float driftAmountIn,
                         float filterVariationIn, float voiceVariationIn) noexcept;

    /** Redibuja las variaciones fijas por nota (filtro/voz) y reinicia el
        estado de drift/decimación. Llamar en cada noteOn. */
    void noteOn() noexcept;

    /** Avanza el drift de pitch un paso; llamar una vez por muestra. */
    void advanceDrift() noexcept;

    /** Offset de pitch por drift, en semitonos, para sumar a la frecuencia
        base de los osciladores de la voz. */
    float getDriftSemitones() const noexcept { return currentDrift * driftDepthSemitones; }

    /** Multiplicador (~1 +/- x) fijo por nota para el cutoff del filtro. */
    float getFilterVariationMultiplier() const noexcept { return filterVariationMultiplier; }

    /** Multiplicador (~1 +/- x) fijo por nota para el nivel de salida. */
    float getVoiceVariationMultiplier() const noexcept { return voiceVariationMultiplier; }

    /** Bit depth + sample rate reduction + ruido, aplicado a la señal de
        salida final de la voz (post-filtro). Llamar una vez por muestra. */
    float processOutputSample (float input) noexcept;

private:
    double sampleRate = 44100.0;

    float vintageAmount = 0.0f;
    float bitDepthBits = 16.0f;
    float sampleRateReduction01 = 0.0f;
    float noiseAmount01 = 0.0f;
    float driftAmount01 = 0.0f;
    float filterVariationAmount01 = 0.0f;
    float voiceVariationAmount01 = 0.0f;

    static constexpr float driftDepthSemitones = 0.3f;
    static constexpr float filterVariationMaxPercent = 0.15f;
    static constexpr float voiceVariationMaxPercent = 0.10f;

    float filterVariationMultiplier = 1.0f;
    float voiceVariationMultiplier = 1.0f;

    // Drift: paseo aleatorio suavizado — nuevo objetivo cada cierto tiempo,
    // interpolado linealmente hacia él para no saltar de golpe.
    float currentDrift = 0.0f;
    float driftTarget = 0.0f;
    double samplesUntilNewDriftTarget = 0.0;

    // Decimación (sample & hold) para la reducción de sample rate.
    float heldSample = 0.0f;
    int samplesSinceHold = 0;

    juce::Random random;
    float nextRandomBipolar() noexcept { return random.nextFloat() * 2.0f - 1.0f; }
};
