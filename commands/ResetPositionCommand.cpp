#include "ResetPositionCommand.h"
#include "Arduino.h"

ResetPositionCommand::ResetPositionCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

ResetPositionCommand::~ResetPositionCommand()
{
}

void ResetPositionCommand::execute()
{
  mArmBuilder->setZeroPosition();
}

void ResetPositionCommand::printResponse()
{
  JointPositions jp = mArmBuilder->getPositions();
  {
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
}
