
#include <Arduino.h>
#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"
#include "actions/MotorAction.h"

Motor *motor;
void setup() {
  delay(2000); // let USB mount
  Serial.begin(115200);
  delay(2000); // let Serial Stabilize

  Serial.println("Starting Up...");

  // PeriodicAction<NeoPixelAction> periodicNeoPixelAction(1000UL, 10 /* UINT32_MAX */, 1000UL, NeoPixel::StateColor::OK);
  // PeriodicAction<MotorAction> periodicMotorAction(1000UL, 10, 1000UL);

  motor = new Motor();
}

void loop() {
  
  Serial.println("Moving FORWARD");
  motor->moveFullRangeInDirection(Motor::Direction::FORWARD);

  Serial.println("Waiting 2 seconds");
  vTaskDelay(pdMS_TO_TICKS(2000));

  Serial.println("Moving REVERSE");
  motor->moveFullRangeInDirection(Motor::Direction::REVERSE);

  Serial.println("Waiting 10 seconds");
  vTaskDelay(pdMS_TO_TICKS(10000));
}

