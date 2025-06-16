#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "MotorAction.h"

void MotorAction::performAction()
{
    Serial.printf("Performing MotorAction for %u iterations.\n", _iterations);

    Motor::Direction direction = Motor::Direction::FORWARD;
    for (uint8_t i = 0; i < _iterations; ++i)
    {
        Serial.println("Performing motor action iteration: " + String(i + 1));
        _motor.moveFullRangeInDirection(Motor::Direction::FORWARD);
        _motor.moveFullRangeInDirection(Motor::Direction::REVERSE);
    }

    Serial.println("Finished performing MotorAction.");
}
