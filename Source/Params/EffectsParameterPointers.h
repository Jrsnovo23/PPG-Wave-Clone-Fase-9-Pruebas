#pragma once

#include <atomic>

struct EffectsParameterPointers
{
    std::atomic<float>* driveEnabled = nullptr;
    std::atomic<float>* driveAmount  = nullptr;
    std::atomic<float>* driveTone    = nullptr;
    std::atomic<float>* driveMix     = nullptr;

    std::atomic<float>* chorusEnabled = nullptr;
    std::atomic<float>* chorusRate    = nullptr;
    std::atomic<float>* chorusDepth   = nullptr;
    std::atomic<float>* chorusMix     = nullptr;

    std::atomic<float>* delayEnabled      = nullptr;
    std::atomic<float>* delaySynced       = nullptr;
    std::atomic<float>* delaySyncDivision = nullptr;
    std::atomic<float>* delayTime         = nullptr;
    std::atomic<float>* delayFeedback     = nullptr;
    std::atomic<float>* delayMix          = nullptr;

    std::atomic<float>* reverbEnabled = nullptr;
    std::atomic<float>* reverbSize    = nullptr;
    std::atomic<float>* reverbDamping = nullptr;
    std::atomic<float>* reverbMix     = nullptr;
};
