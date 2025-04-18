#include <Arduino.h>
#include "actions/PeriodicAction.h"
#include "actions/PrintAction.h"
#include "actions/NeoPixelAction.h"


PeriodicAction<NeoPixelAction> periodicNeoPixelAction(10, 1);
PeriodicAction<PrintAction> periodicPrintAction(10, 2);

unsigned long programStartTime = 0;
bool actionsStopped = false;
const unsigned long stopAfterDuration = 60 * 1000UL; // 60 seconds in milliseconds

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    delay(100); // Wait a bit for Serial to initialize
  delay(1000); // Extra delay to allow serial monitor connection

  Serial.println("Starting periodic actions...");
  periodicNeoPixelAction.start();
  periodicPrintAction.start();
  Serial.println("Periodic actions started.");

  programStartTime = millis(); // Record the time when setup finishes and loop starts
  while(true)
  {
    if (!actionsStopped && (millis() - programStartTime >= stopAfterDuration))
    {
      Serial.println("One minute elapsed. Stopping periodic actions...");
      periodicNeoPixelAction.stop();
      periodicPrintAction.stop();
      Serial.println("Periodic actions stopped.");
      actionsStopped = true;   // Set the flag so we don't try to stop it again
    }

    delay(10);
  }
}

void loop()
{
}
