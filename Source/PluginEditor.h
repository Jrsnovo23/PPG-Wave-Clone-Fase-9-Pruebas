#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <array>
#include <vector>
#include "PluginProcessor.h"
#include "UI/PPGLookAndFeel.h"
#include "UI/StatusLED.h"
#include "UI/EnvelopeVisualizer.h"

/**
    Editor de la Fase 7 (interfaz profesional), con el panel de Vintage
    Character añadido en la Fase 8: LookAndFeel propio (paneles oscuros,
    knobs con arco de valor, acentos cian/ámbar), paneles agrupados
    visualmente, visualización gráfica ADSR en las dos envolventes,
    indicador de actividad MIDI.

    Alcance de esta fase (lo que se deja fuera, honestamente):
    - Sin indicador de CPU (necesita infraestructura de medición de
      tiempo de audio callback que no existe todavía; no imprescindible
      para que la interfaz se vea y sienta profesional).
    - Sin buscador de texto ni filtro de categoría en vivo para presets
      (PresetManager::findMatching() ya los soporta a nivel de datos desde
      la Fase 6; conectarlos es una extensión directa de esta misma
      pantalla, no un cambio de arquitectura).
    - Sync (on/off) y Retrigger de cada LFO, y Delay Sync/Sync Division,
      siguen sin knob dedicado aquí (automatizables desde Ableton Live) —
      mismo criterio que fases anteriores, para no sobrecargar paneles ya
      compactos.
*/
class PPGWaveCloneAudioProcessorEditor : public juce::AudioProcessorEditor,
                                          private juce::Timer
{
public:
    explicit PPGWaveCloneAudioProcessorEditor (PPGWaveCloneAudioProcessor&);
    ~PPGWaveCloneAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override; // sondea actividad MIDI para el LED

    struct OscillatorControls
    {
        juce::Label title;
        juce::ComboBox wavetableBox;
        juce::Slider positionSlider, octaveSlider, coarseSlider, fineSlider, levelSlider;
        juce::Label positionLabel, octaveLabel, coarseLabel, fineLabel, levelLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> wavetableAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            positionAttachment, octaveAttachment, coarseAttachment, fineAttachment, levelAttachment;
    };

    struct FilterControls
    {
        juce::Label title;
        juce::ComboBox typeBox;
        juce::Slider cutoffSlider, resonanceSlider, keyTrackSlider, envAmountSlider;
        juce::Label cutoffLabel, resonanceLabel, keyTrackLabel, envAmountLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> typeAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            cutoffAttachment, resonanceAttachment, keyTrackAttachment, envAmountAttachment;
    };

    struct EnvelopeControls
    {
        juce::Label title;
        std::unique_ptr<EnvelopeVisualizer> visualizer;
        juce::Slider attackSlider, decaySlider, sustainSlider, releaseSlider;
        juce::Label attackLabel, decayLabel, sustainLabel, releaseLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            attackAttachment, decayAttachment, sustainAttachment, releaseAttachment;
    };

    struct LFOControls
    {
        juce::Label title;
        juce::ComboBox waveformBox;
        juce::Slider rateSlider, phaseSlider, depthSlider;
        juce::Label rateLabel, phaseLabel, depthLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveformAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            rateAttachment, phaseAttachment, depthAttachment;
    };

    struct ModSlotControls
    {
        juce::ComboBox sourceBox, destinationBox;
        juce::Slider amountSlider;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> sourceAttachment, destinationAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> amountAttachment;
    };

    struct EffectControls
    {
        juce::Label title;
        juce::ToggleButton enableButton;
        juce::Slider knob1Slider, knob2Slider, knob3Slider;
        juce::Label knob1Label, knob2Label, knob3Label;

        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> enableAttachment;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            knob1Attachment, knob2Attachment, knob3Attachment;
    };

    struct VintageControls
    {
        juce::Label title;
        juce::Slider bitDepthSlider, sampleRateReduceSlider, noiseSlider, driftSlider,
                     filterVariationSlider, voiceVariationSlider, aliasingSlider;
        juce::Label bitDepthLabel, sampleRateReduceLabel, noiseLabel, driftLabel,
                    filterVariationLabel, voiceVariationLabel, aliasingLabel;

        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>
            bitDepthAttachment, sampleRateReduceAttachment, noiseAttachment, driftAttachment,
            filterVariationAttachment, voiceVariationAttachment, aliasingAttachment;
    };

    void setupOscillatorControls (OscillatorControls& controls, int oscNumber);
    void setupFilterControls();
    void setupEnvelopeControls (EnvelopeControls& controls, int envNumber, const juce::String& titleText);
    void setupLFOControls (LFOControls& controls, int lfoNumber);
    void setupModSlotControls (ModSlotControls& controls, int slotNumber);
    void setupEffectControls (EffectControls& controls, const juce::String& titleText,
                               const juce::String& enableParamID,
                               const juce::String& knob1ParamID, const juce::String& knob1Text,
                               const juce::String& knob2ParamID, const juce::String& knob2Text,
                               const juce::String& knob3ParamID, const juce::String& knob3Text);
    void setupVintageControls();
    void setupPresetBar();

    // Cada layout* registra el rectángulo del panel en panelSections antes
    // de subdividirlo para sus propios controles, para que paint() pueda
    // dibujar el fondo/borde del "módulo" detrás de ellos.
    void registerPanel (juce::Rectangle<int> bounds);

    void layoutOscillatorControls (OscillatorControls& controls, juce::Rectangle<int> area);
    void layoutFilterControls (juce::Rectangle<int> area);
    void layoutEnvelopeControls (EnvelopeControls& controls, juce::Rectangle<int> area);
    void layoutLFOControls (LFOControls& controls, juce::Rectangle<int> area);
    void layoutModSlotControls (ModSlotControls& controls, juce::Rectangle<int> area);
    void layoutEffectControls (EffectControls& controls, juce::Rectangle<int> area);
    void layoutVintageControls (juce::Rectangle<int> area);
    void layoutPresetBar (juce::Rectangle<int> area);

    void refreshPresetComboBox();
    void updateFavoriteButtonState();
    void onSaveButtonClicked();

    PPGLookAndFeel lookAndFeel;

    PPGWaveCloneAudioProcessor& processorRef;

    std::vector<juce::Rectangle<int>> panelSections;

    juce::Label titleLabel;
    StatusLED midiLed;
    juce::Label midiLedLabel;

    juce::Slider masterVolumeSlider;
    juce::Label  masterVolumeLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> masterVolumeAttachment;

    juce::Slider vintageAmountSlider;
    juce::Label  vintageAmountLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vintageAmountAttachment;
    VintageControls vintageControls;

    OscillatorControls osc1Controls, osc2Controls;
    FilterControls filterControls;
    EnvelopeControls env1Controls, env2Controls;
    LFOControls lfo1Controls, lfo2Controls;
    juce::Label modMatrixTitle;
    std::array<ModSlotControls, 4> modSlotControls;
    EffectControls driveControls, chorusControls, delayControls, reverbControls;

    juce::ComboBox presetComboBox;
    juce::TextButton presetPrevButton { "<" }, presetNextButton { ">" }, presetRandomButton { "Random" },
                      presetFavoriteButton { "Fav" }, presetSaveButton { "Save" }, presetDeleteButton { "Delete" };
    std::unique_ptr<juce::AlertWindow> saveNameWindow;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PPGWaveCloneAudioProcessorEditor)
};
