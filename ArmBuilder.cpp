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
  mWrist.init();
  mSteppers.addStepper(mBase.getMotor());
  mSteppers.addStepper(mShoulder.getMotor());
  mSteppers.addStepper(mElbow.getMotor());
  mSteppers.addStepper(mWrist.getMotor());
}

void ArmBuilder::goTo(const int base, const int shoulder, const int elbow, const int wrist)
{
  long positions[4]; // Array of desired stepper positions

  positions[0] = base;
  positions[1] = shoulder;
  positions[2] = elbow;
  positions[3] = wrist;

  mSteppers.moveTo(positions);
  mSteppers.runSpeedToPosition(); // Blocks until all are in position
}

void ArmBuilder::move(const int base, const int shoulder, const int elbow, const int wrist)
{
  long positions[4]; // Array of desired stepper positions

  positions[0] = base + mBase.getMotor().currentPosition();
  positions[1] = shoulder + mShoulder.getMotor().currentPosition();
  positions[2] = elbow + mElbow.getMotor().currentPosition();
  positions[3] = wrist + mWrist.getMotor().currentPosition();

  mSteppers.moveTo(positions);
  mSteppers.runSpeedToPosition(); // Blocks until all are in position
}
