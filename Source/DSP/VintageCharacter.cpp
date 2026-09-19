#include "VintageCharacter.h"
#include <cmath>

void VintageCharacter::setParameters (float vintageAmountIn, float bitDepthIn, float sampleRateReductionIn,
                                       float noiseAmountIn, float driftAmountIn,
                                       float filterVariationIn, float voiceVariationIn) noexcept
{
    vintageAmount = juce::jlimit (0.0f, 1.0f, vintageAmountIn);
    bitDepthBits = bitDepthIn;
    sampleRateReduction01 = juce::jlimit (0.0f, 1.0f, sampleRateReductionIn);
    noiseAmount01 = juce::jlimit (0.0f, 1.0f, noiseAmountIn);
    driftAmount01 = juce::jlimit (0.0f, 1.0f, driftAmountIn);
    filterVariationAmount01 = juce::jlimit (0.0f, 1.0f, filterVariationIn);
    voiceVariationAmount01 = juce::jlimit (0.0f, 1.0f, voiceVariationIn);
}

void VintageCharacter::noteOn() noexcept
{
    const float filterRange = filterVariationAmount01 * vintageAmount * filterVariationMaxPercent;
    filterVariationMultiplier = 1.0f + nextRandomBipolar() * filterRange;

    const float voiceRange = voiceVariationAmount01 * vintageAmount * voiceVariationMaxPercent;
    voiceVariationMultiplier = 1.0f + nextRandomBipolar() * voiceRange;

    currentDrift = 0.0f;
    driftTarget = nextRandomBipolar();
    samplesUntilNewDriftTarget = 0.0;

    heldSample = 0.0f;
    samplesSinceHold = 0;
}

void VintageCharacter::advanceDrift() noexcept
{
    if (samplesUntilNewDriftTarget <= 0.0)
    {
        driftTarget = nextRandomBipolar();
        // Nuevo objetivo de drift cada 0.5 - 2 segundos aproximadamente.
        samplesUntilNewDriftTarget = sampleRate * (0.5 + random.nextDouble() * 1.5);
    }

    // Interpolación lineal lenta hacia el objetivo (~0.3s), sin saltos audibles.
    const float step = 1.0f / (float) juce::jmax (1.0, sampleRate * 0.3);
    if (currentDrift < driftTarget)      currentDrift = juce::jmin (driftTarget, currentDrift + step);
    else if (currentDrift > driftTarget) currentDrift = juce::jmax (driftTarget, currentDrift - step);

    samplesUntilNewDriftTarget -= 1.0;
}

float VintageCharacter::processOutputSample (float input) noexcept
{
    float output = input;

    // Reducción de sample rate (decimación tipo sample & hold).
    const float effectiveReduction = sampleRateReduction01 * vintageAmount;
    const int holdSamples = 1 + (int) (effectiveReduction * 15.0f);
    if (samplesSinceHold <= 0)
    {
        heldSample = output;
        samplesSinceHold = holdSamples;
    }
    else
    {
        output = heldSample;
    }
    --samplesSinceHold;

    // Reducción de bit depth, mezclada por vintageAmount (0 = sin efecto).
    if (vintageAmount > 0.0f)
    {
        const float bits = juce::jlimit (2.0f, 16.0f, bitDepthBits);
        const float levels = std::pow (2.0f, bits);
        const float step = 2.0f / levels;
        const float quantised = std::round (output / step) * step;
        output += (quantised - output) * vintageAmount;
    }

    // Ruido digital.
    output += nextRandomBipolar() * noiseAmount01 * vintageAmount * 0.04f;

    return output;
}
