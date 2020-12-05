#include "AbstractArmPart.h"

const uint16_t stepsPerRevolution = 200 * 16; // change this to fit the number of steps per revolution
const uint32_t maxSpeed = stepsPerRevolution;
const uint32_t acceleration = stepsPerRevolution;

AbstractArmPart::AbstractArmPart()
{
}

void AbstractArmPart::init()
{
  mMotor = AccelStepper(AccelStepper::DRIVER, mStepPin, mDirPin);
  mMotor.setMaxSpeed(maxSpeed);     // 100mm/s @ 80 steps/mm
  mMotor.setAcceleration(acceleration); // 2000mm/s^2
  mMotor.setEnablePin(mEnablePin);
  mMotor.setPinsInverted(false, false, true);
  mMotor.enableOutputs();
}

AccelStepper& AbstractArmPart::getMotor()
{
  return mMotor;
}
