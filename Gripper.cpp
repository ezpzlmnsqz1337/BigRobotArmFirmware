#include "Gripper.h"

#include "Config.h"

Gripper::Gripper()
  : mServo(3, static_cast<int32_t>(Config::MIN_GRIPPER_POSITION),
           static_cast<int32_t>(Config::MAX_GRIPPER_POSITION), 90)
{
}

void Gripper::init()
{
  mServo.init();
}

void Gripper::open()
{
  mServo.moveTo(static_cast<int32_t>(Config::MIN_GRIPPER_POSITION));
}

void Gripper::close()
{
  mServo.moveTo(static_cast<int32_t>(Config::MAX_GRIPPER_POSITION));
}

MyServo& Gripper::getServo()
{
  return mServo;
}
