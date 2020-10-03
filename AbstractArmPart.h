#ifndef ABSTRACT_ARM_PART_H
#define ABSTRACT_ARM_PART_H

#include "AccelStepper.h"

class AbstractArmPart
{
protected:
  AccelStepper mMotor;
  uint8_t mStepPin = 0;
  uint8_t mDirPin = 0;
  uint8_t mEnablePin = 0;
  uint8_t mCsPin = 0;

public:
  AbstractArmPart();
  void init();

  AccelStepper& getMotor();
};

// This is the end of the header guard
#endif
