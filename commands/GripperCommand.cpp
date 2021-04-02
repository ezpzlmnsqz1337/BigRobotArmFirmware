#include "GripperCommand.h"
#include "Arduino.h"

GripperCommand::GripperCommand(const ArmBuilder& armBuilder) : AbstractCommand(armBuilder), mEnabled(0), mPosition(0)
{
}

GripperCommand::~GripperCommand()
{
  Serial.println("Deleting gripper");
}

void GripperCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // G1 - handle gripper
  char* e = strtok(NULL, " ");
  char* p = strtok(NULL, " ");
  mEnabled = e != NULL ? atoi(&e[1]) : 0;
  mPosition = p != NULL ? atoi(&p[1]) : 0;
}

void GripperCommand::execute()
{
  if (mEnabled == 1)
  {
    mArmBuilder.getGripper().init();
    JointPositions jp = mArmBuilder.getPositions();
    jp.gripper = mPosition;
    mArmBuilder.goTo(jp);
  }
  else if (mEnabled == 0)
  {
    mArmBuilder.getGripper().getServo().deinit();
  }
}

void GripperCommand::printResponse()
{
  bool enable = mArmBuilder.getGripper().getServo().isEnabled();
  int32_t position = mArmBuilder.getPositions().gripper;
  Serial.print("BigRobotArm::GRIPPER: ");
  Serial.print("E");
  Serial.print(enable ? 1 : 0);
  Serial.print(" P");
  Serial.println(position);
}
