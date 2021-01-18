#ifndef ABSTRACT_ARM_PART_H
#define ABSTRACT_ARM_PART_H

#include "AccelStepper.h"

class AbstractArmPart
{
protected:
  AccelStepper mMotor;
  uint8_t mStepPin;
  uint8_t mDirPin;
  uint8_t mEnablePin;

  uint16_t mStepsPerRevolution = 200 * 16; // change this to fit the number of steps per revolution
  uint32_t mMaxSpeed = mStepsPerRevolution;
  uint32_t mAcceleration = mStepsPerRevolution;

public:
  AbstractArmPart();
  void init(const int nStepPin, const int nDirPin, const int nEnablePin);
  void setMaxSpeed(const int maxSpeed);
  int getMaxSpeed();
  void setAcceleration(const int acceleration);
  int getAcceleration();

  AccelStepper& getMotor();
};

// This is the end of the header guard
#endif
