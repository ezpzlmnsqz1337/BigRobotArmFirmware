#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>

#include "AccelStepper.h"

#include "ArmBuilder.h"

#define SP2_RX 11
#define SP2_TX 12

#define E1_STEP_PIN 36
#define E1_DIR_PIN 34
#define E1_ENABLE_PIN 30
#define E1_CS_PIN 44

#define JOY_LEFT_X_PIN 4
#define JOY_LEFT_Y_PIN 3

#define JOY_RIGHT_X_PIN 14
#define JOY_RIGHT_Y_PIN 13

#define SERVO_PIN 3

#define JOY_LEFT_BTN_PIN 11
#define JOY_RIGHT_BTN_PIN 43

#define BTN1_PIN 4
#define BTN2_PIN 5

bool switched = false;
bool playing = false;

int leftX, leftY, rightX, rightY;
int leftValueX = 520;
int leftValueY = 520;

int rightValueX = 520;
int rightValueY = 520;

int analog1SW = 1;
int analog1SWflag = 0;
int analog1SWlastValue = 1;

int analog2SW = 1;
int analog2SWflag = 0;
int analog2SWlastValue = 1;

int analog3SW = 1;
int analog3SWflag = 0;
int analog3SWlastValue = 1;

int analog4SW = 1;
int analog4SWflag = 0;
int analog4SWlastValue = 1;

ArmBuilder armBuilder;

const int MAX_POSITIONS = 50;

long savedPositions[MAX_POSITIONS][6];
int numOfSavedPositions = 0;
int currentPosition = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("SETUP");
  pinMode(SERVO_PIN, OUTPUT);

  pinMode(JOY_LEFT_X_PIN, INPUT);
  pinMode(JOY_LEFT_Y_PIN, INPUT);

  pinMode(JOY_RIGHT_X_PIN, INPUT);
  pinMode(JOY_RIGHT_Y_PIN, INPUT);

  armBuilder.init();
  Serial.println("SETUP END");
}

void loop()
{
  handleAnalogButtons();
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

  if (playing)
  {
    Serial.println("playing");
    if (currentPosition == numOfSavedPositions + 1)
    {
      currentPosition = 0;
    }
    armBuilder.goTo(savedPositions[currentPosition][0], savedPositions[currentPosition][1],
                    savedPositions[currentPosition][2], savedPositions[currentPosition][3],
                    savedPositions[currentPosition][4]);
    currentPosition++;
  }
  else
  {
    Serial.println("not playing");
    // low pass filter for bad input values
    leftValueX = 0.9 * leftValueX + 0.1 * analogRead(JOY_LEFT_X_PIN);
    leftValueY = 0.9 * leftValueY + 0.1 * analogRead(JOY_LEFT_Y_PIN);
    leftX = map(leftValueX, 100, 923, -400, 400);
    leftY = map(leftValueY, 100, 923, -400, 400);

    // low pass filter for bad input values
    rightValueX = 0.9 * rightValueX + 0.1 * analogRead(JOY_RIGHT_X_PIN);
    rightValueY = 0.9 * rightValueY + 0.1 * analogRead(JOY_RIGHT_Y_PIN);
    rightX = map(rightValueX, 100, 923, -400, 400);
    rightY = map(rightValueY, 100, 923, -400, 400);

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
    if (switched)
    {
      armBuilder.move(leftX, leftY, rightX, 0, rightY);
    }
    else
    {
      armBuilder.move(leftX, leftY, rightX, rightY, 0);
    }
  }
  // Serial.print("END loop");
  delay(500);
}

void handleAnalogButtons()
{
  analog1SW = digitalRead(JOY_LEFT_BTN_PIN);
  analog2SW = digitalRead(JOY_RIGHT_BTN_PIN);
  analog3SW = digitalRead(BTN1_PIN);
  analog4SW = digitalRead(BTN2_PIN);

  Serial.print("analog1: ");
  Serial.println(analog1SW);
  Serial.print("analog2: ");
  Serial.println(analog2SW);
  Serial.print("analog3: ");
  Serial.println(analog3SW);
  Serial.print("analog4: ");
  Serial.println(analog4SW);

  // save position
  if (analog1SW == LOW)
  {
    if (analog1SWlastValue == 1)
    {
      analog1SWlastValue = 0;
      if (analog1SWflag == 0)
      {
        switched = false;
        analog1SWflag = 1;
      }
      else if (analog1SWflag == 1)
      {
        switched = true;
        analog1SWflag = 0;
      }
    }
  }
  else
  {
    analog1SWlastValue = 1;
  }

  // grip
  if (analog2SW == LOW)
  {
    if (analog2SWlastValue == 1)
    {
      analog2SWlastValue = 0;
      if (analog2SWflag == 0)
      {
        armBuilder.getGripper().close();
        analog2SWflag = 1;
      }
      else if (analog2SWflag == 1)
      {
        armBuilder.getGripper().open();
        analog2SWflag = 0;
      }
    }
  }
  else
  {
    analog2SWlastValue = 1;
  }

  // save
  if (analog3SW == LOW)
  {
    if (analog3SWlastValue == 1)
    {
      analog3SWlastValue = 0;
      if (analog3SWflag == 0)
      {
        save();
        analog3SWflag = 1;
      }
      else if (analog3SWflag == 1)
      {
        save();
        analog3SWflag = 0;
      }
    }
  }
  else
  {
    analog3SWlastValue = 1;
  }

  // play
  if (analog4SW == LOW)
  {
    if (analog4SWlastValue == 1)
    {
      analog4SWlastValue = 0;
      if (analog4SWflag == 0)
      {
        play();
        analog4SWflag = 1;
      }
      else if (analog4SWflag == 1)
      {
        stop();
        analog4SWflag = 0;
      }
    }
  }
  else
  {
    analog4SWlastValue = 1;
  }
}

void play()
{
  playing = true;
}

void stop()
{
  playing = false;
}

void save()
{
  if (numOfSavedPositions > MAX_POSITIONS - 1)
  {
    numOfSavedPositions = 0;
  }
  savedPositions[numOfSavedPositions][0] = armBuilder.getBase().getMotor().currentPosition();
  savedPositions[numOfSavedPositions][1] = armBuilder.getShoulder().getMotor().currentPosition();
  savedPositions[numOfSavedPositions][2] = armBuilder.getElbow().getMotor().currentPosition();
  savedPositions[numOfSavedPositions][3] = armBuilder.getWrist1().getMotor().currentPosition();
  savedPositions[numOfSavedPositions][4] = armBuilder.getWrist2().getMotor().currentPosition();
  savedPositions[numOfSavedPositions][5] = armBuilder.getGripper().getServo().getPosition();

  numOfSavedPositions++;
}