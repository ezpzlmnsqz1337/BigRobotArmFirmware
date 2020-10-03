// This is start of the header guard.  ADD_H can be any unique name.  By convention, we use the name of the header file.
#ifndef DRAWBOT_BUILDER_H
#define DRAWBOT_BUILDER_H

#include "MyStepper.h"
#include "MyServo.h"
#include "MotorDirection.h"
// This is the content of the .h file, which is where the declarations go

const unsigned int CANVAS_WIDTH = 150;
const unsigned int CANVAS_HEIGHT = 140;
const unsigned int ARM_1_LENGTH = 200;
const unsigned int ARM_2_LENGTH = 210;
const float ANGLE_PER_STEP = 0.1125;

class DrawbotBuilder
{
private:
  MyStepper motorX;
  MyStepper motorY;
  MyServo servoPen;
  int xEndstopPin;
  int yEndstopPin;

  int xPosition;
  int yPosition;

public:
  DrawbotBuilder(const MyStepper& _motorX, const MyStepper& _motorY, int _xEndstopPin, int _yEndstopPin,
                 const MyServo& _servoPen);

  void init();

  void home();

  void penUp();

  void penDown();

  void moveX(int steps);

  void moveY(int steps);

  void reset();

  void goTo(int x, int y, int speed, bool extrude);
};

// This is the end of the header guard
#endif
