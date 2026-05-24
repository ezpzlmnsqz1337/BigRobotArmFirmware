#include "CommandResponseRules.h"

const char* readyResponseMessage()
{
  return "BigRobotArm::READY";
}

const char* invalidCommandResponseMessage()
{
  return "BigRobotArm::INVALID-COMMAND";
}