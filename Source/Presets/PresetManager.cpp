#include "PresetManager.h"
#include "FactoryPresets.h"
#include "../Params/ParameterIDs.h"

namespace
{
    constexpr const char* companyFolderName = "PPGWaveClone";
    constexpr const char* presetsFolderName = "Presets";
    constexpr const char* favoritesFileName = "favorites.xml";
}

const juce::StringArray& PresetManager::getCategoryNames()
{
    static const juce::StringArray names {
        "Bass", "Lead", "Pad", "Keys", "Bell", "Pluck",
        "Sequence", "FX", "Atmospheric", "Digital", "Experimental", "Percussive", "User"
    };
    return names;
}

PresetManager::PresetManager (juce::AudioProcessorValueTreeState& apvtsIn)
    : apvts (apvtsIn)
{
    loadFavoritesFromDisk();
    loadFactoryPresets();
    loadUserPresetsFromDisk();
}

juce::File PresetManager::getUserPresetsDirectory() const
{
    auto dir = juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
                   .getChildFile (companyFolderName)
                   .getChildFile (presetsFolderName);
    dir.createDirectory();
    return dir;
}

juce::File PresetManager::getFavoritesFile() const
{
    return juce::File::getSpecialLocation (juce::File::userApplicationDataDirectory)
               .getChildFile (companyFolderName)
               .getChildFile (favoritesFileName);
}

juce::String PresetManager::sanitiseFileName (const juce::String& name)
{
    return juce::File::createLegalFileName (name);
}

void PresetManager::loadFactoryPresets()
{
    // Estado por defecto real (todos los parámetros en su valor inicial),
    // capturado antes de que el host restaure ningún proyecto guardado
    // (este objeto se construye en el constructor de PluginProcessor, justo
    // después del APVTS, antes de que el host llame a setStateInformation).
    const juce::ValueTree defaultState = apvts.copyState().createCopy();

    for (auto& def : FactoryPresets::getDefinitions())
    {
        auto state = defaultState.createCopy();

        for (auto& override_ : def.parameterOverrides)
        {
            for (int i = 0; i < state.getNumChildren(); ++i)
            {
                auto child = state.getChild (i);
                if (child.getProperty ("id").toString() == override_.first)
                {
                    child.setProperty ("value", override_.second, nullptr);
                    break;
                }
            }
        }

        PresetInfo info;
        info.name = def.name;
        info.category = def.category;
        info.author = "Factory";
        info.isFactory = true;

        presets.push_back (info);
        presetStates.push_back (state);
    }
}

void PresetManager::loadUserPresetsFromDisk()
{
    auto dir = getUserPresetsDirectory();

    for (auto& file : dir.findChildFiles (juce::File::findFiles, false, "*.xml"))
    {
        std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
        if (xml == nullptr)
            continue;

        auto* stateXml = xml->getChildByName (apvts.state.getType());
        if (stateXml == nullptr)
            continue;

        PresetInfo info;
        info.name = xml->getStringAttribute ("presetName", file.getFileNameWithoutExtension());
        info.category = xml->getStringAttribute ("presetCategory", "User");
        info.author = "User";
        info.isFactory = false;

        presets.push_back (info);
        presetStates.push_back (juce::ValueTree::fromXml (*stateXml));
    }
}

void PresetManager::loadFavoritesFromDisk()
{
    auto file = getFavoritesFile();
    if (! file.existsAsFile())
        return;

    std::unique_ptr<juce::XmlElement> xml (juce::XmlDocument::parse (file));
    if (xml == nullptr)
        return;

    for (auto* child : xml->getChildIterator())
        if (child->hasTagName ("FAVORITE"))
            favoriteNames.add (child->getStringAttribute ("name"));
}

void PresetManager::saveFavoritesToDisk()
{
    juce::XmlElement root ("FAVORITES");
    for (auto& name : favoriteNames)
        root.createNewChildElement ("FAVORITE")->setAttribute ("name", name);

    auto file = getFavoritesFile();
    file.getParentDirectory().createDirectory();
    root.writeTo (file);
}

const PresetInfo& PresetManager::getPresetInfo (int index) const
{
    return presets[(size_t) juce::jlimit (0, (int) presets.size() - 1, index)];
}

void PresetManager::loadPreset (int index)
{
    if (! juce::isPositiveAndBelow (index, (int) presets.size()))
        return;

    currentIndex = index;
    apvts.replaceState (presetStates[(size_t) index].createCopy());
}

void PresetManager::loadNext()
{
    if (presets.empty()) return;
    loadPreset ((currentIndex + 1) % (int) presets.size());
}

void PresetManager::loadPrevious()
{
    if (presets.empty()) return;
    loadPreset ((currentIndex - 1 + (int) presets.size()) % (int) presets.size());
}

void PresetManager::loadRandom()
{
    if (presets.empty()) return;
    loadPreset (random.nextInt ((int) presets.size()));
}

void PresetManager::randomizeParameters()
{
    for (int i = 0; i < apvts.state.getNumChildren(); ++i)
    {
        const auto id = apvts.state.getChild (i).getProperty ("id").toString();
        if (id == ParamIDs::masterVolume)
            continue;

        if (auto* param = apvts.getParameter (id))
            param->setValueNotifyingHost (random.nextFloat());
    }
}

bool PresetManager::saveUserPreset (const juce::String& name, const juce::String& category)
{
    if (name.isEmpty())
        return false;

    for (auto& p : presets)
        if (! p.isFactory && p.name == name)
            return false;

    auto state = apvts.copyState().createCopy();

    juce::XmlElement root ("PPGWAVECLONE_PRESET");
    root.setAttribute ("presetName", name);
    root.setAttribute ("presetCategory", category);
    std::unique_ptr<juce::XmlElement> stateXml (state.createXml());
    root.addChildElement (stateXml.release());

    auto file = getUserPresetsDirectory().getChildFile (sanitiseFileName (name) + ".xml");
    if (! root.writeTo (file))
        return false;

    PresetInfo info;
    info.name = name;
    info.category = category;
    info.author = "User";
    info.isFactory = false;

    presets.push_back (info);
    presetStates.push_back (state);
    currentIndex = (int) presets.size() - 1;

    return true;
}

bool PresetManager::deleteCurrentPresetIfUser()
{
    if (! juce::isPositiveAndBelow (currentIndex, (int) presets.size()))
        return false;

    if (presets[(size_t) currentIndex].isFactory)
        return false;

    auto file = getUserPresetsDirectory().getChildFile (sanitiseFileName (presets[(size_t) currentIndex].name) + ".xml");
    file.deleteFile();

    presets.erase (presets.begin() + currentIndex);
    presetStates.erase (presetStates.begin() + currentIndex);

    if (currentIndex >= (int) presets.size())
        currentIndex = (int) presets.size() - 1;

    if (! presets.empty())
        loadPreset (juce::jmax (0, currentIndex));

    return true;
}

bool PresetManager::isFavorite (const juce::String& presetName) const
{
    return favoriteNames.contains (presetName);
}

void PresetManager::toggleFavorite (const juce::String& presetName)
{
    if (favoriteNames.contains (presetName))
        favoriteNames.removeString (presetName);
    else
        favoriteNames.add (presetName);

    saveFavoritesToDisk();
}

std::vector<int> PresetManager::findMatching (const juce::String& searchText, const juce::String& categoryFilter) const
{
    std::vector<int> result;
    for (int i = 0; i < (int) presets.size(); ++i)
    {
        const auto& p = presets[(size_t) i];
        const bool matchesCategory = categoryFilter.isEmpty() || categoryFilter == "All" || p.category == categoryFilter;
        const bool matchesSearch = searchText.isEmpty() || p.name.containsIgnoreCase (searchText);

        if (matchesCategory && matchesSearch)
            result.push_back (i);
    }
    return result;
}
