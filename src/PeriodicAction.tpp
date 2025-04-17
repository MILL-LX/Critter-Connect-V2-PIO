#include "PeriodicAction.h"

template <typename ActionType>
PeriodicAction<ActionType>::PeriodicAction(
    uint8_t actionPeriodSeconds,
    uint8_t actionDurationSeconds)
    : actionDurationSeconds(actionDurationSeconds),
    actionPeriodSeconds(actionPeriodSeconds) {}

template <typename ActionType>
void PeriodicAction<ActionType>::start() {
    if (taskHandle == nullptr) {
        xTaskCreate(
            taskFunction,
            "PeriodicActionTask",
            1024,  // Stack size in words
            this,
            1,     // Task priority
            &taskHandle
        );
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
    auto* instance = static_cast<PeriodicAction<ActionType>*>(parameters);


    bool actionPerformed = false;
    TickType_t startTick = xTaskGetTickCount();
    while (true) {
        TickType_t nowTick = xTaskGetTickCount();
        TickType_t elapsedTicks = nowTick - startTick;
        uint32_t elapsedSeconds = elapsedTicks / configTICK_RATE_HZ;
        uint32_t secondsInCurrentPeriod = elapsedSeconds % instance->actionPeriodSeconds;

        if (secondsInCurrentPeriod == 0 && !actionPerformed) {
            instance->actionInstance.performAction(instance->actionDurationSeconds);
            actionPerformed = true;
        }
    }
}
