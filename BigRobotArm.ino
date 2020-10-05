#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"

#define SP2_RX 11
#define SP2_TX 12

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30
#define E1_CS_PIN 44

#define JOY_LEFT_X 3
#define JOY_LEFT_Y 4

#define JOY_RIGHT_X 14
#define JOY_RIGHT_Y 13

int leftX, leftY, rightX, rightY;
int valueX = 520;
int valueY = 520;

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

  // Serial.print(analogRead(JOY_LEFT_X));
  // Serial.print(", ");
  // Serial.println(analogRead(JOY_RIGHT_X));

  // Serial.print(analogRead(JOY_RIGHT_X));
  // Serial.print(", ");

  leftX = map(analogRead(JOY_LEFT_X), 0, 1023, -50, 50);
  leftY = map(analogRead(JOY_LEFT_Y), 0, 1023, -50, 50);

  // low pass filter for bad input values
  valueX = 0.9 * valueX + 0.1 * analogRead(JOY_RIGHT_X);
  valueY = 0.9 * valueY + 0.1 * analogRead(JOY_RIGHT_Y);
  rightX = map(valueX, 0, 1023, -50, 50);
  rightY = map(valueY, 0, 1023, -50, 50);

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
  // Serial.print("END loop");
  // delay(500);
}
