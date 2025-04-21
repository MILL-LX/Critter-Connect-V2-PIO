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

    bool _continueAction = false;
    TaskHandle_t _taskHandle = nullptr;
    uint32_t _actionPeriodMillis;
    uint32_t _actionDurationMillis;
    ActionType _actionInstance;
};

// Include the implementation of the template class
#include "PeriodicAction.tpp"
