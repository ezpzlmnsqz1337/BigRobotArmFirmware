#include "SpeedCommand.h"
#include "Arduino.h"
#include "JointCommandRules.h"

SpeedCommand::SpeedCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

SpeedCommand::~SpeedCommand()
{
}

void SpeedCommand::parse(char* cCommand)
{
  mJS = mArmBuilder->getSpeeds();
  isValid = parseJointSpeedsCommand(cCommand, mJS);
}

void SpeedCommand::execute()
{
  mArmBuilder->setSpeeds(mJS);
}

void SpeedCommand::printResponse()
{
  JointSpeeds ja = mArmBuilder->getSpeeds();
  Serial.print("BigRobotArm::SPEED: ");
  Serial.print("B");
  Serial.print(ja.base);
  Serial.print(" S");
  Serial.print(ja.shoulder);
  Serial.print(" E");
  Serial.print(ja.elbow);
  Serial.print(" WR");
  Serial.print(ja.wristRotate);
  Serial.print(" W");
  Serial.println(ja.wrist);
}
