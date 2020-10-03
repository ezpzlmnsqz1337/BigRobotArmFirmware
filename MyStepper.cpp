#include "MyStepper.h"
#include "MotorDirection.h"
#include "Arduino.h"

MyStepper::MyStepper(int _stepsPerRevolution, int _stepPin, int _dirPin, int _resetPin)
    : stepsPerRevolution(_stepsPerRevolution), stepPin(_stepPin), dirPin(_dirPin), resetPin(_resetPin), pos(90)
{
  this->direction = MotorDirection::HOME;
}

void MyStepper::init()
{
  pinMode(this->stepPin, OUTPUT);
  pinMode(this->dirPin, OUTPUT);
  pinMode(this->resetPin, OUTPUT);

  digitalWrite(this->dirPin, LOW);
  digitalWrite(this->resetPin, LOW);
}

void MyStepper::step(int speed)
{
  digitalWrite(this->stepPin, HIGH);
  delayMicroseconds(speed);
  digitalWrite(this->stepPin, LOW);
  delayMicroseconds(speed);
}

void MyStepper::move()
{
}

void MyStepper::moveSteps(int steps)
{
  for (int i = 0; i < steps; i++)
  {
    this->step(100);
  }
}

void MyStepper::spin()
{
  // Spin the stepper motor 1 revolution slowly:
  for (int i = 0; i < stepsPerRevolution; i++)
  {
    // These four lines result in 1 step:
    digitalWrite(this->stepPin, HIGH);
    delayMicroseconds(500);
    digitalWrite(this->stepPin, LOW);
    delayMicroseconds(500);
  }
}

void MyStepper::setDirection(MotorDirection direction)
{
  if (direction == MotorDirection::NOT_HOME)
  {
    Serial.println("Set direction: NOT HOME");
    digitalWrite(this->dirPin, HIGH);
    this->direction = MotorDirection::NOT_HOME;
  }
  else
  {
    Serial.println("Set direction: HOME");
    digitalWrite(this->dirPin, LOW);
    this->direction = MotorDirection::HOME;
  }
}

MotorDirection MyStepper::getDirection()
{
  return this->direction;
}

void MyStepper::pause()
{
  digitalWrite(this->resetPin, HIGH);
}

void MyStepper::resume()
{
  digitalWrite(this->resetPin, LOW);
}

void MyStepper::moveTo(int value)
{
}
