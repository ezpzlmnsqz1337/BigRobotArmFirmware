#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30
#define E1_CS_PIN 44

#define JOY_LEFT_X 39
#define JOY_LEFT_Y 45

#define JOY_RIGHT_X 32
#define JOY_RIGHT_Y 47

int leftX, leftY, rightX, rightY = 0;

ArmBuilder armBuilder;

void setup()
{
  Serial.begin(9600);
  pinMode(JOY_LEFT_X, INPUT);
  pinMode(JOY_LEFT_Y, INPUT);

  pinMode(JOY_RIGHT_X, INPUT);
  pinMode(JOY_RIGHT_Y, INPUT);
}

void loop()
{
  leftX = digitalRead(JOY_LEFT_X);
  leftY = digitalRead(JOY_LEFT_Y);

  rightX = digitalRead(JOY_RIGHT_X);
  rightY = digitalRead(JOY_RIGHT_Y);

  map(leftX, 0, 1023, -20, 20);
  map(leftY, 0, 1023, -20, 20);

  map(rightX, 0, 1023, -20, 20);
  map(rightY, 0, 1023, -20, 20);

  armBuilder.move(leftX, leftY, rightX, rightY);

  delay(1000);
}
