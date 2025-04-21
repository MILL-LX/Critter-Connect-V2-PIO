#include <Arduino.h>
#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"

void setup()
{
  Serial.begin(115200);
  delay(3000); 
}

void loop()
{
  PeriodicAction<NeoPixelAction> periodicNeoPixelAction(5000UL, 1000UL);

  Serial.println("Starting periodic actions...");

  periodicNeoPixelAction.start();

  Serial.println("Periodic actions started.");

  const unsigned long stopAfterDuration = 300000UL; // 5 minutes in milliseconds
  unsigned long programStartTime = millis();;
  bool actionsStopped = false;
  while (true)
  {
    if (!actionsStopped && (millis() - programStartTime >= stopAfterDuration))
    {
      Serial.println("Five minutes have elapsed. Stopping periodic actions...");
      periodicNeoPixelAction.stop();
      Serial.println("Periodic actions stopped.");
      actionsStopped = true;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}
