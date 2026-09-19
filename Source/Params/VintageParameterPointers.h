#pragma once

#include <atomic>

struct VintageParameterPointers
{
    std::atomic<float>* amount           = nullptr; // control global Vintage/Modern
    std::atomic<float>* bitDepth         = nullptr;
    std::atomic<float>* sampleRateReduce = nullptr;
    std::atomic<float>* noise            = nullptr;
    std::atomic<float>* drift            = nullptr;
    std::atomic<float>* filterVariation  = nullptr;
    std::atomic<float>* voiceVariation   = nullptr;
    std::atomic<float>* aliasing         = nullptr;
};
