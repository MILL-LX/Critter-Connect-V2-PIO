#pragma once

#include <atomic>
#include <cstdint>

#include "devices/ApplicationDevices.h"

class NeoPixelAction
{
public:
    // Initialize the reference _neoPixel in the initializer list
    NeoPixelAction(uint32_t durationMillis, NeoPixel::StateColor on_color)
        : _durationMillis(durationMillis),
          _on_color(on_color),
          _neoPixel(ApplicationDevices::getInstance().getNeoPixel()) {}

    void performAction();
    bool isActive();

private:
    uint32_t _durationMillis;
    NeoPixel::StateColor _on_color;
    NeoPixel &_neoPixel; // Reference member
    std::atomic<bool> _isActive{false};

    void setActive(bool value);
};
