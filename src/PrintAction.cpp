#include "PrintAction.h"
#include <Arduino.h>  // Or platform-specific Serial interface

void PrintAction::performAction() {
    Serial.println("Performing periodic print action...");
}
