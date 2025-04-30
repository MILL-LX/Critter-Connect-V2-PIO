#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "MotorAction.h"

void MotorAction::performAction()
{
    TickType_t durationTicks = pdMS_TO_TICKS(_durationMillis);
    TickType_t startTick = xTaskGetTickCount();
    TickType_t endTick = startTick + durationTicks;

    Serial.printf("Performing MotorAction for %ums.\n", _durationMillis);
    
    Motor::Direction direction = Motor::Direction::FORWARD;
    while (xTaskGetTickCount() < endTick)
    {
        _motor.moveFullRangeInDirection(direction);
        direction = direction == Motor::Direction::FORWARD ? Motor::Direction::REVERSE : Motor::Direction::FORWARD;
    }

    Serial.println("Finished performing MotorAction.");
}
