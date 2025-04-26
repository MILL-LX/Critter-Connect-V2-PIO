#include "actions/PeriodicAction.h"
#include "actions/NeoPixelAction.h"
#include "actions/MotorAction.h"

#include "StartupTest.h"

void startupTest() {
    Serial.println("Starting Startup Tests...");
  
    // Every 10 seconds, blink OK colorfor 3 seconds. Run until explicitly stopped.
    PeriodicAction<NeoPixelAction> periodicNeoPixelAction(10000UL, UINT32_MAX, 3000UL, NeoPixel::StateColor::OK);
    periodicNeoPixelAction.start();
  
    // Every 10 seconds move the motor for 10 seconds. Run until explicitly stopped.
    PeriodicAction<MotorAction> periodicMotorAction(10000UL, UINT32_MAX, 10000UL);
    periodicMotorAction.start();
  
    // Let the tests run for 30 seconds
    vTaskDelay(pdMS_TO_TICKS(30000));
  
    // Stop all the actions
    Serial.println("Stopping test actions...");
    periodicNeoPixelAction.stop();
    periodicMotorAction.stop();
  
    // Wait for the actions to stop
    // MOFIX - maybe add a boolean to say whether an action is still running instead of using a delay
    Serial.println("Waiting for test actions to stop...");
    vTaskDelay(pdMS_TO_TICKS(10000));
  
    Serial.println("Startup Tests completed.");
  }
