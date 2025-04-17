#include "PeriodicAction.h"

template <typename ActionType>
PeriodicAction<ActionType>::PeriodicAction(
    uint8_t activeDurationSeconds,
    uint8_t periodIntervalSeconds,
    ActionType* actionInstance)
    : activeDurationSeconds(activeDurationSeconds),
      periodIntervalSeconds(periodIntervalSeconds),
      actionInstance(actionInstance) {}

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
    TickType_t startTick = xTaskGetTickCount();

    while (true) {
        TickType_t nowTick = xTaskGetTickCount();
        TickType_t elapsedTicks = nowTick - startTick;

        uint32_t elapsedSeconds = elapsedTicks / configTICK_RATE_HZ;
        uint32_t secondsInCurrentPeriod = elapsedSeconds % instance->periodIntervalSeconds;

        if (secondsInCurrentPeriod < instance->activeDurationSeconds) {
            instance->actionInstance->performAction();
            vTaskDelay(pdMS_TO_TICKS(500));  // 500ms delay between actions
        } else {
            vTaskDelay(pdMS_TO_TICKS(100));  // Idle poll every 100ms
        }
    }
}
