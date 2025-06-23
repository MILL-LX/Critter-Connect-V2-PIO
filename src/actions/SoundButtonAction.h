#pragma once

#include <atomic>

#include "devices/ApplicationDevices.h"

class SoundButtonAction
{
public:
    SoundButtonAction(SoundPlayer::Sound sound) : _sound(sound) {}

    void start();
    void stop();
    bool isActive();

private:
    std::atomic<bool> _shouldRun{false};
    std::atomic<bool> _isActive{false};

    SoundPlayer::Sound _sound;
    SoundPlayer &_soundPlayer = ApplicationDevices::getInstance().getSoundPlayer();
    SoundButton &_soundButton = ApplicationDevices::getInstance().getSoundButton();

    void setShouldRun(bool value);
    bool shouldRun();
    void setActive(bool value);

    static void taskFunction(void *parameters);
    TaskHandle_t _taskHandle = nullptr;
};
