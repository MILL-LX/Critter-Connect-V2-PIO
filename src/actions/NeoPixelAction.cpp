#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "NeoPixelAction.h"
#include "devices/NeoPixel.h"

void NeoPixelAction::performAction(uint8_t actionDurationSeconds)
{
    TickType_t startTick = xTaskGetTickCount();
    TickType_t durationTicks = pdMS_TO_TICKS(actionDurationSeconds * 1000UL);
    TickType_t endTick = startTick + durationTicks;

    const TickType_t delayIntervalTicks = pdMS_TO_TICKS(200);

    Serial.println("Performing NeoPixelAction for " + String(actionDurationSeconds) + " seconds.");
    bool on = true;
    while (xTaskGetTickCount() < endTick)
    {

        NeoPixel::Color color = on ? NeoPixel::Color::ON : NeoPixel::Color::OFF;
        on = !on;

        _neoPixel.setColor(color);

        TickType_t currentTime = xTaskGetTickCount();
        if ((endTick - currentTime) < delayIntervalTicks)
        {
            vTaskDelay(endTick - currentTime);
            break;
        }
        else
        {
            vTaskDelay(delayIntervalTicks);
        }
    }

    _neoPixel.setColor(NeoPixel::Color::OFF);
    Serial.println("Finished performing NeoPixelAction.");
}
