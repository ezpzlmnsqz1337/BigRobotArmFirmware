#include "RobotArmJoint.h"

RobotArmJoint::RobotArmJoint()
{
}

void RobotArmJoint::init(const int32_t nStepPin, const int32_t nDirPin, const int32_t nEnablePin,
                         const long nStepsPerRevolution, Microstepping microstepping)
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
  mMaxSpeed = mStepsPerRevolution * nMultiplier;
  mMotor.setMaxSpeed(mMaxSpeed);
}

void RobotArmJoint::setAcceleration(const float acceleration)
{
  mAcceleration = acceleration;
  mMotor.setAcceleration(acceleration);
}

void RobotArmJoint::setAccelerationMultiplier(const float nMultiplier)
{
  mAcceleration = mStepsPerRevolution * nMultiplier;
  mMotor.setAcceleration(mAcceleration);
}

int32_t RobotArmJoint::getMaxSpeed()
{
  return mMaxSpeed;
}

int32_t RobotArmJoint::getAcceleration()
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
