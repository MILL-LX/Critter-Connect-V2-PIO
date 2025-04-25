#pragma once

#include <cstdint>

#include "devices/Motor.h"

class MotorAction
{
public:
    void performAction(uint32_t actionDurationMillis);

private:
    Motor _motor;
};
