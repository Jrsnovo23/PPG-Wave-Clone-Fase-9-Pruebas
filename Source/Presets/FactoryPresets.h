#pragma once

#include <juce_core/juce_core.h>
#include <vector>
#include <utility>

namespace FactoryPresets
{
    struct Definition
    {
        juce::String name;
        juce::String category;
        std::vector<std::pair<juce::String, float>> parameterOverrides;
    };

    /**
        Biblioteca inicial: un preset real y distinto por cada una de las 12
        categorías del documento de diseño (sección 15). Cada uno solo
        especifica los parámetros que se apartan de su valor por defecto —
        PresetManager parte del estado por defecto y aplica estos overrides.

        NO son los 128 presets pedidos en el documento original. Ampliar la
        biblioteca es trabajo de contenido (agregar más funciones como las
        de FactoryPresets.cpp), no un cambio de arquitectura — ver README,
        "Problemas conocidos" de la Fase 6.
    */
    const std::vector<Definition>& getDefinitions();
}
