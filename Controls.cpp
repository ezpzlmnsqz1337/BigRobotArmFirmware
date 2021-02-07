#include "Controls.h"

Controls::Controls()
{
}

void Controls::init(ArmBuilder& armBuilder, const uint16_t nDebounceTime)
{
  pinMode(SERVO_PIN, OUTPUT);

  pinMode(JOY_LEFT_X_PIN, INPUT);
  pinMode(JOY_LEFT_Y_PIN, INPUT);

  pinMode(JOY_RIGHT_X_PIN, INPUT);
  pinMode(JOY_RIGHT_Y_PIN, INPUT);

  mArmBuilder = armBuilder;
}

void Controls::loop()
{
  handleJoysticks();
  // handleButtons();
}

void Controls::handleJoysticks()
{
  // low pass filter for bad input values
  mLeftValueX = 0.9 * mLeftValueX + 0.1 * analogRead(JOY_LEFT_X_PIN);
  mLeftValueY = 0.9 * mLeftValueY + 0.1 * analogRead(JOY_LEFT_Y_PIN);
  mLeftX = map(mLeftValueX, 100, 923, -400, 400);
  mLeftY = map(mLeftValueY, 100, 923, -400, 400);

  // low pass filter for bad input values
  mRightValueX = 0.9 * mRightValueX + 0.1 * analogRead(JOY_RIGHT_X_PIN);
  mRightValueY = 0.9 * mRightValueY + 0.1 * analogRead(JOY_RIGHT_Y_PIN);
  mRightX = map(mRightValueX, 100, 923, -400, 400);
  mRightY = map(mRightValueY, 100, 923, -400, 400);
}

bool Controls::isPlaying()
{
  return mPlaying;
}

int Controls::getLeftX()
{
  return mLeftX;
}

int Controls::getLeftY()
{
  return mLeftY;
}

int Controls::getRightX()
{
  return mRightX;
}

int Controls::getRightY()
{
  return mRightY;
}

Inputs Controls::getInputs()
{
  return {mLeftX, mLeftY, mRightX, mRightY};
}