#ifndef CONTROLS_H
#define CONTROLS_H

#include <Arduino.h>
#include <ezButton.h>

#include "ArmBuilder.h"
#include "Structures.h"

#define JOY_LEFT_X_PIN 4
#define JOY_LEFT_Y_PIN 3

#define JOY_RIGHT_X_PIN 14
#define JOY_RIGHT_Y_PIN 13

#define SERVO_PIN 3

#define JOY_LEFT_BTN_PIN 43
#define JOY_RIGHT_BTN_PIN 11

#define BTN1_PIN 4
#define BTN2_PIN 5

class Controls
{
public:
  Controls();
  void init(ArmBuilder& armBuilder, const uint16_t nDebounceTime);
  void loop();
  bool isPlaying();
  bool isSwitched();

  int getLeftX();
  int getLeftY();
  int getRightX();
  int getRightY();

  Inputs getInputs();

private:
  ArmBuilder mArmBuilder;

  int mLeftX;
  int mLeftY;
  int mRightX;
  int mRightY;

  int mLeftValueX = 520;
  int mLeftValueY = 520;
  int mRightValueX = 520;
  int mRightValueY = 520;

  bool mPlaying = false;
  bool mSwitched = false;

  int analog1SWflag = 0;
  int analog1SWlastValue = 1;

  int analog2SWflag = 0;
  int analog2SWlastValue = 1;

  int analog3SWflag = 0;
  int analog3SWlastValue = 1;

  int analog4SWflag = 0;
  int analog4SWlastValue = 1;

  void handleJoysticks();
  void handleButtons();
};

#endif
