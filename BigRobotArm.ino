#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30
#define E1_CS_PIN 44

#define JOY_LEFT_X 3
#define JOY_LEFT_Y 4

#define JOY_RIGHT_X 2
#define JOY_RIGHT_Y 6

int leftX, leftY, rightX, rightY = 0;

ArmBuilder armBuilder;

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");
  armBuilder.init();
  pinMode(JOY_LEFT_X, INPUT);
  pinMode(JOY_LEFT_Y, INPUT);

  pinMode(JOY_RIGHT_X, INPUT);
  pinMode(JOY_RIGHT_Y, INPUT);
  Serial.println("SETUP END");
}

void loop()
{
  // Serial.println("LOOP");

  Serial.println(analogRead(JOY_RIGHT_X));
  Serial.println(analogRead(JOY_RIGHT_Y));
  leftX = map(analogRead(JOY_LEFT_X), 0, 1023, -20, 20);
  leftY = map(analogRead(JOY_LEFT_Y), 0, 1023, -20, 20);

  rightX = map(analogRead(JOY_RIGHT_X), 0, 1023, -20, 20);
  rightY = map(analogRead(JOY_RIGHT_Y), 0, 1023, -20, 20);

  // Serial.print(", LEFT X: ");
  // Serial.println(leftX);
  // Serial.print(", LEFT Y: ");
  // Serial.println(leftY);
  // Serial.print(", RIGHT X: ");
  // Serial.println(rightX);
  // Serial.print(", RIGHT Y: ");
  // Serial.println(rightY);
  // Serial.println();
  // Serial.println();
  armBuilder.move(leftX, leftY, rightX, rightY);
}
