#include "Gripper.h"

const int32_t minPos = 40;
const int32_t maxPos = 150;

Gripper::Gripper() : mServo(3, minPos, maxPos, 90)
{
}

void Gripper::init()
{
  mServo.init();
}

void Gripper::open()
{
  mServo.moveTo(minPos);
}

void Gripper::close()
{
  mServo.moveTo(maxPos);
}

MyServo& Gripper::getServo()
{
  return mServo;
}
