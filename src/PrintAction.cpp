#include "PrintAction.h"
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

void PrintAction::performAction(uint8_t actionDurationSeconds) {
    TickType_t startTick = xTaskGetTickCount();
    TickType_t durationTicks = pdMS_TO_TICKS(actionDurationSeconds * 1000UL);
    TickType_t endTick = startTick + durationTicks;

    const TickType_t delayIntervalTicks = pdMS_TO_TICKS(500);

    Serial.println("Performing PrintAction for " + String(actionDurationSeconds) + " seconds.");

    int counter = 0;
    while (xTaskGetTickCount() < endTick) {
        Serial.print("print: "); Serial.println(counter);
        counter++;

        TickType_t currentTime = xTaskGetTickCount();
        if ((endTick - currentTime) < delayIntervalTicks) {
             vTaskDelay(endTick - currentTime);
             break;
        } else {
            vTaskDelay(delayIntervalTicks);
        }
    }

    Serial.println("Finished performing PrintAction.");
}
