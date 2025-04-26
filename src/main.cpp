
#include <Arduino.h>
#include "FreeRTOS.h"
#include "task.h"

#include "StartupTest.h"

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

  while(true) {
    Serial.println("Main loop waiting...");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
