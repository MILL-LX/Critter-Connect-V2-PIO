#include "SoundButtonAction.h"

void SoundButtonAction::taskFunction(void *action)
{
    // Cast the action pointer to the correct type
    SoundButtonAction *_this = static_cast<SoundButtonAction *>(action);

    _this->setShouldRun(true);
    _this->setActive(true);
    while (_this->shouldRun())
    {
        // Wait for the sound button to be pressed
        if (_this->_soundButton.isPressed())
        {
            // If pressed, play the sound
            _this->_soundPlayer.playSound(_this->_sound);
            while (_this->_soundPlayer.isPlaying())
            {
                vTaskDelay(pdMS_TO_TICKS(100));
            }
        }
    }
    Serial.println("SoundButtonAction task stopping...");
    _this->setActive(false);
    _this->_taskHandle == nullptr;
    vTaskDelay(pdMS_TO_TICKS(100));
    vTaskDelete(nullptr);
}

void SoundButtonAction::start()
{
    if (_taskHandle == nullptr)
    {
        // Create the task
        BaseType_t result = xTaskCreate(
            taskFunction,         // Task function
            "SoundButtonTask", // Name of the task
            1024,                 // Stack size (adjust as needed)
            this,                 // Parameter passed to the task
            1,                    // Priority (adjust as needed)
            &_taskHandle          // Task handle
        );
        if (result != pdPASS)
        {
            _taskHandle = nullptr; // Ensure handle is null on failure
            setActive(false);      // Reset flag on failure
            Serial.println("Failed to create SoundButtonAction task!");
        }
    }
    else
    {
        Serial.println("SoundButtonAction task already started.");
    }
}

void SoundButtonAction::stop()
{
    setShouldRun(false);
}

bool SoundButtonAction::isActive()
{
    return _isActive.load();
}

void SoundButtonAction::setActive(bool value)
{
    _isActive.store(value);
}

void SoundButtonAction::setShouldRun(bool value)
{
    _shouldRun.store(value);
}
bool SoundButtonAction::shouldRun()
{
    return _shouldRun.load();
}