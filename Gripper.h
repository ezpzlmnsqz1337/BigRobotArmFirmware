#ifndef GRIPPER_H
#define GRIPPER_H

#include "MyServo.h"

class Gripper
{
protected:
  MyServo mServo;

public:
  Gripper();
  void init();
  void open();
  void close();
  MyServo& getServo();
};

#endif
