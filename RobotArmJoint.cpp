#include "RobotArmJoint.h"

RobotArmJoint::RobotArmJoint()
{
}

void RobotArmJoint::init(const int nStepPin, const int nDirPin, const int nEnablePin, const long nStepsPerRevolution,
                         Microstepping microstepping)
{
  mStepPin = nStepPin;
  mDirPin = nDirPin;
  mEnablePin = nEnablePin;

  mStepsPerRevolution = nStepsPerRevolution * microstepping;
  mAcceleration = mStepsPerRevolution;
  mMaxSpeed = mStepsPerRevolution;

  mMotor = AccelStepper(AccelStepper::DRIVER, mStepPin, mDirPin);
  mMotor.setMaxSpeed(mMaxSpeed);
  mMotor.setAcceleration(mAcceleration);
  mMotor.setEnablePin(mEnablePin);
  mMotor.setPinsInverted(false, false, true);
  mMotor.enableOutputs();
}

AccelStepper& RobotArmJoint::getMotor()
{
  return mMotor;
}

void RobotArmJoint::setMaxSpeed(const float maxSpeed)
{
  mMaxSpeed = maxSpeed;
  mMotor.setMaxSpeed(maxSpeed);
}

void RobotArmJoint::setMaxSpeedMultiplier(const float nMultiplier)
{
  mMotor.setMaxSpeed(mStepsPerRevolution * nMultiplier);
}

void RobotArmJoint::setAcceleration(const float acceleration)
{
  mMaxSpeed = acceleration;
  mMotor.setAcceleration(acceleration);
}

void RobotArmJoint::setAccelerationMultiplier(const float nMultiplier)
{
  mMotor.setAcceleration(mStepsPerRevolution * nMultiplier);
}

int RobotArmJoint::getMaxSpeed()
{
  return mMaxSpeed;
}

int RobotArmJoint::getAcceleration()
{
  return mAcceleration;
}

float RobotArmJoint::getMaxSpeedMultiplier()
{
  return mMaxSpeed / mStepsPerRevolution;
}

float RobotArmJoint::getAccelerationMultiplier()
{
  return mAcceleration / mStepsPerRevolution;
}
