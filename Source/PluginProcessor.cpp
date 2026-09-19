#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Params/ParameterLayout.h"
#include "Params/ParameterIDs.h"
#include "DSP/WavetableFactory.h"

namespace
{
    constexpr int numVoicesPhase5 = 8; // polifonía configurable real llega en Fase 10

    OscillatorParameterPointers makeOscParams (juce::AudioProcessorValueTreeState& apvts, int oscNumber)
    {
        OscillatorParameterPointers p;
        p.wavetableIndex = apvts.getRawParameterValue (ParamIDs::oscWavetableIndex (oscNumber));
        p.wavePosition   = apvts.getRawParameterValue (ParamIDs::oscWavePosition (oscNumber));
        p.octave         = apvts.getRawParameterValue (ParamIDs::oscOctave (oscNumber));
        p.coarseTune     = apvts.getRawParameterValue (ParamIDs::oscCoarseTune (oscNumber));
        p.fineTune       = apvts.getRawParameterValue (ParamIDs::oscFineTune (oscNumber));
        p.level          = apvts.getRawParameterValue (ParamIDs::oscLevel (oscNumber));
        return p;
    }

    FilterParameterPointers makeFilterParams (juce::AudioProcessorValueTreeState& apvts)
    {
        FilterParameterPointers p;
        p.type      = apvts.getRawParameterValue (ParamIDs::filterType);
        p.cutoffHz  = apvts.getRawParameterValue (ParamIDs::filterCutoff);
        p.resonance = apvts.getRawParameterValue (ParamIDs::filterResonance);
        p.keyTrack  = apvts.getRawParameterValue (ParamIDs::filterKeyTrack);
        p.envAmount = apvts.getRawParameterValue (ParamIDs::filterEnvAmount);
        return p;
    }

    EnvelopeParameterPointers makeEnvParams (juce::AudioProcessorValueTreeState& apvts, int envNumber)
    {
        EnvelopeParameterPointers p;
        p.attack  = apvts.getRawParameterValue (ParamIDs::envAttack (envNumber));
        p.decay   = apvts.getRawParameterValue (ParamIDs::envDecay (envNumber));
        p.sustain = apvts.getRawParameterValue (ParamIDs::envSustain (envNumber));
        p.release = apvts.getRawParameterValue (ParamIDs::envRelease (envNumber));
        return p;
    }

    LFOParameterPointers makeLFOParams (juce::AudioProcessorValueTreeState& apvts, int lfoNumber)
    {
        LFOParameterPointers p;
        p.waveform     = apvts.getRawParameterValue (ParamIDs::lfoWaveform (lfoNumber));
        p.rateHz       = apvts.getRawParameterValue (ParamIDs::lfoRate (lfoNumber));
        p.synced       = apvts.getRawParameterValue (ParamIDs::lfoSynced (lfoNumber));
        p.syncDivision = apvts.getRawParameterValue (ParamIDs::lfoSyncDivision (lfoNumber));
        p.retrigger    = apvts.getRawParameterValue (ParamIDs::lfoRetrigger (lfoNumber));
        p.phase        = apvts.getRawParameterValue (ParamIDs::lfoPhase (lfoNumber));
        p.depth        = apvts.getRawParameterValue (ParamIDs::lfoDepth (lfoNumber));
        return p;
    }

    ModSlotParameterPointers makeModSlotParams (juce::AudioProcessorValueTreeState& apvts, int slotNumber)
    {
        ModSlotParameterPointers p;
        p.source      = apvts.getRawParameterValue (ParamIDs::modSlotSource (slotNumber));
        p.destination = apvts.getRawParameterValue (ParamIDs::modSlotDestination (slotNumber));
        p.amount      = apvts.getRawParameterValue (ParamIDs::modSlotAmount (slotNumber));
        return p;
    }

    VintageParameterPointers makeVintageParams (juce::AudioProcessorValueTreeState& apvts)
    {
        VintageParameterPointers p;
        p.amount           = apvts.getRawParameterValue (ParamIDs::vintageAmount);
        p.bitDepth         = apvts.getRawParameterValue (ParamIDs::vintageBitDepth);
        p.sampleRateReduce = apvts.getRawParameterValue (ParamIDs::vintageSampleRateReduce);
        p.noise            = apvts.getRawParameterValue (ParamIDs::vintageNoise);
        p.drift            = apvts.getRawParameterValue (ParamIDs::vintageDrift);
        p.filterVariation  = apvts.getRawParameterValue (ParamIDs::vintageFilterVariation);
        p.voiceVariation   = apvts.getRawParameterValue (ParamIDs::vintageVoiceVariation);
        p.aliasing         = apvts.getRawParameterValue (ParamIDs::vintageAliasing);
        return p;
    }

    SynthVoiceParameters makeSynthVoiceParameters (juce::AudioProcessorValueTreeState& apvts)
    {
        SynthVoiceParameters v;
        v.osc1 = makeOscParams (apvts, 1);
        v.osc2 = makeOscParams (apvts, 2);
        v.filter = makeFilterParams (apvts);
        v.env1 = makeEnvParams (apvts, 1);
        v.env2 = makeEnvParams (apvts, 2);
        v.lfo1 = makeLFOParams (apvts, 1);
        v.lfo2 = makeLFOParams (apvts, 2);
        for (int i = 0; i < 4; ++i)
            v.modSlots[(size_t) i] = makeModSlotParams (apvts, i + 1);
        v.vintage = makeVintageParams (apvts);
        return v;
    }

    EffectsParameterPointers makeEffectsParams (juce::AudioProcessorValueTreeState& apvts)
    {
        EffectsParameterPointers p;
        p.driveEnabled = apvts.getRawParameterValue (ParamIDs::driveEnabled);
        p.driveAmount  = apvts.getRawParameterValue (ParamIDs::driveAmount);
        p.driveTone    = apvts.getRawParameterValue (ParamIDs::driveTone);
        p.driveMix     = apvts.getRawParameterValue (ParamIDs::driveMix);

        p.chorusEnabled = apvts.getRawParameterValue (ParamIDs::chorusEnabled);
        p.chorusRate    = apvts.getRawParameterValue (ParamIDs::chorusRate);
        p.chorusDepth   = apvts.getRawParameterValue (ParamIDs::chorusDepth);
        p.chorusMix     = apvts.getRawParameterValue (ParamIDs::chorusMix);

        p.delayEnabled      = apvts.getRawParameterValue (ParamIDs::delayEnabled);
        p.delaySynced       = apvts.getRawParameterValue (ParamIDs::delaySynced);
        p.delaySyncDivision = apvts.getRawParameterValue (ParamIDs::delaySyncDivision);
        p.delayTime         = apvts.getRawParameterValue (ParamIDs::delayTime);
        p.delayFeedback     = apvts.getRawParameterValue (ParamIDs::delayFeedback);
        p.delayMix          = apvts.getRawParameterValue (ParamIDs::delayMix);

        p.reverbEnabled = apvts.getRawParameterValue (ParamIDs::reverbEnabled);
        p.reverbSize    = apvts.getRawParameterValue (ParamIDs::reverbSize);
        p.reverbDamping = apvts.getRawParameterValue (ParamIDs::reverbDamping);
        p.reverbMix     = apvts.getRawParameterValue (ParamIDs::reverbMix);
        return p;
    }
}

PPGWaveCloneAudioProcessor::PPGWaveCloneAudioProcessor()
    : AudioProcessor (BusesProperties()
                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMETERS", ParameterLayout::createParameterLayout()),
      presetManager (apvts),
      wavetableSet (WavetableFactory::createDefaultSet()),
      voiceParams (makeSynthVoiceParameters (apvts)),
      effectsParams (makeEffectsParams (apvts))
{
    for (int i = 0; i < numVoicesPhase5; ++i)
        synth.addVoice (new SynthVoice (*wavetableSet, voiceParams));

    synth.addSound (new SynthSound());
}

void PPGWaveCloneAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate (sampleRate);
    effectsChain.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
    effectsChain.reset();
}

bool PPGWaveCloneAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void PPGWaveCloneAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                                juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    if (! midiMessages.isEmpty())
        midiActivityCounter.store (15);
    else if (midiActivityCounter.load() > 0)
        midiActivityCounter.store (midiActivityCounter.load() - 1);

    // Tempo del host, para LFOs y Delay sincronizados. 120 BPM si el host
    // no reporta posición (p.ej. algunos modos standalone).
    double bpm = 120.0;
    if (auto* playHead = getPlayHead())
        if (auto position = playHead->getPosition())
            bpm = position->getBpm().orFallback (120.0);

    for (int i = 0; i < synth.getNumVoices(); ++i)
        if (auto* voice = dynamic_cast<SynthVoice*> (synth.getVoice (i)))
            voice->setHostBpm (bpm);

    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());

    effectsChain.setDriveParams (effectsParams.driveEnabled->load() > 0.5f,
                                  effectsParams.driveAmount->load(),
                                  effectsParams.driveTone->load(),
                                  effectsParams.driveMix->load());

    effectsChain.setChorusParams (effectsParams.chorusEnabled->load() > 0.5f,
                                   effectsParams.chorusRate->load(),
                                   effectsParams.chorusDepth->load(),
                                   effectsParams.chorusMix->load());

    effectsChain.setDelayParams (effectsParams.delayEnabled->load() > 0.5f,
                                  effectsParams.delaySynced->load() > 0.5f,
                                  (int) effectsParams.delaySyncDivision->load(),
                                  effectsParams.delayTime->load(),
                                  bpm,
                                  effectsParams.delayFeedback->load(),
                                  effectsParams.delayMix->load());

    effectsChain.setReverbParams (effectsParams.reverbEnabled->load() > 0.5f,
                                   effectsParams.reverbSize->load(),
                                   effectsParams.reverbDamping->load(),
                                   effectsParams.reverbMix->load());

    effectsChain.process (buffer);

    const float volumeDb = apvts.getRawParameterValue (ParamIDs::masterVolume)->load();
    buffer.applyGain (juce::Decibels::decibelsToGain (volumeDb));
}

juce::AudioProcessorEditor* PPGWaveCloneAudioProcessor::createEditor()
{
    return new PPGWaveCloneAudioProcessorEditor (*this);
}

void PPGWaveCloneAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PPGWaveCloneAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState != nullptr && xmlState->hasTagName (apvts.state.getType()))
        apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PPGWaveCloneAudioProcessor();
}
