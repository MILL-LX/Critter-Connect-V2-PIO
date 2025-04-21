// PeriodicAction.tpp (Illustrative changes)

#include "PeriodicAction.h"
#include <Arduino.h> // For Serial prints if needed

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
        // Create the task
        xTaskCreate(
            taskFunction,           // Task function
            "PeriodicActionTask",   // Name of the task
            2048,                   // Stack size (adjust as needed)
            this,                   // Parameter passed to the task
            1,                      // Priority (adjust as needed)
            &_taskHandle             // Task handle
        );
        if (_taskHandle == nullptr) {
             Serial.println("Failed to create PeriodicAction task!");
             // Handle error appropriately
        }
    }
}

template <typename ActionType>
void PeriodicAction<ActionType>::stop() {
    _continueAction = false;
}

template <typename ActionType>
void PeriodicAction<ActionType>::taskFunction(void* parameters) {
    PeriodicAction<ActionType>* params = static_cast<PeriodicAction<ActionType>*>(parameters);

    // Convert period from milliseconds to ticks
    const TickType_t periodTicks = pdMS_TO_TICKS(params->_actionPeriodMillis);
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime

    params->_continueAction = true;
    while (params->_continueAction) {
        Serial.println("Period starting...");
        params->_actionInstance.performAction(params->_actionDurationMillis);
        vTaskDelayUntil(&lastWakeTime, periodTicks);
        Serial.println("Period ended.");
    }

    vTaskDelete(params->_taskHandle);
    params->_taskHandle = nullptr;
}
