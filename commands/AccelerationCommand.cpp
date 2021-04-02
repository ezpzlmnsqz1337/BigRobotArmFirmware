#include "AccelerationCommand.h"
#include "Arduino.h"

AccelerationCommand::AccelerationCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

AccelerationCommand::~AccelerationCommand()
{
}

void AccelerationCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // if acceleration command
  char* base = strtok(NULL, " ");
  char* shoulder = strtok(NULL, " ");
  char* elbow = strtok(NULL, " ");
  char* wristRotate = strtok(NULL, " ");
  char* wrist = strtok(NULL, " ");

  mJA = mArmBuilder->getAccelerations();
  mJA.base = base != NULL ? atoi(&base[1]) : mJA.base;
  mJA.shoulder = shoulder != NULL ? atoi(&shoulder[1]) : mJA.shoulder;
  mJA.elbow = elbow != NULL ? atoi(&elbow[1]) : mJA.elbow;
  mJA.wristRotate = wristRotate != NULL ? atoi(&wristRotate[2]) : mJA.wristRotate;
  mJA.wrist = wrist != NULL ? atoi(&wrist[1]) : mJA.wrist;
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
