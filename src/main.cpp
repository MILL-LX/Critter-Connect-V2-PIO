#include <Arduino.h>
#include "PeriodicAction.h"
#include "PrintAction.h"

PrintAction printAction;
PeriodicAction<PrintAction> periodic(2, 10, &printAction);  // active 2s every 10s

void setup() {
    Serial.begin(115200);

    // Wait for the Serial connection to be established
    while (!Serial) {
        delay(100);  // Wait a bit for Serial to initialize, if necessary
    }

    // Delay for 1 second after Serial.begin()
    delay(1000);

    periodic.start();  // Start the periodic action
}

void loop() {
    // Main loop remains free
}
