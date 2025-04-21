// PeriodicAction.tpp (Illustrative changes)

#include "PeriodicAction.h"
#include <Arduino.h> // For Serial prints if needed

template <typename ActionType>
PeriodicAction<ActionType>::PeriodicAction(uint32_t actionPeriodMillis,
                                           uint32_t actionDurationMillis)
    : actionPeriodMillis(actionPeriodMillis),
      actionDurationMillis(actionDurationMillis),
      actionInstance(){}

template <typename ActionType>
void PeriodicAction<ActionType>::start() {
    if (taskHandle == nullptr) {
        // Create the task
        xTaskCreate(
            taskFunction,           // Task function
            "PeriodicActionTask",   // Name of the task
            2048,                   // Stack size (adjust as needed)
            this,                   // Parameter passed to the task
            1,                      // Priority (adjust as needed)
            &taskHandle             // Task handle
        );
        if (taskHandle == nullptr) {
             Serial.println("Failed to create PeriodicAction task!");
             // Handle error appropriately
        }
    }
}

template <typename ActionType>
void PeriodicAction<ActionType>::stop() {
    if (taskHandle != nullptr) {
        vTaskDelete(taskHandle);
        taskHandle = nullptr;
    }
}

template <typename ActionType>
void PeriodicAction<ActionType>::taskFunction(void* parameters) {
    PeriodicAction<ActionType>* params = static_cast<PeriodicAction<ActionType>*>(parameters);

    // Convert period from milliseconds to ticks
    const TickType_t periodTicks = pdMS_TO_TICKS(params->actionPeriodMillis);
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime

    while (true) {
        Serial.println("Period starting...");
        params->actionInstance.performAction(params->actionDurationMillis);
        vTaskDelayUntil(&lastWakeTime, periodTicks);
        Serial.println("Period ended.");
    }
}

