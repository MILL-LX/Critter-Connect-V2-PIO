
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "StartupTest.h"
#include "Application.h"

Application app;

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
  // This runs forever
  app.run();
}
