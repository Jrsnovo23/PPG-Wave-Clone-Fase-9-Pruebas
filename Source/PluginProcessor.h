#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "Synth/SynthVoice.h"
#include "Synth/SynthSound.h"
#include "DSP/WavetableSet.h"
#include "DSP/Effects/EffectsChain.h"
#include "Params/SynthVoiceParameters.h"
#include "Params/EffectsParameterPointers.h"
#include "Presets/PresetManager.h"

class PPGWaveCloneAudioProcessor : public juce::AudioProcessor
{
public:
    PPGWaveCloneAudioProcessor();
    ~PPGWaveCloneAudioProcessor() override = default;

    //==============================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    //==============================================================
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override  { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    //==============================================================
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}

    //==============================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState apvts;
    PresetManager presetManager; // se inicializa con apvts; debe declararse después

    /** Se pone en un valor alto cuando llega MIDI y baja 1 por cada bloque de
        audio sin MIDI nuevo — el editor lo sondea para encender/apagar el LED
        de actividad MIDI sin necesitar callbacks cruzados entre hilos. */
    std::atomic<int> midiActivityCounter { 0 };

private:
    std::unique_ptr<WavetableSet> wavetableSet;
    SynthVoiceParameters voiceParams;
    juce::Synthesiser synth;

    EffectsChain effectsChain;
    EffectsParameterPointers effectsParams;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PPGWaveCloneAudioProcessor)
};
