#include "AccelerationCommand.h"
#include "Arduino.h"
#include "JointCommandRules.h"

AccelerationCommand::AccelerationCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

AccelerationCommand::~AccelerationCommand()
{
}

void AccelerationCommand::parse(char* cCommand)
{
  mJA = mArmBuilder->getAccelerations();
  isValid = parseJointAccelerationsCommand(cCommand, mJA);
}

void AccelerationCommand::execute()
{
  mArmBuilder->setAccelerations(mJA);
}

void AccelerationCommand::printResponse()
{
  JointAccelerations ja = mArmBuilder->getAccelerations();
  Serial.print("BigRobotArm::ACCELERATION: ");
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
