#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "MotorAction.h"

void MotorAction::performAction()
{
    Serial.printf("Performing MotorAction for %u iterations.\n", _iterations);

    Motor::Direction direction = Motor::Direction::FORWARD;
    if (isActive())
    {
        Serial.println("Motor is already active. Skipping action.");
        return;
    }
    setActive(true);
    Serial.println("MotorAction is now active.");
    for (uint8_t i = 0; i < _iterations; ++i)
    {
        Serial.println("Performing motor action iteration: " + String(i + 1));
        _motor.moveFullRangeInDirection(Motor::Direction::FORWARD);
        _motor.moveFullRangeInDirection(Motor::Direction::REVERSE);
    }
    setActive(false);
    Serial.println("MotorAction is no longer active.");
    Serial.printf("MotorAction completed after %u iterations.\n", _iterations);
}

bool MotorAction::isActive()
{
    return _isActive.load();
}

void MotorAction::setActive(bool value)
{
    _isActive.store(value);
}