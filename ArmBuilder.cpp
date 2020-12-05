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
  mElbow.init();
  mWrist1.init();
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

void ArmBuilder::move(const int base, const int shoulder, const int elbow, const int wrist1,  const int wrist2)
{
  const int deadZone = 7;
  long positions[5]; // Array of desired stepper positions

  positions[0] = abs(base) > deadZone ? base + mBase.getMotor().currentPosition() : mBase.getMotor().currentPosition();
  positions[1] = abs(shoulder) > deadZone ? shoulder + mShoulder.getMotor().currentPosition() : mShoulder.getMotor().currentPosition();
  positions[2] = abs(elbow) > deadZone ? elbow + mElbow.getMotor().currentPosition() : mElbow.getMotor().currentPosition();
  positions[3] = abs(wrist1) > deadZone ? wrist1 + mWrist1.getMotor().currentPosition() : mWrist1.getMotor().currentPosition();
  positions[4] = abs(wrist2) > deadZone ? wrist2 + mWrist2.getMotor().currentPosition() : mWrist2.getMotor().currentPosition();

  mSteppers.moveTo(positions);
  mSteppers.runSpeedToPosition(); // Blocks until all are in position
}
