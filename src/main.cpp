#include <Arduino.h>
#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"
#include "actions/MotorAction.h"

void setup()
{
  Serial.begin(115200);
  delay(3000); 
}

void loop()
{
  PeriodicAction<NeoPixelAction> periodicNeoPixelAction(1000UL, 10 /* UINT32_MAX */, 1000UL, NeoPixel::StateColor::OK);
  PeriodicAction<MotorAction> periodicMotorAction(1000UL, 10, 1000UL);

  Serial.println("Starting periodic actions...");

  periodicNeoPixelAction.start();
  periodicMotorAction.start();

  Serial.println("Periodic actions started.\n\n");

  while (true)
    vTaskDelay(pdMS_TO_TICKS(10));
}
