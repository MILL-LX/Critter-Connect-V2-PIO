#pragma once

#include "FreeRTOS.h" // Ensure FreeRTOS types like TickType_t are available
#include "task.h"     // Ensure task-related functions like vTaskDelete are available
#include <cstdint>    // For uint32_t and other fixed-width types

template <typename ActionType>
class PeriodicAction
{
public:
    template <typename... ActionArgs>
    PeriodicAction(uint32_t actionPeriodMillis,
                   uint32_t actionDurationMillis, ActionArgs &&...actionArgs);

    void start();
    void stop();

private:
    static void taskFunction(void *parameters);

    TaskHandle_t taskHandle = nullptr;
    // Store values in milliseconds
    uint32_t actionPeriodMillis;
    uint32_t actionDurationMillis;
    ActionType actionInstance;
};

// Include the implementation of the template class
#include "PeriodicAction.tpp"
