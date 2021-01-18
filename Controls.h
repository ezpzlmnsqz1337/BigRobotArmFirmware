#ifndef CONTROLS_H
#define CONTROLS_H

#include <Arduino.h>
#include <ezButton.h>

#include "ArmBuilder.h"

#define JOY_LEFT_X_PIN 4
#define JOY_LEFT_Y_PIN 3

#define JOY_RIGHT_X_PIN 14
#define JOY_RIGHT_Y_PIN 13

#define SERVO_PIN 3

#define JOY_LEFT_BTN_PIN 43
#define JOY_RIGHT_BTN_PIN 11

#define BTN1_PIN 4
#define BTN2_PIN 5

int leftX, leftY, rightX, rightY;
int leftValueX = 520;
int leftValueY = 520;

int rightValueX = 520;
int rightValueY = 520;

ezButton analog1SW(JOY_LEFT_BTN_PIN);
int analog1SWflag = 0;
int analog1SWlastValue = 1;

ezButton analog2SW(JOY_RIGHT_BTN_PIN);
int analog2SWflag = 0;
int analog2SWlastValue = 1;

ezButton analog3SW(BTN1_PIN);
int analog3SWflag = 0;
int analog3SWlastValue = 1;

ezButton analog4SW(BTN2_PIN);
int analog4SWflag = 0;
int analog4SWlastValue = 1;

class Controls
{
public:
  Controls();
  void init(ArmBuilder& armBuilder, const uint16_t nDebounceTime);
  void loop();
  bool isPlaying();
  bool isSwitched();

private:
  ArmBuilder mArmBuilder;
  bool mPlaying = false;
  bool mSwitched = false;
  void handleJoysticks();
  void handleButtons();
};

#endif
