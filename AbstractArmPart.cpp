#include "AbstractArmPart.h"


AbstractArmPart::AbstractArmPart()
{
}

void AbstractArmPart::init()
{
  mMotor = AccelStepper(AccelStepper::DRIVER, mStepPin, mDirPin);
  mMotor.setMaxSpeed(mMaxSpeed);     // 100mm/s @ 80 steps/mm
  mMotor.setAcceleration(mAcceleration); // 2000mm/s^2
  mMotor.setEnablePin(mEnablePin);
  mMotor.setPinsInverted(false, false, true);
  mMotor.enableOutputs();
}

AccelStepper& AbstractArmPart::getMotor()
{
  return mMotor;
}


void AbstractArmPart::setMaxSpeed(const int maxSpeed){
  mMaxSpeed = maxSpeed;
  mMotor.setMaxSpeed(maxSpeed);
}

void AbstractArmPart::setAcceleration(const int acceleration){
  mMaxSpeed = acceleration;
  mMotor.setAcceleration(acceleration);
}


int AbstractArmPart::getMaxSpeed(){
  return mMaxSpeed;
}

int AbstractArmPart::getAcceleration(){
  return mAcceleration;
}
