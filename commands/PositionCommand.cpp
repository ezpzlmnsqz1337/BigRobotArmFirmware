#include "PositionCommand.h"
#include "Arduino.h"
#include "JointCommandRules.h"

PositionCommand::PositionCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

PositionCommand::~PositionCommand()
{
}

void PositionCommand::parse(char* cCommand)
{
  mJP = mArmBuilder->getPositions();
  isValid = parseJointPositionsCommand(cCommand, mJP);
}

void PositionCommand::execute()
{
  mArmBuilder->goTo(mJP);
}

void PositionCommand::printResponse()
{
  JointPositions jp = mArmBuilder->getPositions();
  Serial.println("BigRobotArm::MOVING-TO");
  Serial.print("BigRobotArm::POSITION: ");
  Serial.print("B");
  Serial.print(jp.base);
  Serial.print(" S");
  Serial.print(jp.shoulder);
  Serial.print(" E");
  Serial.print(jp.elbow);
  Serial.print(" WR");
  Serial.print(jp.wristRotate);
  Serial.print(" W");
  Serial.println(jp.wrist);
}
