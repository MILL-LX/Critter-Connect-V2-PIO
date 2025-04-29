#include <Arduino.h>

#include "Application.h"

void Application::start() {
    if(_is_running) {
        Serial.println("Application already running, ignoring start().");
        return;
    }


}