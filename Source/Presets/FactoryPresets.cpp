#include "FactoryPresets.h"
#include "../Params/ParameterIDs.h"
#include "../DSP/ModulationMatrix.h"

namespace
{
    using Override = std::pair<juce::String, float>;

    std::vector<Override> digitalBass()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 3.0f }, // Square
            { ParamIDs::oscOctave (1), -1.0f },
            { ParamIDs::oscWavetableIndex (2), 2.0f }, // Saw
            { ParamIDs::oscOctave (2), -1.0f },
            { ParamIDs::oscFineTune (2), 8.0f },
            { ParamIDs::oscLevel (2), 0.6f },
            { ParamIDs::filterCutoff, 900.0f },
            { ParamIDs::filterResonance, 0.25f },
            { ParamIDs::filterEnvAmount, 0.6f },
            { ParamIDs::envAttack (1), 0.002f },
            { ParamIDs::envDecay (1), 0.15f },
            { ParamIDs::envSustain (1), 0.6f },
            { ParamIDs::envRelease (1), 0.15f },
            { ParamIDs::envDecay (2), 0.2f },
            { ParamIDs::envSustain (2), 0.1f },
            { ParamIDs::driveEnabled, 1.0f },
            { ParamIDs::driveAmount, 0.3f },
            { ParamIDs::driveTone, 0.4f },
            { ParamIDs::driveMix, 0.5f },
            { ParamIDs::reverbEnabled, 0.0f },
        };
    }

    std::vector<Override> glassLead()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 5.0f }, // Harmonic Morph
            { ParamIDs::oscWavePosition (1), 0.3f },
            { ParamIDs::oscWavetableIndex (2), 0.0f }, // Sine
            { ParamIDs::oscOctave (2), 1.0f },
            { ParamIDs::oscLevel (2), 0.3f },
            { ParamIDs::filterCutoff, 4500.0f },
            { ParamIDs::filterResonance, 0.3f },
            { ParamIDs::envAttack (1), 0.01f },
            { ParamIDs::envDecay (1), 0.4f },
            { ParamIDs::envSustain (1), 0.8f },
            { ParamIDs::envRelease (1), 0.5f },
            { ParamIDs::lfoRate (1), 0.4f },
            { ParamIDs::lfoDepth (1), 0.5f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo1 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::osc1WavePosition },
            { ParamIDs::modSlotAmount (1), 0.4f },
            { ParamIDs::chorusEnabled, 1.0f },
            { ParamIDs::chorusRate, 0.25f },
            { ParamIDs::chorusDepth, 0.35f },
            { ParamIDs::chorusMix, 0.35f },
        };
    }

    std::vector<Override> crystalPad()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 4.0f }, // PWM
            { ParamIDs::oscWavetableIndex (2), 5.0f }, // Harmonic Morph
            { ParamIDs::oscLevel (2), 0.6f },
            { ParamIDs::filterCutoff, 3000.0f },
            { ParamIDs::filterResonance, 0.1f },
            { ParamIDs::filterEnvAmount, 0.3f },
            { ParamIDs::envAttack (1), 1.2f },
            { ParamIDs::envDecay (1), 1.0f },
            { ParamIDs::envSustain (1), 0.9f },
            { ParamIDs::envRelease (1), 2.0f },
            { ParamIDs::envAttack (2), 1.5f },
            { ParamIDs::lfoWaveform (1), 1.0f }, // Triangle
            { ParamIDs::lfoRate (1), 0.15f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo1 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::osc2WavePosition },
            { ParamIDs::modSlotAmount (1), 0.5f },
            { ParamIDs::chorusEnabled, 1.0f },
            { ParamIDs::chorusMix, 0.25f },
            { ParamIDs::reverbEnabled, 1.0f },
            { ParamIDs::reverbSize, 0.7f },
            { ParamIDs::reverbMix, 0.4f },
        };
    }

    std::vector<Override> digitalKeys()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 1.0f }, // Triangle
            { ParamIDs::oscWavetableIndex (2), 3.0f }, // Square
            { ParamIDs::oscLevel (2), 0.25f },
            { ParamIDs::filterCutoff, 3500.0f },
            { ParamIDs::envAttack (1), 0.005f },
            { ParamIDs::envDecay (1), 0.6f },
            { ParamIDs::envSustain (1), 0.5f },
            { ParamIDs::envRelease (1), 0.4f },
            { ParamIDs::reverbEnabled, 1.0f },
            { ParamIDs::reverbSize, 0.3f },
            { ParamIDs::reverbMix, 0.15f },
        };
    }

    std::vector<Override> metallicBell()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 5.0f }, // Harmonic Morph
            { ParamIDs::oscWavePosition (1), 0.8f },
            { ParamIDs::oscWavetableIndex (2), 0.0f }, // Sine
            { ParamIDs::oscOctave (2), 1.0f },
            { ParamIDs::oscCoarseTune (2), 7.0f }, // quinta
            { ParamIDs::filterType, 1.0f }, // High Pass
            { ParamIDs::filterCutoff, 800.0f },
            { ParamIDs::envAttack (1), 0.001f },
            { ParamIDs::envDecay (1), 1.5f },
            { ParamIDs::envSustain (1), 0.0f },
            { ParamIDs::envRelease (1), 1.5f },
            { ParamIDs::lfoRate (2), 3.5f },
            { ParamIDs::modSlotSource (2), (float) ModulationSources::lfo2 },
            { ParamIDs::modSlotDestination (2), (float) ModulationDestinations::filterCutoff },
            { ParamIDs::modSlotAmount (2), 0.15f },
            { ParamIDs::reverbEnabled, 1.0f },
            { ParamIDs::reverbSize, 0.5f },
            { ParamIDs::reverbMix, 0.3f },
        };
    }

    std::vector<Override> digitalPluck()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 2.0f }, // Saw
            { ParamIDs::oscLevel (2), 0.0f },
            { ParamIDs::filterCutoff, 2500.0f },
            { ParamIDs::filterResonance, 0.35f },
            { ParamIDs::filterEnvAmount, 0.8f },
            { ParamIDs::envAttack (1), 0.001f },
            { ParamIDs::envDecay (1), 0.25f },
            { ParamIDs::envSustain (1), 0.0f },
            { ParamIDs::envRelease (1), 0.1f },
            { ParamIDs::envAttack (2), 0.001f },
            { ParamIDs::envDecay (2), 0.15f },
            { ParamIDs::envSustain (2), 0.0f },
            { ParamIDs::delayEnabled, 1.0f },
            { ParamIDs::delaySynced, 1.0f },
            { ParamIDs::delaySyncDivision, 2.0f }, // 1/4
            { ParamIDs::delayFeedback, 0.2f },
            { ParamIDs::delayMix, 0.2f },
        };
    }

    std::vector<Override> analogSequence()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 3.0f }, // Square
            { ParamIDs::oscWavetableIndex (2), 4.0f }, // PWM
            { ParamIDs::oscLevel (2), 0.4f },
            { ParamIDs::filterCutoff, 1800.0f },
            { ParamIDs::filterResonance, 0.3f },
            { ParamIDs::envAttack (1), 0.001f },
            { ParamIDs::envDecay (1), 0.12f },
            { ParamIDs::envSustain (1), 0.3f },
            { ParamIDs::envRelease (1), 0.08f },
            { ParamIDs::lfoRate (1), 4.0f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo1 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::filterCutoff },
            { ParamIDs::modSlotAmount (1), 0.3f },
            { ParamIDs::delayEnabled, 1.0f },
            { ParamIDs::delaySynced, 1.0f },
            { ParamIDs::delaySyncDivision, 1.0f }, // 1/8
            { ParamIDs::delayFeedback, 0.35f },
            { ParamIDs::delayMix, 0.3f },
        };
    }

    std::vector<Override> sciFiFX()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 5.0f },
            { ParamIDs::oscWavetableIndex (2), 4.0f },
            { ParamIDs::filterType, 2.0f }, // Band Pass
            { ParamIDs::filterCutoff, 2200.0f },
            { ParamIDs::filterResonance, 0.5f },
            { ParamIDs::envAttack (1), 0.5f },
            { ParamIDs::envRelease (1), 2.0f },
            { ParamIDs::lfoWaveform (2), 5.0f }, // Random
            { ParamIDs::lfoRate (2), 1.5f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo2 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::osc2WavePosition },
            { ParamIDs::modSlotAmount (1), 0.6f },
            { ParamIDs::modSlotSource (2), (float) ModulationSources::random },
            { ParamIDs::modSlotDestination (2), (float) ModulationDestinations::filterCutoff },
            { ParamIDs::modSlotAmount (2), 0.3f },
            { ParamIDs::delayEnabled, 1.0f },
            { ParamIDs::delayFeedback, 0.5f },
            { ParamIDs::delayMix, 0.35f },
            { ParamIDs::reverbEnabled, 1.0f },
            { ParamIDs::reverbSize, 0.8f },
            { ParamIDs::reverbMix, 0.4f },
        };
    }

    std::vector<Override> darkAtmospheric()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 4.0f },
            { ParamIDs::oscWavetableIndex (2), 1.0f },
            { ParamIDs::oscOctave (2), -1.0f },
            { ParamIDs::filterCutoff, 900.0f },
            { ParamIDs::filterResonance, 0.15f },
            { ParamIDs::envAttack (1), 2.5f },
            { ParamIDs::envDecay (1), 1.0f },
            { ParamIDs::envSustain (1), 1.0f },
            { ParamIDs::envRelease (1), 3.0f },
            { ParamIDs::lfoRate (1), 0.08f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo1 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::filterCutoff },
            { ParamIDs::modSlotAmount (1), 0.4f },
            { ParamIDs::chorusEnabled, 1.0f },
            { ParamIDs::chorusMix, 0.3f },
            { ParamIDs::reverbEnabled, 1.0f },
            { ParamIDs::reverbSize, 0.9f },
            { ParamIDs::reverbDamping, 0.6f },
            { ParamIDs::reverbMix, 0.5f },
        };
    }

    std::vector<Override> complexDigital()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 5.0f },
            { ParamIDs::oscWavetableIndex (2), 3.0f },
            { ParamIDs::filterType, 2.0f },
            { ParamIDs::filterCutoff, 2600.0f },
            { ParamIDs::filterResonance, 0.45f },
            { ParamIDs::lfoRate (2), 0.6f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo2 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::osc2WavePosition },
            { ParamIDs::modSlotAmount (1), 0.5f },
            { ParamIDs::driveEnabled, 1.0f },
            { ParamIDs::driveAmount, 0.15f },
            { ParamIDs::driveMix, 0.3f },
        };
    }

    std::vector<Override> experimentalDrift()
    {
        return {
            { ParamIDs::oscWavetableIndex (2), 5.0f },
            { ParamIDs::lfoWaveform (1), 6.0f }, // Sample & Hold
            { ParamIDs::lfoRate (1), 0.7f },
            { ParamIDs::lfoWaveform (2), 5.0f }, // Random
            { ParamIDs::lfoRate (2), 0.9f },
            { ParamIDs::modSlotSource (1), (float) ModulationSources::lfo1 },
            { ParamIDs::modSlotDestination (1), (float) ModulationDestinations::osc1Pitch },
            { ParamIDs::modSlotAmount (1), 0.08f },
            { ParamIDs::modSlotSource (2), (float) ModulationSources::lfo2 },
            { ParamIDs::modSlotDestination (2), (float) ModulationDestinations::filterCutoff },
            { ParamIDs::modSlotAmount (2), 0.5f },
            { ParamIDs::modSlotSource (3), (float) ModulationSources::random },
            { ParamIDs::modSlotDestination (3), (float) ModulationDestinations::amplifier },
            { ParamIDs::modSlotAmount (3), 0.2f },
            { ParamIDs::delayEnabled, 1.0f },
            { ParamIDs::delayFeedback, 0.45f },
            { ParamIDs::delayMix, 0.3f },
            { ParamIDs::reverbEnabled, 1.0f },
            { ParamIDs::reverbSize, 0.6f },
            { ParamIDs::reverbMix, 0.35f },
        };
    }

    std::vector<Override> digitalPercussion()
    {
        return {
            { ParamIDs::oscWavetableIndex (1), 3.0f },
            { ParamIDs::oscOctave (1), 2.0f },
            { ParamIDs::oscLevel (2), 0.0f },
            { ParamIDs::filterType, 1.0f }, // High Pass
            { ParamIDs::filterCutoff, 1200.0f },
            { ParamIDs::filterEnvAmount, 0.7f },
            { ParamIDs::envAttack (1), 0.0005f },
            { ParamIDs::envDecay (1), 0.08f },
            { ParamIDs::envSustain (1), 0.0f },
            { ParamIDs::envRelease (1), 0.05f },
            { ParamIDs::envAttack (2), 0.0005f },
            { ParamIDs::envDecay (2), 0.03f },
            { ParamIDs::envSustain (2), 0.0f },
            { ParamIDs::driveEnabled, 1.0f },
            { ParamIDs::driveAmount, 0.4f },
            { ParamIDs::driveMix, 0.6f },
            { ParamIDs::reverbEnabled, 0.0f },
        };
    }
}

const std::vector<FactoryPresets::Definition>& FactoryPresets::getDefinitions()
{
    static const std::vector<Definition> definitions {
        { "Digital Bass",       "Bass",         digitalBass() },
        { "Glass Lead",         "Lead",         glassLead() },
        { "Crystal Pad",        "Pad",          crystalPad() },
        { "Digital Keys",       "Keys",         digitalKeys() },
        { "Metallic Bell",      "Bell",         metallicBell() },
        { "Digital Pluck",      "Pluck",        digitalPluck() },
        { "Analog Sequence",    "Sequence",     analogSequence() },
        { "Sci-Fi Texture",     "FX",           sciFiFX() },
        { "Dark Wave Pad",      "Atmospheric",  darkAtmospheric() },
        { "Complex Digital",    "Digital",      complexDigital() },
        { "Experimental Drift", "Experimental", experimentalDrift() },
        { "Digital Percussion", "Percussive",   digitalPercussion() },
    };
    return definitions;
}
