#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "AccelStepper.h"
#include "MultiStepper.h"

#define X_STEP_PIN 54
#define X_DIR_PIN 55
#define X_ENABLE_PIN 38
#define X_CS_PIN 53

#define Y_STEP_PIN 60
#define Y_DIR_PIN 61
#define Y_ENABLE_PIN 56
#define Y_CS_PIN 49

#define Z_STEP_PIN 46
#define Z_DIR_PIN 48
#define Z_ENABLE_PIN 62
#define Z_CS_PIN 40

#define E0_STEP_PIN 26
#define E0_DIR_PIN 28
#define E0_ENABLE_PIN 24
#define E0_CS_PIN 42

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30
#define E1_CS_PIN 44

const uint16_t stepsPerRevolution = 200 * 16; // change this to fit the number of steps per revolution
// for your motor

// Alas its not possible to build an array of these with different pins for each :-(
AccelStepper stepper1(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepper2(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
AccelStepper stepper3(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);
AccelStepper stepper4(AccelStepper::DRIVER, E0_STEP_PIN, E0_DIR_PIN);
// Up to 10 steppers can be handled as a group by MultiStepper
MultiStepper steppers;

void setup()
{
  Serial.begin(9600);
  // Configure each stepper
  stepper1.setMaxSpeed(1000 * stepsPerRevolution);     // 100mm/s @ 80 steps/mm
  stepper1.setAcceleration(1000 * stepsPerRevolution); // 2000mm/s^2
  stepper1.setEnablePin(X_ENABLE_PIN);
  stepper1.setPinsInverted(false, false, true);
  stepper1.enableOutputs();

  stepper2.setMaxSpeed(1000 * stepsPerRevolution);     // 100mm/s @ 80 steps/mm
  stepper2.setAcceleration(1000 * stepsPerRevolution); // 2000mm/s^2
  stepper2.setEnablePin(Y_ENABLE_PIN);
  stepper2.setPinsInverted(false, false, true);
  stepper2.enableOutputs();

  stepper3.setMaxSpeed(1000 * stepsPerRevolution);     // 100mm/s @ 80 steps/mm
  stepper3.setAcceleration(1000 * stepsPerRevolution); // 2000mm/s^2
  stepper3.setEnablePin(Z_ENABLE_PIN);
  stepper3.setPinsInverted(false, false, true);
  stepper3.enableOutputs();

  stepper4.setMaxSpeed(1000 * stepsPerRevolution);     // 100mm/s @ 80 steps/mm
  stepper4.setAcceleration(1000 * stepsPerRevolution); // 2000mm/s^2
  stepper4.setEnablePin(E0_ENABLE_PIN);
  stepper4.setPinsInverted(false, false, true);
  stepper4.enableOutputs();
  // Then give them to MultiStepper to manage
  steppers.addStepper(stepper1);
  steppers.addStepper(stepper2);
  steppers.addStepper(stepper3);
  steppers.addStepper(stepper4);
}

void loop()
{
  long positions[4]; // Array of desired stepper positions

  positions[0] = stepsPerRevolution;
  positions[1] = stepsPerRevolution;
  positions[2] = stepsPerRevolution;
  positions[3] = stepsPerRevolution;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);

  // Move to a different coordinate
  positions[0] = 2 * stepsPerRevolution;
  positions[1] = 2 * stepsPerRevolution;
  positions[2] = 2 * stepsPerRevolution;
  positions[3] = 2 * stepsPerRevolution;
  steppers.moveTo(positions);
  steppers.runSpeedToPosition(); // Blocks until all are in position
  delay(1000);
}
