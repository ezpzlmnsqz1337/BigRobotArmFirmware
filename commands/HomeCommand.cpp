#include "HomeCommand.h"
#include "Arduino.h"

HomeCommand::HomeCommand(const ArmBuilder& armBuilder) : AbstractCommand(armBuilder)
{
}

HomeCommand::~HomeCommand()
{
  Serial.println("Deleting home");
}

void HomeCommand::parse(char* cCommand)
{
}

void HomeCommand::execute()
{
  Serial.println("EX1");
  JointPositions jp{0, 0, 0, 0, 0, 40};
  Serial.println("EX2");
  mArmBuilder.goTo(jp);
  Serial.println("EX3");
}

void HomeCommand::printResponse()
{
  JointPositions jp = mArmBuilder.getPositions();
  {
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
}
