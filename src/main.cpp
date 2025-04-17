#include <Arduino.h>
#include "PeriodicAction.h"
#include "PrintAction.h"

PeriodicAction<PrintAction> periodicPrintAction(10, 2);

unsigned long programStartTime = 0;
bool actionTaskStopped = false;
const unsigned long stopAfterDuration = 60 * 1000UL; // 60 seconds in milliseconds

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(100); // Wait a bit for Serial to initialize
  delay(1000); // Extra delay to allow serial monitor connection

  Serial.println("Starting periodic print action...");
  periodicPrintAction.start(); // Start the periodic action task

  programStartTime = millis(); // Record the time when setup finishes and loop starts
  while(true)
  {
    if (!actionTaskStopped && (millis() - programStartTime >= stopAfterDuration))
    {
      Serial.println("One minute elapsed. Stopping periodic print action task...");
      periodicPrintAction.stop(); // Stop the periodic action task
      actionTaskStopped = true;   // Set the flag so we don't try to stop it again
    }

    delay(10);
  }
}

void loop()
{
}
