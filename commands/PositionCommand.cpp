#include "PositionCommand.h"
#include "Arduino.h"

PositionCommand::PositionCommand(ArmBuilder* armBuilder) : AbstractCommand(armBuilder)
{
}

PositionCommand::~PositionCommand()
{
}

void PositionCommand::parse(char* cCommand)
{
  strtok(cCommand, " ");
  // handle positions
  char* base = strtok(NULL, " ");
  char* shoulder = strtok(NULL, " ");
  char* elbow = strtok(NULL, " ");
  char* wristRotate = strtok(NULL, " ");
  char* wrist = strtok(NULL, " ");

  mJP = mArmBuilder->getPositions();
  mJP.base = base != NULL ? atol(&base[1]) : mJP.base;
  mJP.shoulder = shoulder != NULL ? atol(&shoulder[1]) : mJP.shoulder;
  mJP.elbow = elbow != NULL ? atol(&elbow[1]) : mJP.elbow;
  mJP.wristRotate = wristRotate != NULL ? atol(&wristRotate[2]) : mJP.wristRotate;
  mJP.wrist = wrist != NULL ? atol(&wrist[1]) : mJP.wrist;
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
