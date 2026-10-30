#include "GripperCommand.h"
#include "Arduino.h"
#include "CommandInputRules.h"
#include "FirmwareLimits.h"

GripperCommand::GripperCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder), mEnabled(0), mPosition(0)
{
}

GripperCommand::~GripperCommand()
{
}

void GripperCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // G1 - handle gripper
  char* e = strtok(NULL, " ");
  char* p = strtok(NULL, " ");

  if (e != nullptr)
  {
    int32_t enabled = 0;
    if (!parsePrefixedInt32(e, "E", &enabled) || (enabled != 0 && enabled != 1))
    {
      isValid = false;
      return;
    }
    mEnabled = static_cast<int8_t>(enabled);
  }

  if (p != nullptr)
  {
    int32_t position = 0;
    if (!parsePrefixedInt32(p, "P", &position))
    {
      isValid = false;
      return;
    }
    mPosition = clampGripperPosition(position);
  }
}

void GripperCommand::execute()
{
  if (mEnabled == 1)
  {
    mArmBuilder->getGripper().init();
    JointPositions jp = mArmBuilder->getPositions();
    jp.gripper = mPosition;
    mArmBuilder->goTo(jp);
  }
  else if (mEnabled == 0)
  {
    mArmBuilder->getGripper().getServo().deinit();
  }
}

void GripperCommand::printResponse()
{
  bool enable = mArmBuilder->getGripper().getServo().isEnabled();
  int32_t position = mArmBuilder->getPositions().gripper;
  Serial.print("BigRobotArm::GRIPPER: ");
  Serial.print("E");
  Serial.print(enable ? 1 : 0);
  Serial.print(" P");
  Serial.println(position);
}
