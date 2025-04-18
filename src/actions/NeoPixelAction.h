#pragma once

#include <cstdint>

#include "devices/NeoPixel.h"

class NeoPixelAction
{
public:
    void performAction(uint8_t actionDurationSeconds);

private:
    NeoPixel _neoPixel;
};
