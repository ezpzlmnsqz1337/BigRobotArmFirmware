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

#define JOY_LEFT_X_PIN 3
#define JOY_LEFT_Y_PIN 4

#define JOY_RIGHT_X_PIN 14
#define JOY_RIGHT_Y_PIN 13

#define SERVO_PIN 3

int leftX, leftY, rightX, rightY, centerX;
int leftValueX = 520;
int leftValueY = 520;

int rightValueX = 520;
int rightValueY = 520;

ArmBuilder armBuilder;

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");
  armBuilder.init();
  pinMode(SERVO_PIN, INPUT);

  pinMode(JOY_LEFT_X_PIN, INPUT);
  pinMode(JOY_LEFT_Y_PIN, INPUT);

  pinMode(JOY_RIGHT_X_PIN, INPUT);
  pinMode(JOY_RIGHT_Y_PIN, INPUT);
  Serial.println("SETUP END");
}

void loop()
{
  // Serial.println("LOOP");

  // Serial.print(analogRead(JOY_LEFT_X_PIN));
  // Serial.print(", ");
  // Serial.print(analogRead(JOY_LEFT_Y_PIN));
  // Serial.println(" ---");

  // Serial.print(analogRead(JOY_RIGHT_X_PIN));
  // Serial.print(", ");

  // Serial.print(analogRead(JOY_RIGHT_Y_PIN));
  // Serial.println();

  // leftX = map(analogRead(JOY_LEFT_X_PIN), 0, 1023, -50, 50);
  // leftY = map(analogRead(JOY_LEFT_Y_PIN), 0, 1023, -50, 50);

  // low pass filter for bad input values
  leftValueX = 0.9 * leftValueX + 0.1 * analogRead(JOY_LEFT_X_PIN);
  leftValueY = 0.9 * leftValueY + 0.1 * analogRead(JOY_LEFT_Y_PIN);
  leftX = map(leftValueX, 100, 923, -50, 50);
  leftY = map(leftValueY, 100, 923, -50, 50);


  // low pass filter for bad input values
  rightValueX = 0.9 * rightValueX + 0.1 * analogRead(JOY_RIGHT_X_PIN);
  rightValueY = 0.9 * rightValueY + 0.1 * analogRead(JOY_RIGHT_Y_PIN);
  rightX = map(rightValueX, 100, 923, -50, 50);
  rightY = map(rightValueY, 100, 923, -50, 50);

  // fake last analog control input
  centerX = 0;

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
  armBuilder.move(leftX, leftY, rightX, rightY, centerX);
  // Serial.print("END loop");
  // delay(100);
}
