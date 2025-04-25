
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
  //PeriodicAction<NeoPixelAction> periodicNeoPixelAction(10000UL, 3 /* UINT32_MAX */, 3000UL, NeoPixel::StateColor::OK);
  PeriodicAction<MotorAction> periodicMotorAction(16000UL, 3, 5000UL);
  vTaskDelay(pdMS_TO_TICKS(2000));


  Serial.println("Starting PeriodicActions...");
  //periodicNeoPixelAction.start();
  periodicMotorAction.start();
  Serial.println("PeriodicActions started.");

  while(true) {
    // Serial.println("Just waiting around for 2 seconds...");
    vTaskDelay(pdMS_TO_TICKS(2000));
  }


  // Motor motor;

  // while (true)
  // {
  //   Serial.println("Moving FORWARD");
  //   motor.moveFullRangeInDirection(Motor::Direction::FORWARD);

  //   // Serial.println("Waiting 1 seconds");
  //   // vTaskDelay(pdMS_TO_TICKS(500));

  //   Serial.println("Moving REVERSE");
  //   motor.moveFullRangeInDirection(Motor::Direction::REVERSE);

  //   Serial.println("Waiting 2 seconds");
  //   vTaskDelay(pdMS_TO_TICKS(2000));
  // }
}
