#pragma once

#include <cstdint>

#include "devices/ApplicationDevices.h"

class MotorAction
{
public:
    MotorAction(uint32_t durationMillis) : _durationMillis(durationMillis), _motor(ApplicationDevices::getInstance().getMotor()) {}

    void performAction();

private:
    uint32_t _durationMillis;
    Motor &_motor;
};
