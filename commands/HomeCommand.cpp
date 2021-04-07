#include "HomeCommand.h"
#include "Arduino.h"

HomeCommand::HomeCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

HomeCommand::~HomeCommand()
{
}

void HomeCommand::execute()
{
  JointPositions jp{0, 0, 0, 0, 0, 40};
  mArmBuilder->goTo(jp);
}

void HomeCommand::printResponse()
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
