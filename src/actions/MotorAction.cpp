#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "MotorAction.h"

void MotorAction::performAction(uint32_t actionDurationMillis)
{
    TickType_t durationTicks = pdMS_TO_TICKS(actionDurationMillis);
    TickType_t startTick = xTaskGetTickCount();
    TickType_t endTick = startTick + durationTicks;

    Serial.println("Performing MotorAction for " + String(actionDurationMillis) + " ms.");
    
    Motor::Direction direction = Motor::Direction::FORWARD;
    while (xTaskGetTickCount() < endTick)
    {
        _motor.moveFullRangeInDirection(direction);
        direction = direction == Motor::Direction::FORWARD ? Motor::Direction::REVERSE : Motor::Direction::FORWARD;
    }

    Serial.println("Finished performing MotorAction.");
}
