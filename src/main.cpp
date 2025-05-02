
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "app/StartupTest.h"

#include "actions/GPSReceiverAction.h"

void setup()
{
  delay(2000); // let USB mount
  Serial.begin(115200);
  delay(2000); // let Serial Stabilize

  Serial.println("Setup complete. Running startup tests...");
  startupTest();
}

void loop()
{
  // Every 10 seconds check for a location update for 1 second
  // PeriodicAction<GPSReceiverAction> periodicGPSReceiverAction(10000UL, UINT32_MAX);
  // periodicGPSReceiverAction.start();

  while(true)
  {
    // Serial.println("Main thread is just waiting...");
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}
