#ifndef ROBOT_ARM_JOINT_H
#define ROBOT_ARM_JOINT_H

#include "AccelStepper.h"
#include "Config.h"

class RobotArmJoint
{
protected:
  AccelStepper mMotor;
  uint8_t mStepPin;
  uint8_t mDirPin;
  uint8_t mEnablePin;

  long mStepsPerRevolution;
  float mMaxSpeed;
  float mAcceleration;

public:
  RobotArmJoint();
  void init(const int nStepPin, const int nDirPin, const int nEnablePin, const long nStepsPerRevolution,
            Microstepping microsteping);

  void setMaxSpeed(const float maxSpeed);
  int getMaxSpeed();
  void setAcceleration(const float acceleration);
  int getAcceleration();

  void setMaxSpeedMultiplier(const float nMultiplier);
  void setAccelerationMultiplier(const float nMultiplier);

  AccelStepper& getMotor();
};

// This is the end of the header guard
#endif