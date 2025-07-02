#include "PeriodicAction.h"
#include <Arduino.h> // For Serial prints if needed
#include <utility>   // For std::forward

template <typename ActionType>
template <typename... ActionArgs>
PeriodicAction<ActionType>::PeriodicAction(uint32_t actionPeriodMillis,
                                           uint32_t actionPeriodIterations,
                                           ActionArgs &&...args)
    : _actionPeriodMillis(actionPeriodMillis),
      _actionPeriodIterations(actionPeriodIterations),
      _actionInstance(std::forward<ActionArgs>(args)...)
{
}

template <typename ActionType>
void PeriodicAction<ActionType>::start()
{
    if (_taskHandle != nullptr)
    {
        stop();
    }

    if (_taskHandle == nullptr)
    {
        _continueAction.store(true); // Atomic store
        // Create the task
        BaseType_t result = xTaskCreate(
            taskFunction,         // Task function
            "PeriodicActionTask", // Name of the task
            1024,                 // Stack size (adjust as needed)
            this,                 // Parameter passed to the task
            1,                    // Priority (adjust as needed)
            &_taskHandle          // Task handle
        );
        if (result != pdPASS)
        {
            _taskHandle = nullptr;        // Ensure handle is null on failure
            _continueAction.store(false); // Reset flag on failure
            Serial.println("Failed to create PeriodicAction task!");
        }
        else
        {
            while (!isActive())
            {
                vTaskDelay(pdMS_TO_TICKS(100)); // Wait for the task to start
            }
            Serial.println("PeriodicAction task started successfully.");
        }
    }
    else
    {
        Serial.printf("PeriodicAction task already started. %x is running.\n", _taskHandle);
    }
}

template <typename ActionType>
void PeriodicAction<ActionType>::stop()
{
    _continueAction.store(false);
    vTaskDelay(pdMS_TO_TICKS(500));
    Serial.println("Explicitly stopping PeriodicAction task...");
    if (_taskHandle != nullptr)
    {
        vTaskDelete(_taskHandle);
        _taskHandle = nullptr;
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

template <typename ActionType>
void PeriodicAction<ActionType>::taskFunction(void *parameters)
{
    PeriodicAction<ActionType> *periodicAction = static_cast<PeriodicAction<ActionType> *>(parameters);

    // Convert period from milliseconds to ticks
    const TickType_t periodTicks = pdMS_TO_TICKS(periodicAction->_actionPeriodMillis);
    TickType_t lastWakeTime = xTaskGetTickCount(); // Initialize lastWakeTime

    uint32_t iterationsLeft = periodicAction->_actionPeriodIterations;
    bool runUntilExplicitlyStopped = iterationsLeft == UINT32_MAX;
    periodicAction->_continueAction.store(true);
    while ((runUntilExplicitlyStopped || iterationsLeft) && periodicAction->_continueAction.load())
    {
        // String message = "PeriodicAction with period " + String(periodicAction->_actionPeriodMillis) + " millis running for " + (runUntilExplicitlyStopped ? "unlimited" : String(iterationsLeft)) + " more iterations.";
        // Serial.println(message);

        TickType_t periodStartTime = xTaskGetTickCount();

        periodicAction->_actionInstance.performAction();
        vTaskDelayUntil(&lastWakeTime, periodTicks);

        // Serial.println("PeriodAction ended after " + String(pdTICKS_TO_MS(xTaskGetTickCount() - periodStartTime)) + " millis.\n");

        iterationsLeft--;
    }

    periodicAction->_continueAction.store(false); // Ensure the action is marked as stopped

    Serial.printf("PeriodicAction Task self-deleting %x...\n", periodicAction->_taskHandle);
    periodicAction->_taskHandle = nullptr;
    vTaskDelay(pdMS_TO_TICKS(1000));
    vTaskDelete(nullptr);
}

template <typename ActionType>
bool PeriodicAction<ActionType>::isActive()
{
    return _actionInstance.isActive() || _continueAction.load();
}