
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
  //MODEBUG startupTest();
}

void loop()
{
  // The GPS Receiver Action runs indefinitely, checking out positions and updating the state.
  GPSReceiverAction gpsReceiverAction;
  gpsReceiverAction.performAction();

  // We should never get here
  Serial.println("GPSReceiverAction completed unexpectedly. This should not happen.");
  vTaskDelay(pdMS_TO_TICKS(1000)); // Wait a second before the next loop iteration
}
