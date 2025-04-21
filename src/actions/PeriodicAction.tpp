#include "PeriodicAction.h"
#include <Arduino.h> // For Serial prints if needed
#include <utility> // For std::forward

template <typename ActionType>
template <typename... ActionArgs>
PeriodicAction<ActionType>::PeriodicAction(uint32_t actionPeriodMillis,
                                           uint32_t actionDurationMillis, ActionArgs &&...args)
    : _actionPeriodMillis(actionPeriodMillis),
      _actionDurationMillis(actionDurationMillis),
      _actionInstance(std::forward<ActionArgs>(args)...)
      {}


template <typename ActionType>
void PeriodicAction<ActionType>::start() {
    if (_taskHandle == nullptr) {
        _continueAction.store(true); // Atomic store
        // Create the task
        BaseType_t result = xTaskCreate(
            taskFunction,           // Task function
            "PeriodicActionTask",   // Name of the task
            2048,                   // Stack size (adjust as needed)
            this,                   // Parameter passed to the task
            1,                      // Priority (adjust as needed)
            &_taskHandle             // Task handle
        );
        if (result != pdPASS) {
             _taskHandle = nullptr; // Ensure handle is null on failure
             _continueAction.store(false); // Reset flag on failure
             Serial.println("Failed to create PeriodicAction task!");
        }
    } else {
         Serial.println("PeriodicAction task already started.");
    }
}

template <typename ActionType>
void PeriodicAction<ActionType>::stop() {
    _continueAction.store(false);
}

template <typename ActionType>
void PeriodicAction<ActionType>::taskFunction(void* parameters) {
    PeriodicAction<ActionType>* periodicAction = static_cast<PeriodicAction<ActionType>*>(parameters);

    // Convert period from milliseconds to ticks
    const TickType_t periodTicks = pdMS_TO_TICKS(periodicAction->_actionPeriodMillis);
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime

    periodicAction->_continueAction.store(true);
    while (periodicAction->_continueAction.load()) {
        TickType_t periodStartTime = xTaskGetTickCount();
        Serial.println("Period of " + String(periodicAction->_actionPeriodMillis) + " millis starting...");
        periodicAction->_actionInstance.performAction(periodicAction->_actionDurationMillis);

        // Delay until the next period
        vTaskDelayUntil(&lastWakeTime, periodTicks);
        Serial.println("Period ended after " + String(pdTICKS_TO_MS(xTaskGetTickCount()-periodStartTime)) + " millis.");
    }

    Serial.println("PeriodicAction Task self-deleting.");
    periodicAction->_taskHandle = nullptr;
    vTaskDelete(nullptr);
}

