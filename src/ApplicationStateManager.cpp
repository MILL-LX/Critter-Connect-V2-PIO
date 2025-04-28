#include <Arduino.h>

#include "ApplicationStateManager.h"

void ApplicationStateManager::start() {
    if(_is_running) {
        Serial.println("Application already running, ignoring start().");
        return;
    }


}