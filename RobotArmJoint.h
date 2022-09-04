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
  void init(const int32_t nStepPin, const int32_t nDirPin, const int32_t nEnablePin, const long nStepsPerRevolution,
            Microstepping microsteping);

  void setMaxSpeed(const float maxSpeed);
  int32_t getMaxSpeed();
  void setAcceleration(const float acceleration);
  int32_t getAcceleration();

  void setMaxSpeedMultiplier(const float nMultiplier);
  void setAccelerationMultiplier(const float nMultiplier);
  float getMaxSpeedMultiplier();
  float getAccelerationMultiplier();

  AccelStepper& getMotor();
};

#endif
