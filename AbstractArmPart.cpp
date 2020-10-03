#include "AbstractArmPart.h"
#include "Arduino.h"

const uint16_t stepsPerRevolution = 200 * 16; // change this to fit the number of steps per revolution

AbstractArmPart::AbstractArmPart()
{
}

void AbstractArmPart::init()
{
  mMotor = AccelStepper(AccelStepper::DRIVER, mStepPin, mDirPin);
  mMotor.setMaxSpeed(1000 * stepsPerRevolution);     // 100mm/s @ 80 steps/mm
  mMotor.setAcceleration(1000 * stepsPerRevolution); // 2000mm/s^2
  mMotor.setEnablePin(mEnablePin);
  mMotor.setPinsInverted(false, false, true);
  mMotor.enableOutputs();
}

AccelStepper& AbstractArmPart::getMotor()
{
  return mMotor;
}
