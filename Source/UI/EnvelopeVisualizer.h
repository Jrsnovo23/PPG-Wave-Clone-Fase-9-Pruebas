#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_audio_processors/juce_audio_processors.h>

/**
    Dibuja la forma ADSR (Attack/Decay/Sustain/Release) de una envolvente,
    leyendo en vivo sus 4 parámetros del APVTS — pedido en la sección 18
    del documento de diseño ("Visualización gráfica ADSR").

    El ancho de los segmentos Attack/Decay/Release es proporcional a una
    escala logarítmica de su tiempo real (no lineal, para que 5ms y 5s
    quepan ambos de forma legible en el mismo gráfico) — así que sí se ve
    el cambio al mover esos knobs, no solo al mover Sustain. El tramo de
    Sustain no tiene "tiempo" propio (solo nivel), así que su ancho visual
    es fijo; su altura sí refleja el parámetro real.
*/
class EnvelopeVisualizer : public juce::Component,
                            private juce::Timer
{
public:
    EnvelopeVisualizer (juce::AudioProcessorValueTreeState& apvtsIn,
                         juce::String attackParamID, juce::String decayParamID,
                         juce::String sustainParamID, juce::String releaseParamID);
    ~EnvelopeVisualizer() override;

    void paint (juce::Graphics&) override;

private:
    void timerCallback() override;

    juce::AudioProcessorValueTreeState& apvts;
    const juce::String attackID, decayID, sustainID, releaseID;

    float lastAttack = -1.0f, lastDecay = -1.0f, lastSustain = -1.0f, lastRelease = -1.0f;
};
