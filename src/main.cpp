
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "StartupTest.h"
#include "ApplicationStateManager.h"

ApplicationStateManager app;

void setup()
{
  delay(2000); // let USB mount
  Serial.begin(115200);
  delay(2000); // let Serial Stabilize

  Serial.println("Setup complete.");
}

void loop()
{
  startupTest();

  // This runs forever
  app.start();
}
