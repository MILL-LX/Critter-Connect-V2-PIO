#pragma once

#include "FreeRTOS.h"   // Ensure FreeRTOS types like TickType_t are available
#include "task.h"       // Ensure task-related functions like vTaskDelete are available
#include <cstdint>      // For uint8_t and other fixed-width types

template <typename ActionType>
class PeriodicAction {
public:
    PeriodicAction(uint8_t actionPeriodSeconds,
                   uint8_t actionDurationSeconds);

    void start();
    void stop();

private:
    static void taskFunction(void* parameters);

    TaskHandle_t taskHandle = nullptr;
    uint8_t actionPeriodSeconds;
    uint8_t actionDurationSeconds;
    ActionType actionInstance;
};

// Include the implementation of the template class
#include "PeriodicAction.tpp"
