#pragma once

#include "FreeRTOS.h"
#include "task.h"
#include <cstdint>
#include <atomic> // Include atomic header

template <typename ActionType>
class PeriodicAction
{
public:
    template <typename... ActionArgs>
    PeriodicAction(uint32_t actionPeriodMillis,
                   uint32_t actionPeriodIterations,
                   uint32_t actionDurationMillis,
                   ActionArgs &&...actionArgs);

    void start();
    void stop();

private:
    static void taskFunction(void *parameters);

    std::atomic<bool> _continueAction{false};
    TaskHandle_t _taskHandle = nullptr;
    uint32_t _actionPeriodMillis;
    uint32_t _actionPeriodIterations;
    uint32_t _actionDurationMillis;
    ActionType _actionInstance;
};

#include "PeriodicAction.tpp"