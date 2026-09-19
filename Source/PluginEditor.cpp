#include "PluginEditor.h"
#include "Params/ParameterIDs.h"
#include "DSP/WavetableFactory.h"
#include "DSP/Filter.h"
#include "DSP/LFO.h"
#include "DSP/ModulationMatrix.h"

namespace
{
    void setupRotarySlider (juce::Slider& slider, juce::Label& label, const juce::String& labelText,
                             juce::Component& parent)
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 46, 14);
        parent.addAndMakeVisible (slider);

        label.setText (labelText, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        label.setFont (juce::Font (9.0f));
        label.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
        parent.addAndMakeVisible (label);
    }

    void layoutKnobRow (juce::Rectangle<int> row, const std::vector<std::pair<juce::Slider*, juce::Label*>>& knobs)
    {
        if (knobs.empty())
            return;

        const int knobWidth = row.getWidth() / (int) knobs.size();
        for (auto& knob : knobs)
        {
            auto slot = row.removeFromLeft (knobWidth);
            knob.first->setBounds (slot.removeFromTop (52).reduced (4, 0));
            knob.second->setBounds (slot);
        }
    }
}

PPGWaveCloneAudioProcessorEditor::PPGWaveCloneAudioProcessorEditor (PPGWaveCloneAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    setLookAndFeel (&lookAndFeel);

    titleLabel.setText ("PPG WAVE CLONE - Fase 8", juce::dontSendNotification);
    titleLabel.setJustificationType (juce::Justification::centred);
    titleLabel.setFont (juce::Font (13.0f, juce::Font::bold));
    titleLabel.setColour (juce::Label::textColourId, PPGLookAndFeel::textPrimary);
    addAndMakeVisible (titleLabel);

    addAndMakeVisible (midiLed);
    midiLedLabel.setText ("MIDI", juce::dontSendNotification);
    midiLedLabel.setFont (juce::Font (9.0f));
    midiLedLabel.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (midiLedLabel);

    setupRotarySlider (masterVolumeSlider, masterVolumeLabel, "Master Volume", *this);
    masterVolumeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::masterVolume, masterVolumeSlider);

    setupRotarySlider (vintageAmountSlider, vintageAmountLabel, "Vintage / Modern", *this);
    vintageAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageAmount, vintageAmountSlider);

    setupOscillatorControls (osc1Controls, 1);
    setupOscillatorControls (osc2Controls, 2);

    setupFilterControls();
    setupEnvelopeControls (env1Controls, 1, "ENV 1 (AMP)");
    setupEnvelopeControls (env2Controls, 2, "ENV 2 (FILTER)");

    setupLFOControls (lfo1Controls, 1);
    setupLFOControls (lfo2Controls, 2);

    modMatrixTitle.setText ("MODULATION MATRIX", juce::dontSendNotification);
    modMatrixTitle.setJustificationType (juce::Justification::centred);
    modMatrixTitle.setFont (juce::Font (10.0f, juce::Font::bold));
    modMatrixTitle.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (modMatrixTitle);

    for (int i = 0; i < 4; ++i)
        setupModSlotControls (modSlotControls[(size_t) i], i + 1);

    setupEffectControls (driveControls, "DRIVE", ParamIDs::driveEnabled,
                          ParamIDs::driveAmount, "Amount", ParamIDs::driveTone, "Tone", ParamIDs::driveMix, "Mix");
    setupEffectControls (chorusControls, "CHORUS", ParamIDs::chorusEnabled,
                          ParamIDs::chorusRate, "Rate", ParamIDs::chorusDepth, "Depth", ParamIDs::chorusMix, "Mix");
    setupEffectControls (delayControls, "DELAY", ParamIDs::delayEnabled,
                          ParamIDs::delayTime, "Time", ParamIDs::delayFeedback, "Feedback", ParamIDs::delayMix, "Mix");
    setupEffectControls (reverbControls, "REVERB", ParamIDs::reverbEnabled,
                          ParamIDs::reverbSize, "Size", ParamIDs::reverbDamping, "Damping", ParamIDs::reverbMix, "Mix");

    setupVintageControls();

    setupPresetBar();

    setSize (640, 880);
    startTimerHz (30);
}

PPGWaveCloneAudioProcessorEditor::~PPGWaveCloneAudioProcessorEditor()
{
    stopTimer();
    setLookAndFeel (nullptr);
}

void PPGWaveCloneAudioProcessorEditor::timerCallback()
{
    midiLed.setLit (processorRef.midiActivityCounter.load() > 0);
}

void PPGWaveCloneAudioProcessorEditor::setupOscillatorControls (OscillatorControls& controls, int oscNumber)
{
    controls.title.setText ("OSCILLATOR " + juce::String (oscNumber), juce::dontSendNotification);
    controls.title.setJustificationType (juce::Justification::centred);
    controls.title.setFont (juce::Font (10.0f, juce::Font::bold));
    controls.title.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (controls.title);

    controls.wavetableBox.addItemList (WavetableFactory::getDefaultWavetableNames(), 1);
    addAndMakeVisible (controls.wavetableBox);
    controls.wavetableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        processorRef.apvts, ParamIDs::oscWavetableIndex (oscNumber), controls.wavetableBox);

    setupRotarySlider (controls.positionSlider, controls.positionLabel, "Position", *this);
    controls.positionAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::oscWavePosition (oscNumber), controls.positionSlider);

    setupRotarySlider (controls.octaveSlider, controls.octaveLabel, "Octave", *this);
    controls.octaveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::oscOctave (oscNumber), controls.octaveSlider);

    setupRotarySlider (controls.coarseSlider, controls.coarseLabel, "Coarse", *this);
    controls.coarseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::oscCoarseTune (oscNumber), controls.coarseSlider);

    setupRotarySlider (controls.fineSlider, controls.fineLabel, "Fine", *this);
    controls.fineAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::oscFineTune (oscNumber), controls.fineSlider);

    setupRotarySlider (controls.levelSlider, controls.levelLabel, "Level", *this);
    controls.levelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::oscLevel (oscNumber), controls.levelSlider);
}

void PPGWaveCloneAudioProcessorEditor::setupFilterControls()
{
    filterControls.title.setText ("FILTER", juce::dontSendNotification);
    filterControls.title.setJustificationType (juce::Justification::centred);
    filterControls.title.setFont (juce::Font (10.0f, juce::Font::bold));
    filterControls.title.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (filterControls.title);

    filterControls.typeBox.addItemList (FilterTypeChoices::getNames(), 1);
    addAndMakeVisible (filterControls.typeBox);
    filterControls.typeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        processorRef.apvts, ParamIDs::filterType, filterControls.typeBox);

    setupRotarySlider (filterControls.cutoffSlider, filterControls.cutoffLabel, "Cutoff", *this);
    filterControls.cutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::filterCutoff, filterControls.cutoffSlider);

    setupRotarySlider (filterControls.resonanceSlider, filterControls.resonanceLabel, "Resonance", *this);
    filterControls.resonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::filterResonance, filterControls.resonanceSlider);

    setupRotarySlider (filterControls.keyTrackSlider, filterControls.keyTrackLabel, "Key Track", *this);
    filterControls.keyTrackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::filterKeyTrack, filterControls.keyTrackSlider);

    setupRotarySlider (filterControls.envAmountSlider, filterControls.envAmountLabel, "Env Amount", *this);
    filterControls.envAmountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::filterEnvAmount, filterControls.envAmountSlider);
}

void PPGWaveCloneAudioProcessorEditor::setupEnvelopeControls (EnvelopeControls& controls, int envNumber,
                                                                const juce::String& titleText)
{
    controls.title.setText (titleText, juce::dontSendNotification);
    controls.title.setJustificationType (juce::Justification::centred);
    controls.title.setFont (juce::Font (10.0f, juce::Font::bold));
    controls.title.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (controls.title);

    controls.visualizer = std::make_unique<EnvelopeVisualizer> (
        processorRef.apvts,
        ParamIDs::envAttack (envNumber), ParamIDs::envDecay (envNumber),
        ParamIDs::envSustain (envNumber), ParamIDs::envRelease (envNumber));
    addAndMakeVisible (*controls.visualizer);

    setupRotarySlider (controls.attackSlider, controls.attackLabel, "Attack", *this);
    controls.attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::envAttack (envNumber), controls.attackSlider);

    setupRotarySlider (controls.decaySlider, controls.decayLabel, "Decay", *this);
    controls.decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::envDecay (envNumber), controls.decaySlider);

    setupRotarySlider (controls.sustainSlider, controls.sustainLabel, "Sustain", *this);
    controls.sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::envSustain (envNumber), controls.sustainSlider);

    setupRotarySlider (controls.releaseSlider, controls.releaseLabel, "Release", *this);
    controls.releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::envRelease (envNumber), controls.releaseSlider);
}

void PPGWaveCloneAudioProcessorEditor::setupLFOControls (LFOControls& controls, int lfoNumber)
{
    controls.title.setText ("LFO " + juce::String (lfoNumber), juce::dontSendNotification);
    controls.title.setJustificationType (juce::Justification::centred);
    controls.title.setFont (juce::Font (10.0f, juce::Font::bold));
    controls.title.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (controls.title);

    controls.waveformBox.addItemList (LFOWaveformChoices::getNames(), 1);
    addAndMakeVisible (controls.waveformBox);
    controls.waveformAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        processorRef.apvts, ParamIDs::lfoWaveform (lfoNumber), controls.waveformBox);

    setupRotarySlider (controls.rateSlider, controls.rateLabel, "Rate", *this);
    controls.rateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::lfoRate (lfoNumber), controls.rateSlider);

    setupRotarySlider (controls.phaseSlider, controls.phaseLabel, "Phase", *this);
    controls.phaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::lfoPhase (lfoNumber), controls.phaseSlider);

    setupRotarySlider (controls.depthSlider, controls.depthLabel, "Depth", *this);
    controls.depthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::lfoDepth (lfoNumber), controls.depthSlider);
}

void PPGWaveCloneAudioProcessorEditor::setupModSlotControls (ModSlotControls& controls, int slotNumber)
{
    controls.sourceBox.addItemList (ModulationSources::getNames(), 1);
    addAndMakeVisible (controls.sourceBox);
    controls.sourceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        processorRef.apvts, ParamIDs::modSlotSource (slotNumber), controls.sourceBox);

    controls.destinationBox.addItemList (ModulationDestinations::getNames(), 1);
    addAndMakeVisible (controls.destinationBox);
    controls.destinationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment> (
        processorRef.apvts, ParamIDs::modSlotDestination (slotNumber), controls.destinationBox);

    controls.amountSlider.setSliderStyle (juce::Slider::LinearHorizontal);
    controls.amountSlider.setTextBoxStyle (juce::Slider::TextBoxRight, false, 42, 16);
    addAndMakeVisible (controls.amountSlider);
    controls.amountAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::modSlotAmount (slotNumber), controls.amountSlider);
}

void PPGWaveCloneAudioProcessorEditor::setupEffectControls (EffectControls& controls, const juce::String& titleText,
                                                              const juce::String& enableParamID,
                                                              const juce::String& knob1ParamID, const juce::String& knob1Text,
                                                              const juce::String& knob2ParamID, const juce::String& knob2Text,
                                                              const juce::String& knob3ParamID, const juce::String& knob3Text)
{
    controls.title.setText (titleText, juce::dontSendNotification);
    controls.title.setJustificationType (juce::Justification::centred);
    controls.title.setFont (juce::Font (10.0f, juce::Font::bold));
    controls.title.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (controls.title);

    controls.enableButton.setButtonText ("On");
    addAndMakeVisible (controls.enableButton);
    controls.enableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment> (
        processorRef.apvts, enableParamID, controls.enableButton);

    setupRotarySlider (controls.knob1Slider, controls.knob1Label, knob1Text, *this);
    controls.knob1Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, knob1ParamID, controls.knob1Slider);

    setupRotarySlider (controls.knob2Slider, controls.knob2Label, knob2Text, *this);
    controls.knob2Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, knob2ParamID, controls.knob2Slider);

    setupRotarySlider (controls.knob3Slider, controls.knob3Label, knob3Text, *this);
    controls.knob3Attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, knob3ParamID, controls.knob3Slider);
}

void PPGWaveCloneAudioProcessorEditor::setupVintageControls()
{
    vintageControls.title.setText ("VINTAGE CHARACTER", juce::dontSendNotification);
    vintageControls.title.setJustificationType (juce::Justification::centred);
    vintageControls.title.setFont (juce::Font (10.0f, juce::Font::bold));
    vintageControls.title.setColour (juce::Label::textColourId, PPGLookAndFeel::textDim);
    addAndMakeVisible (vintageControls.title);

    setupRotarySlider (vintageControls.bitDepthSlider, vintageControls.bitDepthLabel, "Bit Depth", *this);
    vintageControls.bitDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageBitDepth, vintageControls.bitDepthSlider);

    setupRotarySlider (vintageControls.sampleRateReduceSlider, vintageControls.sampleRateReduceLabel, "S.Rate Reduce", *this);
    vintageControls.sampleRateReduceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageSampleRateReduce, vintageControls.sampleRateReduceSlider);

    setupRotarySlider (vintageControls.noiseSlider, vintageControls.noiseLabel, "Noise", *this);
    vintageControls.noiseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageNoise, vintageControls.noiseSlider);

    setupRotarySlider (vintageControls.driftSlider, vintageControls.driftLabel, "Drift", *this);
    vintageControls.driftAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageDrift, vintageControls.driftSlider);

    setupRotarySlider (vintageControls.filterVariationSlider, vintageControls.filterVariationLabel, "Filter Var", *this);
    vintageControls.filterVariationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageFilterVariation, vintageControls.filterVariationSlider);

    setupRotarySlider (vintageControls.voiceVariationSlider, vintageControls.voiceVariationLabel, "Voice Var", *this);
    vintageControls.voiceVariationAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageVoiceVariation, vintageControls.voiceVariationSlider);

    setupRotarySlider (vintageControls.aliasingSlider, vintageControls.aliasingLabel, "Aliasing", *this);
    vintageControls.aliasingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processorRef.apvts, ParamIDs::vintageAliasing, vintageControls.aliasingSlider);
}

void PPGWaveCloneAudioProcessorEditor::setupPresetBar()
{
    addAndMakeVisible (presetComboBox);
    presetComboBox.onChange = [this]
    {
        const int id = presetComboBox.getSelectedId();
        if (id <= 0)
            return;

        processorRef.presetManager.loadPreset (id - 1);
        updateFavoriteButtonState();
        presetDeleteButton.setEnabled (! processorRef.presetManager.getPresetInfo (id - 1).isFactory);
    };

    addAndMakeVisible (presetPrevButton);
    presetPrevButton.onClick = [this]
    {
        processorRef.presetManager.loadPrevious();
        refreshPresetComboBox();
    };

    addAndMakeVisible (presetNextButton);
    presetNextButton.onClick = [this]
    {
        processorRef.presetManager.loadNext();
        refreshPresetComboBox();
    };

    addAndMakeVisible (presetRandomButton);
    presetRandomButton.onClick = [this]
    {
        processorRef.presetManager.randomizeParameters();
        // Ya no corresponde a ningún preset guardado en concreto.
        presetComboBox.setSelectedId (0, juce::dontSendNotification);
        updateFavoriteButtonState();
    };

    addAndMakeVisible (presetFavoriteButton);
    presetFavoriteButton.onClick = [this]
    {
        auto& manager = processorRef.presetManager;
        manager.toggleFavorite (manager.getPresetInfo (manager.getCurrentIndex()).name);
        updateFavoriteButtonState();
    };

    addAndMakeVisible (presetSaveButton);
    presetSaveButton.onClick = [this] { onSaveButtonClicked(); };

    addAndMakeVisible (presetDeleteButton);
    presetDeleteButton.onClick = [this]
    {
        processorRef.presetManager.deleteCurrentPresetIfUser();
        refreshPresetComboBox();
    };

    refreshPresetComboBox();
}

void PPGWaveCloneAudioProcessorEditor::refreshPresetComboBox()
{
    auto& manager = processorRef.presetManager;

    presetComboBox.clear (juce::dontSendNotification);
    presetComboBox.addSectionHeading ("FACTORY");
    bool addedUserHeading = false;

    for (int i = 0; i < manager.getNumPresets(); ++i)
    {
        const auto& info = manager.getPresetInfo (i);
        if (! info.isFactory && ! addedUserHeading)
        {
            presetComboBox.addSectionHeading ("USER");
            addedUserHeading = true;
        }
        presetComboBox.addItem (info.name + "  [" + info.category + "]", i + 1);
    }

    if (manager.getNumPresets() > 0)
        presetComboBox.setSelectedId (manager.getCurrentIndex() + 1, juce::dontSendNotification);

    updateFavoriteButtonState();
    presetDeleteButton.setEnabled (manager.getNumPresets() > 0
        && ! manager.getPresetInfo (manager.getCurrentIndex()).isFactory);
}

void PPGWaveCloneAudioProcessorEditor::updateFavoriteButtonState()
{
    auto& manager = processorRef.presetManager;
    if (manager.getNumPresets() == 0)
        return;

    const bool fav = manager.isFavorite (manager.getPresetInfo (manager.getCurrentIndex()).name);
    presetFavoriteButton.setButtonText (fav ? "*Fav" : "Fav");
}

void PPGWaveCloneAudioProcessorEditor::onSaveButtonClicked()
{
    saveNameWindow = std::make_unique<juce::AlertWindow> ("Guardar preset", "Nombre del preset:",
                                                           juce::MessageBoxIconType::NoIcon);
    saveNameWindow->addTextEditor ("name", "", "Nombre:");
    saveNameWindow->addButton ("Guardar", 1, juce::KeyPress (juce::KeyPress::returnKey));
    saveNameWindow->addButton ("Cancelar", 0, juce::KeyPress (juce::KeyPress::escapeKey));

    juce::Component::SafePointer<PPGWaveCloneAudioProcessorEditor> safeThis (this);

    saveNameWindow->enterModalState (true, juce::ModalCallbackFunction::create (
        [safeThis] (int result)
        {
            if (safeThis == nullptr)
                return;

            if (result == 1)
            {
                const auto name = safeThis->saveNameWindow->getTextEditorContents ("name");
                safeThis->processorRef.presetManager.saveUserPreset (name, "User");
                safeThis->refreshPresetComboBox();
            }

            safeThis->saveNameWindow.reset();
        }));
}

void PPGWaveCloneAudioProcessorEditor::registerPanel (juce::Rectangle<int> bounds)
{
    panelSections.push_back (bounds);
}

void PPGWaveCloneAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (PPGLookAndFeel::background);

    for (auto& bounds : panelSections)
    {
        auto b = bounds.toFloat();
        g.setColour (PPGLookAndFeel::panelBackground);
        g.fillRoundedRectangle (b, 5.0f);
        g.setColour (PPGLookAndFeel::panelBorder);
        g.drawRoundedRectangle (b.reduced (0.5f), 5.0f, 1.0f);
    }
}

void PPGWaveCloneAudioProcessorEditor::layoutOscillatorControls (OscillatorControls& controls, juce::Rectangle<int> area)
{
    controls.title.setBounds (area.removeFromTop (14));
    controls.wavetableBox.setBounds (area.removeFromTop (16).reduced (10, 0));
    area.removeFromTop (4);

    auto knobRow = area.removeFromTop (72);
    layoutKnobRow (knobRow, {
        { &controls.positionSlider, &controls.positionLabel },
        { &controls.octaveSlider,   &controls.octaveLabel },
        { &controls.coarseSlider,   &controls.coarseLabel },
        { &controls.fineSlider,     &controls.fineLabel },
        { &controls.levelSlider,    &controls.levelLabel },
    });
}

void PPGWaveCloneAudioProcessorEditor::layoutFilterControls (juce::Rectangle<int> area)
{
    filterControls.title.setBounds (area.removeFromTop (14));
    filterControls.typeBox.setBounds (area.removeFromTop (16).reduced (6, 0));
    area.removeFromTop (4);
    area.removeFromTop (25); // alinea con el visualizador ADSR de los paneles de envolvente

    auto knobRow = area.removeFromTop (72);
    layoutKnobRow (knobRow, {
        { &filterControls.cutoffSlider,    &filterControls.cutoffLabel },
        { &filterControls.resonanceSlider, &filterControls.resonanceLabel },
        { &filterControls.keyTrackSlider,  &filterControls.keyTrackLabel },
        { &filterControls.envAmountSlider, &filterControls.envAmountLabel },
    });
}

void PPGWaveCloneAudioProcessorEditor::layoutEnvelopeControls (EnvelopeControls& controls, juce::Rectangle<int> area)
{
    controls.title.setBounds (area.removeFromTop (14));
    area.removeFromTop (20); // alinea con el combo box de Filter
    controls.visualizer->setBounds (area.removeFromTop (22).reduced (4, 0));
    area.removeFromTop (3);

    auto knobRow = area.removeFromTop (72);
    layoutKnobRow (knobRow, {
        { &controls.attackSlider,  &controls.attackLabel },
        { &controls.decaySlider,   &controls.decayLabel },
        { &controls.sustainSlider, &controls.sustainLabel },
        { &controls.releaseSlider, &controls.releaseLabel },
    });
}

void PPGWaveCloneAudioProcessorEditor::layoutLFOControls (LFOControls& controls, juce::Rectangle<int> area)
{
    controls.title.setBounds (area.removeFromTop (14));
    controls.waveformBox.setBounds (area.removeFromTop (16).reduced (10, 0));
    area.removeFromTop (4);

    auto knobRow = area.removeFromTop (72);
    layoutKnobRow (knobRow, {
        { &controls.rateSlider,  &controls.rateLabel },
        { &controls.phaseSlider, &controls.phaseLabel },
        { &controls.depthSlider, &controls.depthLabel },
    });
}

void PPGWaveCloneAudioProcessorEditor::layoutModSlotControls (ModSlotControls& controls, juce::Rectangle<int> area)
{
    auto row = area.reduced (0, 1);
    const int comboWidth = row.getWidth() / 3;
    controls.sourceBox.setBounds (row.removeFromLeft (comboWidth).reduced (3, 0));
    controls.destinationBox.setBounds (row.removeFromLeft (comboWidth).reduced (3, 0));
    controls.amountSlider.setBounds (row.reduced (3, 0));
}

void PPGWaveCloneAudioProcessorEditor::layoutEffectControls (EffectControls& controls, juce::Rectangle<int> area)
{
    controls.title.setBounds (area.removeFromTop (14));
    controls.enableButton.setBounds (area.removeFromTop (16).withSizeKeepingCentre (54, 15));
    area.removeFromTop (4);

    auto knobRow = area.removeFromTop (72);
    layoutKnobRow (knobRow, {
        { &controls.knob1Slider, &controls.knob1Label },
        { &controls.knob2Slider, &controls.knob2Label },
        { &controls.knob3Slider, &controls.knob3Label },
    });
}

void PPGWaveCloneAudioProcessorEditor::layoutVintageControls (juce::Rectangle<int> area)
{
    vintageControls.title.setBounds (area.removeFromTop (14));
    area.removeFromTop (4);

    auto knobRow = area.removeFromTop (72);
    layoutKnobRow (knobRow, {
        { &vintageControls.bitDepthSlider,         &vintageControls.bitDepthLabel },
        { &vintageControls.sampleRateReduceSlider, &vintageControls.sampleRateReduceLabel },
        { &vintageControls.noiseSlider,            &vintageControls.noiseLabel },
        { &vintageControls.driftSlider,             &vintageControls.driftLabel },
        { &vintageControls.filterVariationSlider,   &vintageControls.filterVariationLabel },
        { &vintageControls.voiceVariationSlider,    &vintageControls.voiceVariationLabel },
        { &vintageControls.aliasingSlider,          &vintageControls.aliasingLabel },
    });
}

void PPGWaveCloneAudioProcessorEditor::layoutPresetBar (juce::Rectangle<int> area)
{
    presetPrevButton.setBounds (area.removeFromLeft (22));
    presetNextButton.setBounds (area.removeFromRight (22));

    midiLedLabel.setBounds (area.removeFromRight (32));
    midiLed.setBounds (area.removeFromRight (16).withSizeKeepingCentre (12, 12));
    area.removeFromRight (4);

    presetDeleteButton.setBounds (area.removeFromRight (48).reduced (3, 0));
    presetSaveButton.setBounds (area.removeFromRight (44).reduced (3, 0));
    presetFavoriteButton.setBounds (area.removeFromRight (38).reduced (3, 0));
    presetRandomButton.setBounds (area.removeFromRight (52).reduced (3, 0));
    presetComboBox.setBounds (area.reduced (3, 0));
}

void PPGWaveCloneAudioProcessorEditor::resized()
{
    panelSections.clear();

    auto area = getLocalBounds().reduced (12);

    auto headerArea = area.removeFromTop (22);
    layoutPresetBar (headerArea);
    area.removeFromTop (6);

    titleLabel.setBounds (area.removeFromTop (20));
    area.removeFromTop (6);

    auto masterArea = area.removeFromTop (70);
    auto masterLeft = masterArea.removeFromLeft (masterArea.getWidth() / 2);
    auto masterRight = masterArea;
    masterVolumeSlider.setBounds (masterLeft.withSizeKeepingCentre (56, 50).translated (0, -6));
    masterVolumeLabel.setBounds (masterLeft.removeFromBottom (14));
    vintageAmountSlider.setBounds (masterRight.withSizeKeepingCentre (56, 50).translated (0, -6));
    vintageAmountLabel.setBounds (masterRight.removeFromBottom (14));
    area.removeFromTop (8);

    auto oscRow = area.removeFromTop (108);
    auto osc1Area = oscRow.removeFromLeft (oscRow.getWidth() / 2).reduced (5, 0);
    auto osc2Area = oscRow.reduced (5, 0);
    registerPanel (osc1Area);
    registerPanel (osc2Area);
    layoutOscillatorControls (osc1Controls, osc1Area);
    layoutOscillatorControls (osc2Controls, osc2Area);
    area.removeFromTop (8);

    auto lowerRow = area.removeFromTop (132);
    const int thirdWidth = lowerRow.getWidth() / 3;
    auto filterArea = lowerRow.removeFromLeft (thirdWidth).reduced (5, 0);
    auto env1Area   = lowerRow.removeFromLeft (thirdWidth).reduced (5, 0);
    auto env2Area   = lowerRow.reduced (5, 0);
    registerPanel (filterArea);
    registerPanel (env1Area);
    registerPanel (env2Area);
    layoutFilterControls (filterArea);
    layoutEnvelopeControls (env1Controls, env1Area);
    layoutEnvelopeControls (env2Controls, env2Area);
    area.removeFromTop (8);

    auto lfoRow = area.removeFromTop (108);
    auto lfo1Area = lfoRow.removeFromLeft (lfoRow.getWidth() / 2).reduced (5, 0);
    auto lfo2Area = lfoRow.reduced (5, 0);
    registerPanel (lfo1Area);
    registerPanel (lfo2Area);
    layoutLFOControls (lfo1Controls, lfo1Area);
    layoutLFOControls (lfo2Controls, lfo2Area);
    area.removeFromTop (8);

    auto modArea = area.removeFromTop (102);
    registerPanel (modArea);
    modMatrixTitle.setBounds (modArea.removeFromTop (14));
    modArea.removeFromTop (3);
    for (auto& slot : modSlotControls)
    {
        layoutModSlotControls (slot, modArea.removeFromTop (18));
        modArea.removeFromTop (3);
    }
    area.removeFromTop (8);

    auto effectsRow = area.removeFromTop (108);
    const int quarterWidth = effectsRow.getWidth() / 4;
    auto driveArea  = effectsRow.removeFromLeft (quarterWidth).reduced (5, 0);
    auto chorusArea = effectsRow.removeFromLeft (quarterWidth).reduced (5, 0);
    auto delayArea  = effectsRow.removeFromLeft (quarterWidth).reduced (5, 0);
    auto reverbArea = effectsRow.reduced (5, 0);
    registerPanel (driveArea);
    registerPanel (chorusArea);
    registerPanel (delayArea);
    registerPanel (reverbArea);
    layoutEffectControls (driveControls, driveArea);
    layoutEffectControls (chorusControls, chorusArea);
    layoutEffectControls (delayControls, delayArea);
    layoutEffectControls (reverbControls, reverbArea);
    area.removeFromTop (8);

    auto vintageArea = area.removeFromTop (90);
    registerPanel (vintageArea);
    layoutVintageControls (vintageArea);
}
