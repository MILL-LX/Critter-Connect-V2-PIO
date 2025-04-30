#pragma once

#include <cstdint>

#include "devices/Motor.h"

class MotorAction
{
public:
    MotorAction(uint32_t durationMillis) : _durationMillis(durationMillis) {}
    void performAction();

private:
    uint32_t _durationMillis;
    Motor _motor;
};
