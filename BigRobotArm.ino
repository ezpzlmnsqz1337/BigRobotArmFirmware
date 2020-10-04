#include <stdio.h>
#include <stdlib.h>
#include <Arduino.h>
#include <SoftwareSerial.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"

#define SP2_RX 11
#define SP2_TX 12

SoftwareSerial gamepad(SP2_RX, SP2_TX);

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30
#define E1_CS_PIN 44

#define JOY_LEFT_X 3
#define JOY_LEFT_Y 4

#define JOY_RIGHT_X 14
#define JOY_RIGHT_Y 6

char cmd; // Reads the data from the serial port
int leftX, leftY, rightX, rightY, sofar = 0;
const int BUFFER_SIZE = 100;
char buffer[BUFFER_SIZE];

ArmBuilder armBuilder;

void setup()
{
  Serial.begin(9600);
  gamepad.begin(57600);
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

  sofar = 0;
  if (gamepad.available() > 0)
  {
    do
    {
      cmd = gamepad.read();
      if (sofar < BUFFER_SIZE)
      {
        buffer[sofar] = cmd;
        sofar++;
      }
    } while (cmd != '\n');
    Serial.print(buffer);
  }

  // Serial.print(analogRead(JOY_RIGHT_X));
  // Serial.print(", ");
  // Serial.println(analogRead(JOY_RIGHT_Y));
  // leftX = map(analogRead(JOY_LEFT_X), 0, 1023, -20, 20);
  // leftY = map(analogRead(JOY_LEFT_Y), 0, 1023, -20, 20);

  // rightX = map(analogRead(JOY_RIGHT_X), 0, 1023, -20, 20);
  // rightY = map(analogRead(JOY_RIGHT_Y), 0, 1023, -20, 20);

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
  // armBuilder.move(leftX, leftY, rightX, rightY);
  // Serial.print("END loop");
  delay(500);
}
