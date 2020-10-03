// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef MY_STEPPER_H
#define MY_STEPPER_H

#include "MotorDirection.h"

// This is the content of the .h file, which is where the declarations go

class MyStepper
{
private:
  int pos;
  int stepPin;
  int dirPin;
  int resetPin;
  int stepsPerRevolution;
  MotorDirection direction;

public:
  MyStepper(int _stepsPerRevolution, int _stepPin, int _dirPin, int _resetPin);

  void init();

  void step(int speed);

  void move();

  void moveSteps(int steps);

  void spin();

  void setDirection(MotorDirection direction);

  MotorDirection getDirection();

  void pause();

  void resume();

  void moveTo(int value);
};

// This is the end of the header guard
#endif
