#include "ArmBuilder.h"
#include "Arduino.h"
#include "math.h"

ArmBuilder::ArmBuilder()
{
}

void ArmBuilder::init()
{
  mBase.init();

  mShoulder.init();
  mShoulder.setMaxSpeed(mShoulder.getMaxSpeed() / 2);
  mShoulder.setAcceleration(mShoulder.getAcceleration() / 2);

  mElbow.init();
  mElbow.setMaxSpeed(mElbow.getMaxSpeed() * 2);
  mElbow.setAcceleration(mElbow.getAcceleration() * 2);

  mWrist1.init();
  mWrist1.setMaxSpeed(mWrist1.getMaxSpeed() / 2);
  mWrist1.setAcceleration(mWrist1.getAcceleration() / 2);

  mWrist2.init();
  mGripper.init();
  mSteppers.addStepper(mBase.getMotor());
  mSteppers.addStepper(mShoulder.getMotor());
  mSteppers.addStepper(mElbow.getMotor());
  mSteppers.addStepper(mWrist1.getMotor());
  mSteppers.addStepper(mWrist2.getMotor());
}

void ArmBuilder::goTo(const int base, const int shoulder, const int elbow, const int wrist1, const int wrist2)
{
  long positions[5]; // Array of desired stepper positions

  positions[0] = base;
  positions[1] = shoulder;
  positions[2] = elbow;
  positions[3] = wrist1;
  positions[4] = wrist2;

  mSteppers.moveTo(positions);
  mSteppers.runSpeedToPosition(); // Blocks until all are in position
}

void ArmBuilder::move(const int base, const int shoulder, const int elbow, const int wrist1, const int wrist2)
{
  const int deadZone = 30;
  long positions[5]; // Array of desired stepper positions

  int dBase = getNormalizedValue(base);
  int dShoulder = getNormalizedValue(shoulder);
  int dElbow = getNormalizedValue(elbow);
  int dWrist1 = getNormalizedValue(wrist1);
  int dWrist2 = getNormalizedValue(wrist2);

  positions[0] = dBase + mBase.getMotor().currentPosition();
  positions[1] = dShoulder + mShoulder.getMotor().currentPosition();
  positions[2] = dElbow + mElbow.getMotor().currentPosition();
  positions[3] = dWrist1 + mWrist1.getMotor().currentPosition();
  positions[4] = dWrist2 + mWrist2.getMotor().currentPosition();

  // Serial.print(", LEFT X: ");
  // Serial.println(dBase);
  // Serial.print(", LEFT Y: ");
  // Serial.println(dShoulder);
  // Serial.print(", RIGHT X: ");
  // Serial.println(dElbow);
  // Serial.print(", RIGHT Y: ");
  // Serial.println(dWrist1);
  // Serial.println();
  // Serial.println();

  mSteppers.moveTo(positions);
  mSteppers.runSpeedToPosition(); // Blocks until all are in position
}

int ArmBuilder::getNormalizedValue(const int value)
{
  const int deadZone = 30;
  return abs(value) > deadZone ? (value / 100) : 0;
}

Base& ArmBuilder::getBase()
{
  return mBase;
}

Shoulder& ArmBuilder::getShoulder()
{
  return mShoulder;
}

Elbow& ArmBuilder::getElbow()
{
  return mElbow;
}

Wrist& ArmBuilder::getWrist1()
{
  return mWrist1;
}

Wrist& ArmBuilder::getWrist2()
{
  return mWrist2;
}

Gripper& ArmBuilder::getGripper()
{
  return mGripper;
}
