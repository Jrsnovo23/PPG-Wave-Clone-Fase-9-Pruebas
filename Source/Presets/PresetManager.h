#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>

struct PresetInfo
{
    juce::String name;
    juce::String category;
    juce::String author;
    bool isFactory = false;
};

/**
    Gestiona la biblioteca de presets: fábrica (horneados en el binario vía
    FactoryPresets.h, protegidos contra modificación) y de usuario (guardados
    en disco como XML, en el directorio de datos de aplicación del sistema).

    Un "preset" es una foto completa del estado del APVTS — reutiliza
    exactamente el mismo mecanismo que getStateInformation/setStateInformation
    del plugin, así que cargar un preset (fábrica o usuario) es indistinguible
    para el host de una automatización manual de todos los parámetros a la
    vez.
*/
class PresetManager
{
public:
    explicit PresetManager (juce::AudioProcessorValueTreeState& apvtsIn);

    static const juce::StringArray& getCategoryNames();

    int getNumPresets() const noexcept { return (int) presets.size(); }
    const PresetInfo& getPresetInfo (int index) const;
    int getCurrentIndex() const noexcept { return currentIndex; }

    void loadPreset (int index);
    void loadNext();
    void loadPrevious();
    void loadRandom();

    /** Asigna un valor aleatorio a cada parámetro del plugin (dentro de su
        propio rango), excepto Master Volume -- para no pegar un salto de
        volumen inesperado. A diferencia de loadRandom() (que carga uno de
        los presets guardados), esto genera una variación nueva que no
        necesariamente corresponde a ningún preset existente. */
    void randomizeParameters();

    /** false si el nombre está vacío, o ya existe un preset de usuario con
        ese nombre exacto. */
    bool saveUserPreset (const juce::String& name, const juce::String& category);

    /** Solo borra el preset actual si es de usuario; no hace nada si es de
        fábrica (protección contra modificación accidental). */
    bool deleteCurrentPresetIfUser();

    bool isFavorite (const juce::String& presetName) const;
    void toggleFavorite (const juce::String& presetName);

    /** Índices de presets cuyo nombre contiene searchText (insensible a
        mayúsculas) y cuya categoría coincide con categoryFilter ("" o
        "All" = todas). Pensado para el browser de la Fase 7; el editor de
        esta fase no lo usa todavía (ver README). */
    std::vector<int> findMatching (const juce::String& searchText, const juce::String& categoryFilter) const;

private:
    juce::AudioProcessorValueTreeState& apvts;

    std::vector<PresetInfo> presets;
    std::vector<juce::ValueTree> presetStates; // paralelo a presets
    int currentIndex = 0;

    juce::StringArray favoriteNames;
    juce::Random random;

    juce::File getUserPresetsDirectory() const;
    juce::File getFavoritesFile() const;

    void loadFactoryPresets();
    void loadUserPresetsFromDisk();
    void loadFavoritesFromDisk();
    void saveFavoritesToDisk();

    static juce::String sanitiseFileName (const juce::String& name);

    JUCE_DECLARE_NON_COPYABLE (PresetManager)
};
