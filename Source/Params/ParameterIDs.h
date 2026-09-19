#pragma once

#include <juce_core/juce_core.h>

/**
    IDs de parámetros del plugin.

    IMPORTANTE: una vez publicado un preset o proyecto de Ableton Live que use
    un ID, ese ID no debe cambiar de nombre — Ableton Live guarda automatizaciones
    y estado del plugin referenciando estos strings. Nuevos parámetros de fases
    futuras se agregan aquí, nunca se renombran los existentes.
*/
namespace ParamIDs
{
    static const juce::String masterVolume { "masterVolume" };

    // Osciladores (Fase 2)
    inline juce::String oscWavetableIndex (int oscNumber) { return "osc" + juce::String (oscNumber) + "WavetableIndex"; }
    inline juce::String oscWavePosition   (int oscNumber) { return "osc" + juce::String (oscNumber) + "WavePosition"; }
    inline juce::String oscOctave         (int oscNumber) { return "osc" + juce::String (oscNumber) + "Octave"; }
    inline juce::String oscCoarseTune     (int oscNumber) { return "osc" + juce::String (oscNumber) + "CoarseTune"; }
    inline juce::String oscFineTune       (int oscNumber) { return "osc" + juce::String (oscNumber) + "FineTune"; }
    inline juce::String oscLevel          (int oscNumber) { return "osc" + juce::String (oscNumber) + "Level"; }

    // Filtro (Fase 3)
    static const juce::String filterType      { "filterType" };
    static const juce::String filterCutoff    { "filterCutoff" };
    static const juce::String filterResonance { "filterResonance" };
    static const juce::String filterKeyTrack  { "filterKeyTrack" };
    static const juce::String filterEnvAmount { "filterEnvAmount" };

    // Envolventes (Fase 3): env 1 = amplificador, env 2 = filtro
    inline juce::String envAttack  (int envNumber) { return "env" + juce::String (envNumber) + "Attack"; }
    inline juce::String envDecay   (int envNumber) { return "env" + juce::String (envNumber) + "Decay"; }
    inline juce::String envSustain (int envNumber) { return "env" + juce::String (envNumber) + "Sustain"; }
    inline juce::String envRelease (int envNumber) { return "env" + juce::String (envNumber) + "Release"; }

    // LFOs (Fase 4)
    inline juce::String lfoWaveform     (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "Waveform"; }
    inline juce::String lfoRate         (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "Rate"; }
    inline juce::String lfoSynced       (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "Synced"; }
    inline juce::String lfoSyncDivision (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "SyncDivision"; }
    inline juce::String lfoRetrigger    (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "Retrigger"; }
    inline juce::String lfoPhase        (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "Phase"; }
    inline juce::String lfoDepth        (int lfoNumber) { return "lfo" + juce::String (lfoNumber) + "Depth"; }

    // Matriz de modulación (Fase 4): 4 rutas fijas
    inline juce::String modSlotSource      (int slotNumber) { return "mod" + juce::String (slotNumber) + "Source"; }
    inline juce::String modSlotDestination (int slotNumber) { return "mod" + juce::String (slotNumber) + "Destination"; }
    inline juce::String modSlotAmount      (int slotNumber) { return "mod" + juce::String (slotNumber) + "Amount"; }

    // Efectos (Fase 5)
    static const juce::String driveEnabled { "driveEnabled" };
    static const juce::String driveAmount  { "driveAmount" };
    static const juce::String driveTone    { "driveTone" };
    static const juce::String driveMix     { "driveMix" };

    static const juce::String chorusEnabled { "chorusEnabled" };
    static const juce::String chorusRate    { "chorusRate" };
    static const juce::String chorusDepth   { "chorusDepth" };
    static const juce::String chorusMix     { "chorusMix" };

    static const juce::String delayEnabled      { "delayEnabled" };
    static const juce::String delaySynced       { "delaySynced" };
    static const juce::String delaySyncDivision { "delaySyncDivision" };
    static const juce::String delayTime         { "delayTime" };
    static const juce::String delayFeedback     { "delayFeedback" };
    static const juce::String delayMix          { "delayMix" };

    static const juce::String reverbEnabled { "reverbEnabled" };
    static const juce::String reverbSize    { "reverbSize" };
    static const juce::String reverbDamping { "reverbDamping" };
    static const juce::String reverbMix     { "reverbMix" };

    // Vintage Character (Fase 8)
    static const juce::String vintageAmount           { "vintageAmount" }; // control global Vintage/Modern
    static const juce::String vintageBitDepth         { "vintageBitDepth" };
    static const juce::String vintageSampleRateReduce { "vintageSampleRateReduce" };
    static const juce::String vintageNoise            { "vintageNoise" };
    static const juce::String vintageDrift            { "vintageDrift" };
    static const juce::String vintageFilterVariation  { "vintageFilterVariation" };
    static const juce::String vintageVoiceVariation   { "vintageVoiceVariation" };
    static const juce::String vintageAliasing         { "vintageAliasing" };
}
