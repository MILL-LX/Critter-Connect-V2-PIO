
#include <Arduino.h>
#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"
#include "actions/MotorAction.h"

void setup()
{
  delay(2000); // let USB mount
  Serial.begin(115200);
  delay(2000); // let Serial Stabilize

  Serial.println("Setup complete.");
}

void loop()
{
  Serial.println("Creating PeriodicActions...");
  PeriodicAction<NeoPixelAction> periodicNeoPixelAction(10000UL, 30 /* UINT32_MAX */, 3000UL, NeoPixel::StateColor::OK);
  PeriodicAction<MotorAction> periodicMotorAction(60000UL, 3, 10000UL);
  vTaskDelay(pdMS_TO_TICKS(2000));


  Serial.println("Starting PeriodicActions...");
  periodicNeoPixelAction.start();
  periodicMotorAction.start();
  Serial.println("PeriodicActions started.");

  while(true) {
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
