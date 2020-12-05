#include "Gripper.h"

const int minPos = 0;
const int maxPos = 180;

Gripper::Gripper(): mServo(3, minPos, maxPos, 90)
{
  
}

void Gripper::init() {
  
}

void Gripper::open() {
  mServo.moveTo(minPos);
}

void Gripper::close() {
  mServo.moveTo(maxPos);
}

MyServo& Gripper::getServo()
{
  return mServo;
}
